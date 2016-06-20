//////////////////////////////////////////////////////////////////////////////
/// Copyright (C) 2014 Gefu Tang <tanggefu@gmail.com>. All Rights Reserved.
///
/// This file is part of LSHBOX.
///
/// LSHBOX is free software: you can redistribute it and/or modify it under
/// the terms of the GNU General Public License as published by the Free
/// Software Foundation, either version 3 of the License, or(at your option)
/// any later version.
///
/// LSHBOX is distributed in the hope that it will be useful, but WITHOUT
/// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
/// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
/// more details.
///
/// You should have received a copy of the GNU General Public License along
/// with LSHBOX. If not, see <http://www.gnu.org/licenses/>.
///
/// @version 0.1
/// @author Gefu Tang & Zhifeng Xiao
/// @date 2014.6.30
//////////////////////////////////////////////////////////////////////////////

/**
 * @file create_benchmark_filedb.cpp
 *
 * @brief Linear scan dataset and construct benchmark.
 */
#include <lshbox.h>
int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 5)
    {
        std::cerr << "Usage: ./create_benchmark_filedb data_path benchmark_file [Q = 500] [K = 50]" << std::endl;
        return -1;
    }
    unsigned K = 50, Q = 500, seed = 2;
    if (argc > 3)
    {
        Q = atoi(argv[3]);
    }
    if (argc > 4)
    {
        K = atoi(argv[4]);
    }
    lshbox::timer timer;
    std::cout << "CREATE BENCHMARK FOR DATA ..." << std::endl;
    std::string file(argv[1]);
    std::string ben_file(argv[2]);
    lshbox::FileDB<float> data(file);
    lshbox::Benchmark bench;
    bench.init(Q, K, data.getSize(), seed);
    lshbox::Metric<float> metric(data.getDim(), L2_DIST);
    timer.restart();
    for (unsigned i = 0; i != Q; ++i)
    {
        unsigned q = bench.getQuery(i);
        lshbox::Topk &topk = bench.getAnswer(i);
        std::cout << "------------------------- " << i + 1 << "/" << Q << " -------------------------" << std::endl; 
        lshbox::progress_display pd(data.getSize());
        for (unsigned j = 0; j != data.getSize(); ++j)
        {
            topk.push(j, metric.dist(&data.getIthVec(q)[0], &data.getIthVec(j)[0]));
            ++pd;
        }
        topk.genTopk();
    }
    std::cout << "MEAN QUERY TIME: " << timer.elapsed() / Q << "s." << std::endl;
    bench.save(ben_file);
}