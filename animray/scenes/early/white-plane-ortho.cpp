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


#include <fost/main>
#include <fost/unicode>
#include <animray/ray.hpp>
#include <animray/epsilon.hpp>
#include <animray/camera/ortho.hpp>
#include <animray/geometry/planar/plane.hpp>
#include <animray/targa.hpp>


FSL_MAIN(
    "animray",
    "AnimRay. Copyright 2010-2018 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    boost::filesystem::wpath output_filename =
        fostlib::coerce< boost::filesystem::wpath >(args[1].value_or("white-plane-ortho.tga"));
    int width = fostlib::coerce< int >( args[2].value_or("1920") );
    int height = fostlib::coerce< int >( args[3].value_or("1080") );

    const double size = 20.0;
    const double aspect = double(width) / height;
    const double fw = width > height ? aspect * size : size;
    const double fh = width > height ? size : size / aspect;

    typedef animray::ray<double> ray;
    animray::plane<ray> plane;
    typedef animray::film< animray::rgb< uint8_t > > film_type;
    animray::ortho_camera<ray> camera(fw, fh, width, height, -9, 1);
    film_type output(width, height,
        [=, &plane](const film_type::size_type x, const film_type::size_type y) {
            ray r(camera(x, y));
            ray intersection(plane.intersects(r, 0.0).value());
            ray light(intersection.from(), ray::end_type(5.0, 5.0, -5.0));
            const double costheta = dot(light.direction(), intersection.direction());
            return animray::rgb< uint8_t >(50 + 205 * costheta);
        });
    animray::targa(output_filename, output);

    return 0;
}

