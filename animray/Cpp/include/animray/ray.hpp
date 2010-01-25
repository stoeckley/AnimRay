/*
    Copyright 1995-2010, Kirit Saelensminde.
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


#ifndef ANIMRAY_RAY_HPP
#define ANIMRAY_RAY_HPP
#pragma once


#include <animray/unit-vector.hpp>


namespace animray {


    /// Represents a ray starting at a point and going to infinity in a direction
    /// in 3D space
    template< typename D >
    class ray {
        public:
            /// The value type of the line
            typedef D value_type;
            /// The type of the end points
            typedef point3d< value_type > end_type;
            typedef unit_vector< value_type > direction_type;

            /// Construct a null line
            ray() {
            }
            /// Construct a line between two locations
            ray( const end_type &from, const end_type &to )
            : from( from ), direction( to - from ) {
            }

            /// The start of the ray
            fostlib::accessors< end_type > from;
            /// A unit direction vector
            fostlib::accessors< direction_type > direction;

            /// Compare for equality
            bool operator == ( const ray &r ) const {
                return direction() == r.direction() && from() == r.from();
            }
            /// Compare for inequality
            bool operator != ( const ray &r ) const {
                return ! ( *this == r );
            }
    };


}


#endif // ANIMRAY_RAY_HPP
