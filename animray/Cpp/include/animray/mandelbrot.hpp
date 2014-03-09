/*
    Copyright 2010-2014, Kirit Saelensminde.
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


#ifndef ANIMRAY_MANDELBROT_HPP
#define ANIMRAY_MANDELBROT_HPP
#pragma once


#include <fost/core>
#include <complex>


namespace animray{


    namespace mandelbrot {


        /// A film transformation functor implementing the mandelbrot
        template< typename F, typename D >
        struct transformer {
            const typename F::size_type width, height;
            const D aspect, weight;
            const D cx, cy, ox, oy, sz;
            const std::size_t bits;
            const unsigned int mask;

            transformer(
                typename F::size_type width, typename F::size_type height,
                D x, D y, D s, std::size_t bits
            ) : width(width), height(height),
                    aspect( D(width) / D(height) ),
                    weight( D(1) / std::max( width, height ) ),
                    cx( x ), cy( y ),
                    ox(x - s * ( aspect < D(1) ? aspect : D(1))),
                    oy(y - s / ( aspect > D(1) ? aspect : D(1))),
                    sz( s * D(2) ),
                    bits( bits ), mask(  ( 0x1 << bits ) - 1 ) {
            }
            typename F::color_type scale( unsigned int v ) const {
                if ( bits < 8 )
                    return v << (8-bits);
                else
                    return v >> (bits-8);
            }

            typedef typename F::color_type result_type;
            typedef typename F::extents_type::corner_type arg1_type;

            typename F::color_type operator () (
                const typename F::extents_type::corner_type &loc
            ) const {
                const D proportion_x = D( loc.x() ) * weight;
                const D proportion_y = D( loc.y() ) * weight;
                const D x = proportion_x * sz + ox;
                const D y = proportion_y * sz + oy;
                const std::complex< D > position( x, y );
                unsigned int counter = 1;
                for ( std::complex< D > current( position );
                    std::norm(current) < D(4) && counter > 0;
                    current = current * current + position
                ) counter = ( counter + 1 ) & mask;
                return scale(counter);
            }
        };


    }


}


#endif // ANIMRAY_MANDELBROT_HPP
