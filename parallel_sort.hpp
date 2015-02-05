/**
    in memory sorting of files

    \author Steve Göring
    \contact stg7@gmx.de

    parallel sort impl

    \brief modified parallel sort implementation, based on https://github.com/sol-prog/Sort_data_parallel

    description:
        https://solarianprogrammer.com/2013/02/04/sorting-data-in-parallel-cpu-gpu/

    g++ params -std=c++11 -lpthread

    \date 30.09.2014
**/
/**
    This file is part of im_sort.

    im_sort is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    im_sort is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with im_sort.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef PARALLEL_SORT_HEADER_HPP_
#define PARALLEL_SORT_HEADER_HPP_
#include <algorithm>
#include <thread>

template<typename T>
inline void parallel_sort(std::vector<T>& V, size_t parts = -1) {

    if (parts == -1) { // default case: use all aviable cpu cores
        parts = std::thread::hardware_concurrency();
    }

    // split the data in "parts" pieces and sort each piece in a separate thread
    std::vector<size_t> bnd(parts + 1);
    size_t delta = V.size() / parts;
    size_t reminder = V.size() % parts;
    size_t N1 = 0, N2 = 0;
    bnd[0] = N1;
    for (size_t i = 0; i < parts; ++i) {
        N2 = N1 + delta;
        if (i == parts - 1)
            N2 += reminder;
        bnd[i + 1] = N2;
        N1 = N2;
    }

    std::vector<std::thread> thr;
    thr.reserve(parts);

    // create threads for each part and sort parts
    for (size_t i = 0; i < parts; i++) {
        thr.emplace_back(
            std::thread(
                [](std::vector<T>& V, const size_t left, const size_t right) {
                    std::sort(std::begin(V) + left, std::begin(V) + right);
                }, std::ref(V), bnd[i], bnd[i + 1]
            ));
    }

    for (auto& t : thr) {
        t.join();
    }
    thr.clear();


    std::vector<size_t> limits;
    limits.reserve(parts);

    // merge two parts together

    while (parts >= 2) {
        for (size_t i = 0; i < parts - 1; i += 2) {
            thr.emplace_back(
                std::thread(
                    [](std::vector<T>& V, size_t n0, size_t n1, size_t n2, size_t n3) {
                        std::inplace_merge(std::begin(V) + n0, std::begin(V) + n1, std::begin(V) + n3);
                    }, std::ref(V), bnd[i], bnd[i + 1], bnd[i + 1], bnd[i + 2]));

            size_t naux = limits.size();
            if (naux > 0) {
                if (limits[naux - 1] != bnd[i]) {
                    limits.emplace_back(bnd[i]);
                }
                limits.emplace_back(bnd[i + 2]);
            }
            else {
                limits.emplace_back(bnd[i]);
                limits.emplace_back(bnd[i + 2]);
            }
        }

        for (auto& t : thr) {
            t.join();
        }

        parts /= 2;
        bnd = limits;
        limits.clear();
        thr.clear();
    }
}

#endif