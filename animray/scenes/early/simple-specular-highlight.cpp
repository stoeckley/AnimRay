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
#include <animray/camera/pinhole.hpp>
#include <animray/geometry/quadrics/sphere-unit-origin.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/movable.hpp>
#include <animray/intersection.hpp>
#include <animray/scene.hpp>
#include <animray/shader.hpp>
#include <animray/surface/matte.hpp>
#include <animray/surface/gloss.hpp>
#include <animray/light/ambient.hpp>
#include <animray/light/collection.hpp>
#include <animray/light/point.hpp>
#include <animray/targa.hpp>
#include <animray/affine.hpp>


FSL_MAIN(
    "animray",
    "AnimRay. Copyright 2010-2018 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    boost::filesystem::wpath output_filename =
        fostlib::coerce< boost::filesystem::wpath >(args[1].value_or("simple-specular-highlight.tga"));
    const int width = fostlib::coerce< int >( args[2].value_or("600") );
    const int height = fostlib::coerce< int >( args[3].value_or("400") );

    typedef double world;
    const world aspect = double(width) / height;
    const world fw = width > height ? aspect * 0.024 : 0.024;
    const world fh = width > height ? 0.024 : 0.024 / aspect;

    typedef animray::movable<animray::surface<
            animray::unit_sphere_at_origin< animray::ray< world > >,
            animray::gloss< world >,
            animray::matte< animray::rgb<float> >
        >> sphere_type;
    typedef animray::scene<
        animray::collection<sphere_type>,
        animray::light<
            std::tuple<
                animray::light<void, float>,
                animray::light<
                    std::vector<
                        animray::light<animray::point3d<world>, animray::rgb<float>>>,
                    animray::rgb<float>>
            >, animray::rgb<float>
        >,
        animray::rgb<float>>
            scene_type;
    scene_type scene;
    scene.background(animray::rgb<float>(10, 50, 70));

    scene.geometry().insert(
        sphere_type(100.0f, animray::rgb<float>(1.0, 1.0, 1.0))(
            animray::translate<world>(0.0, 0.0, 5.0)));
    scene.geometry().insert(
        sphere_type(200.0f, animray::rgb<float>(0, 1.0, 1.0))(
            animray::translate<world>(-1.0, -1.0, 0.0)));
    scene.geometry().insert(
        sphere_type(10.0f, animray::rgb<float>(1.0, 0.25, 0.5))(
            animray::translate<world>(1.0, -1.0, 0.0)));
    scene.geometry().insert(
        sphere_type(20.0f, animray::rgb<float>(0.25, 1.0, 0.5))(
            animray::translate<world>(-1.0, 1.0, 0.0)));
    scene.geometry().insert(
        sphere_type(50.0f, animray::rgb<float>(0.25, 0.5, 1.0))(
            animray::translate<world>(1.0, 1.0, 0.0)));

    std::get<0>(scene.light()).color(50);
    std::get<1>(scene.light()).push_back(
        animray::light<animray::point3d<world>, animray::rgb<float>>(
            animray::point3d<world>(-5.0, 5.0, -5.0),
            animray::rgb<float>(0x40, 0xa0, 0x40)));
    std::get<1>(scene.light()).push_back(
        animray::light<animray::point3d<world>, animray::rgb<float>>(
            animray::point3d<world>(-5.0, -5.0, -5.0),
            animray::rgb<float>(0xa0, 0x40, 0x40)));
    std::get<1>(scene.light()).push_back(
        animray::light<animray::point3d<world>, animray::rgb<float>>(
            animray::point3d<world>(5.0, -5.0, -5.0),
            animray::rgb<float>(0x40, 0x40, 0xa0)));

    animray::movable<
            animray::pinhole_camera<animray::ray<world>>,
            animray::ray<world>>
        camera(fw, fh, width, height, 0.05);
    camera(animray::translate<world>(0.0, 0.0, -8.5));
    typedef animray::film<animray::rgb<uint8_t>> film_type;
    film_type output(width, height,
        [&scene, &camera](const film_type::size_type x, const film_type::size_type y) {
            animray::rgb<float> photons(scene(camera, x, y));
            const float exposure = 1.4f;
            return animray::rgb<uint8_t>(
                uint8_t(photons.red() / exposure > 255 ? 255 : photons.red() / exposure),
                uint8_t(photons.green() / exposure > 255 ? 255 : photons.green() / exposure),
                uint8_t(photons.blue() / exposure > 255 ? 255 : photons.blue() / exposure));
        });
    animray::targa(output_filename, output);

    return 0;
}
