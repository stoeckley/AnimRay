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


#ifndef ANIMRAY_RGBA_HPP
#define ANIMRAY_RGBA_HPP
#pragma once


#include <animray/detail/array_based.hpp>


namespace animray {


    /// Represents the normal 3 channel RGB colour space with alpha channel
    template< typename D >
    class rgba : private detail::array_based< D, 4 > {
        typedef detail::array_based< D, 4 > superclass;
    public:
        typedef typename superclass::value_type value_type;
        typedef typename superclass::array_type array_type;
        typedef typename
            superclass::const_value_parameter_type
            const_value_parameter_type
        ;
        static const std::size_t c_array_size = superclass::c_array_size;
        using superclass::to_json;

        /// Return the channel values
        const array_type &array() const {
            return superclass::array;
        }

        /// Compare for equality
        bool operator == ( const rgba &r ) const {
            return superclass::array == r.superclass::array;
        }
        /// Compare for inequality
        bool operator != ( const rgba &r ) const {
            return superclass::array != r.superclass::array;
        }
    };


}


#endif // ANIMRAY_RGBA_HPP
