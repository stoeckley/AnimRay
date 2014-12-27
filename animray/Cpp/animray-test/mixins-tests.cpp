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


#include <animray/mixins/depth-count.hpp>
#include <animray/mixins/time.hpp>
#include <fost/test>


FSL_TEST_SUITE(mixins);


FSL_TEST_FUNCTION(depth_count) {
    class base {};
    static_assert(
        not std::is_same<animray::with_depth_count<base>::type, base>::value,
        "We should get different types when we add in the depth count");
    typedef animray::with_depth_count<base>::type with_count;
    with_count counted;
    FSL_CHECK_EQ(counted.depth_count(), 1u);
    static_assert(
        std::is_same<with_count, animray::with_depth_count<with_count>::type>::value,
        "The depth count is already added, so expect the same type");
    animray::with_depth_count<with_count>::type two(counted);
    FSL_CHECK_EQ(two.depth_count(), 2u);
}


FSL_TEST_FUNCTION(time) {
    class base {};
    static_assert(
        not std::is_same<animray::with_time<base>::type, base>::value,
        "We should get different types when we add in the time information");
    typedef animray::with_time<base>::type with_time;
    with_time at_time;
    FSL_CHECK_EQ(at_time.time(), fostlib::timestamp(2010, 10, 10, 10));
    static_assert(
        std::is_same<with_time, animray::with_time<with_time>::type>::value,
        "The time is already added, so expect the same type");
}
