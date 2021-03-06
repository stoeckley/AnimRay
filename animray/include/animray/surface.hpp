/*
    Copyright 2014-2018, Kirit Saelensminde.
    http://www.kirit.com/AnimRay

    This file is part of AnimRay.

    AnimRay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AnimRay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with AnimRay.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef ANIMRAY_SURFACE_HPP
#define ANIMRAY_SURFACE_HPP
#pragma once


#include <fost/core>
#include <animray/intersection.hpp>
#include <animray/shader.hpp>
#include <animray/emission.hpp>
#include <animray/maths/zip.hpp>


namespace animray {


    /// Forward declaration of the surface
    template< typename O, typename... S >
    class surface;


    /// Partial specialisation of the intersection type for a surface
    template< typename O, typename... S >
    class intersection< surface<O, S...> > : public O::intersection_type {
        typedef typename O::intersection_type superclass;
        typedef typename surface<O, S...>::surface_parameters_type
            surface_parameters_type;
        const surface_parameters_type *m_parameters;
    public:
        /// Default constructor (no intersection)
        intersection()
        : m_parameters(nullptr) {
        }

        /// Have an intersection so store the parameters
        intersection(const superclass &r,
            const surface_parameters_type &p)
        : superclass(r), m_parameters(&p) {
        }

        /// Allow access to the parameters
        const surface_parameters_type &parameters() const {
            return *m_parameters;
        }

        /// Handle multiplication for transformaion of co-ordinate spaces
        template<typename B>
        intersection operator * (const B &by) const {
            intersection r{*this};
            r.superclass::operator *= (by);
            return r;
        }
    };


    /// Stores the layers of a surface description
    template< typename O, typename... S >
    class surface {
    public:
        /// The underlying object type
        typedef O instance_type;
        /// The type of the local coordinate system
        typedef typename instance_type::local_coord_type local_coord_type;
        /// The physical model of the surface
        typedef std::tuple<typename S::parameters... >
            surface_parameters_type;
        /// The intersection type
        typedef intersection< surface > intersection_type;

        /// Default construct a surface
        surface() {}

        /// Pass the constructor arguments on to the underlying parameters
        surface(typename S::parameters... args)
        : surface_parameters(std::forward<typename S::parameters>(args)...) {
        }

        /// The geometry that is being shaded
        fostlib::accessors<instance_type, fostlib::lvalue> geometry;

        /// Capture the surface physics model
        fostlib::accessors< surface_parameters_type > surface_parameters;

        /// Pass on affine transformation to the geometry
        template<typename T>
        surface &operator () (const T &t) {
            geometry()(t);
            return *this;
        }

        /// Calculate the intersection of the ray on the instance
        template<typename R, typename E>
        fostlib::nullable< intersection_type > intersects(
            const R &by, const E epsilon
        ) const {
            fostlib::nullable< typename O::intersection_type >
                hit(geometry().intersects(by, epsilon));
            if ( hit ) {
                return intersection_type(hit.value(), surface_parameters());
            } else {
                return fostlib::null;
            }
        }

        /// Calculate whether this object occludes the ray or not
        template< typename R >
        bool occludes(const R &by, const local_coord_type epsilon) const {
            return geometry().occludes(by, epsilon);
        }
    };


    /// Specialisation of the surface interaction that will use all of the surface layers
    template<typename C, typename O, typename RI, typename RL,
        typename G, typename... S>
    struct surface_interaction<C, intersection<surface<O, S...>>, RI, RL, G> {
        surface_interaction() {}
        C operator() (
            const RI &observer, const RL &light,
            const intersection<surface<O, S...>> &intersection,
            const C &incident,
            const G &scene
        ) const {
            return std::apply([&](auto... pair) {
                return (pair.first(pair.second, observer, light, intersection, incident, scene) + ...);
            }, zip(std::tuple<S...>{}, intersection.parameters()));
        }
    };


    /// Specialisation of the surface emjssion that will use all of the surface layers
    template<typename C, typename O, typename RI, typename G,
        typename... S>
    struct surface_emission<C, RI, intersection<surface<O, S...>>, G> {
        surface_emission() {}
        C operator() (
            const RI &observer,
            const intersection<surface<O, S...>> &intersection,
            const G &scene
        ) const {
            return std::apply([&](auto... pair) {
                return (pair.first(pair.second, C{}, observer, intersection, scene) + ...);
            }, zip(std::tuple<S...>{}, intersection.parameters()));
        }
    };


}


#endif // ANIMRAY_SURFACE_HPP
