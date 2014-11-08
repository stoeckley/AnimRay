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


#ifndef ANIMRAY_CAMERA_PINHOLE_HPP
#define ANIMRAY_CAMERA_PINHOLE_HPP
#pragma once


#include <animray/camera/flat.hpp>


namespace animray {


    /// Standard computer graphics camera
    template< typename R,
        typename C = flat_camera< typename R::local_coord_type > >
    class pinhole_camera {
        /// The 2d camera
        C camera;
    public:
        /// The underlying local co-ordinate type
        typedef typename R::local_coord_type local_coord_type;
        /// The type of ray generated by the camera
        typedef R ray_type;
        /// The type of 2d camera used to generate initial locations
        typedef C camera_type;
        /// The type used to measure the height of the camera image
        typedef typename camera_type::extents_type extents_type;
        /// The type used to measure the physical resolution of the camera
        typedef typename camera_type::resolution_type resolution_type;

        /// Construct the camera for a particular physical size and resolution
        pinhole_camera(extents_type w, extents_type h,
                resolution_type c, resolution_type r,
                extents_type l, extents_type f= extents_type(0))
        : camera(w, h, c, r), focal_plane(f), focal_length(l) {
        }

        /// Build a ray that starts from the requested pixel
        ray_type operator() (resolution_type x, resolution_type y) const {
            const animray::point2d<extents_type> pc(camera(x, y));
            typedef typename ray_type::end_type end_type;
            return ray_type(
                end_type(extents_type(0), extents_type(0), focal_plane),
                end_type(pc.x(), pc.y(), focal_plane + focal_length));
        }

    private:
        /// The location of the focal plane for the camera
        extents_type focal_plane;
        /// The focal length
        extents_type focal_length;
    };


}


#endif // ANIMRAY_CAMERA_PINHOLE_HPP
