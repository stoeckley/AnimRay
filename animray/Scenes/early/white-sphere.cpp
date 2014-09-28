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


#include <fost/main>
#include <fost/unicode>
#include <animray/epsilon.hpp>
#include <animray/sphere.hpp>
#include <animray/targa.hpp>


FSL_MAIN(
    "animray",
    "AnimRay. Copyright 2010-2014 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    boost::filesystem::wpath output_filename =
        fostlib::coerce< boost::filesystem::wpath >(args[1].value("out.tga"));
    int width = fostlib::coerce< int >( args[2].value("100") );
    int height = fostlib::coerce< int >( args[3].value("100") );

    typedef animray::ray<double> ray;
    animray::sphere<ray> sphere;
    typedef animray::film< animray::rgb< uint8_t > > film_type;
    film_type output(width, height,
        [=, &sphere](const film_type::size_type x, const film_type::size_type y) {
            const double limit = std::min(width, height) / 2.0;
            const double cx = (double(x) + 0.5 - width/2.0) / limit;
            const double cy = -(double(y) + 0.5 - height/2.0) / limit;
            ray r(ray::end_type(cx, cy, -10.0), ray::end_type(cx, cy, -9.0));
            fostlib::nullable<ray> intersection(sphere.intersects(r, 0.0));
            if ( !intersection.isnull() ) {
                ray light(intersection.value().from(), ray::end_type(5.0, 5.0, -5.0));
                if ( sphere.occludes(light, 1e-9) ) {
                    return animray::rgb< uint8_t >(50);
                } else {
                    const double costheta = dot(light.direction(),
                        intersection.value().direction());
                    return animray::rgb< uint8_t >(50 + 205 * costheta);
                }
            } else {
                return animray::rgb< uint8_t >(0);
            }
        });
    animray::targa(output_filename, output);

    return 0;
}
