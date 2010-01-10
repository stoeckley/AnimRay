/*
    Copyright 2010, Kirit Saelensminde.
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


#include <animray/extents2d>
#include <fost/test>


FSL_TEST_SUITE( extents2d );


FSL_TEST_FUNCTION( extents_construct ) {
    fostlib::test::default_copy_constructable< animray::extents2d< char > >();
    fostlib::test::default_copy_constructable< animray::extents2d< int > >();
    fostlib::test::default_copy_constructable< animray::extents2d< double > >();
}


FSL_TEST_FUNCTION( intersection ) {
    animray::extents2d< char >
        e1( 0, 0, 10, 10 ),
        e2( 20, 20, 30, 30 ),
        e3( 5, 5, 15, 15 ),
        e4( 5, 5, 10, 10 )
    ;
    FSL_CHECK_EQ( e1.intersection( e1 ).value(), e1 );
    FSL_CHECK( e1.intersection( e2 ).isnull() );
    FSL_CHECK_EQ( e1.intersection( e3 ).value(), e4 );
    FSL_CHECK_EQ( e1.intersection( e3 ).value(), e4 );
}
