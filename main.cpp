/**
    in memory sorting of files

    \author Steve Göring
    \contact stg7@gmx.de

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

#include <iostream>
#include <fstream>
#include <istream>
#include <vector>
#include <algorithm>

#include <iterator>

#include <boost/program_options.hpp>

#include "parallel_sort.hpp"

/**
    no longer needed, but maybe useful
**/
std::size_t count_lines(std::string filename) {
    std::fstream file(filename.c_str());

    std::size_t count = std::count_if(std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>(),
        [](char c) {return c == '\n';});
    return count;
}


/**
    simple sorter
**/
int main(int argc, const char* argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    namespace po = boost::program_options;

    // declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("file,f", po::value<std::string>(), "input file")
        ("output,o", po::value<std::string>(), "output file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") ) {
        std::cout << desc << std::endl;
        return 1;
    }

    // set output stream
    std::streambuf* obuf = std::cout.rdbuf();
    std::ofstream of;

    if (vm.count("output") != 0) {
        std::string ofile = vm["output"].as<std::string>();
        of.open(ofile.c_str());
        obuf = of.rdbuf();
    }
    std::ostream out(obuf);

    out.sync_with_stdio(false);


    // set input stream
    std::streambuf* ibuf = std::cin.rdbuf();
    std::ifstream ifn;

    if (vm.count("file") != 0) {
        std::string filename = vm["file"].as<std::string>();
        ifn.open(filename.c_str());
        ibuf = ifn.rdbuf();
    }
    std::istream infile(ibuf);

    infile.sync_with_stdio(false);

    std::string line = "";
    std::vector<std::string> lines;

    while (std::getline(infile, line)) {
        line.shrink_to_fit(); // save memory
        lines.emplace_back(line);
    }

    // start sorting
    parallel_sort(lines);

    for (std::string& s: lines) {
        out << s << "\n";
    }

    return 0;
}
