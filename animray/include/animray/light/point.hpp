/*
    Copyright 2014, Kirit Saelensminde.
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


#ifndef ANIMRAY_LIGHT_POINT_HPP
#define ANIMRAY_LIGHT_POINT_HPP
#pragma once


#include <animray/light/light.hpp>
#include <animray/shader.hpp>
#include <animray/ray.hpp>
#include <animray/epsilon.hpp>


namespace animray {


    /// Point lights
    template< typename C, typename W >
    class light<point3d<W>, C> : public light<void, C> {
        typedef light<void, C> superclass;
    public:
        /// The light geometry
        typedef point3d<W> geometry_type;

        /// The geometry of the light
        fostlib::accessors< geometry_type > geometry;

        /// Construct from a position and color
        light(const geometry_type &p, const typename superclass::color_type &c)
        : superclass(c), geometry(p) {
        }

        /// Calculate the illumination given by this light
        template< typename O, typename I, typename G >
        typename superclass::color_type operator () (
            const O &observer, const I &intersection, const G &scene
        ) const {
            O illumination(observer);
            illumination.from(intersection.from());
            illumination.to(geometry());
            if ( not scene.geometry().occludes(illumination, epsilon<I>::value ) ) {
                return shader(observer, illumination, intersection, superclass::color(), scene);
            } else {
                return typename superclass::color_type();
            }
        }
    };


}


#endif // ANIMRAY_LIGHT_POINT_HPP
