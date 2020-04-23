# include <iostream>
# include <cmath>
# include <cstdlib>
# include <ctime>
# include <limits>
# include <boost/crc.hpp>
#include <typeinfo>
# include "count_min_sketch.hpp"
using namespace std;

CountMinSketch::CountMinSketch() {}

// CountMinSketch constructor
void CountMinSketch::set(int width, int depth, int* hash_gens) {
  
    total = 0;
    w = width;
    d = depth;

    // initialize counter array of arrays, C
    C = new int* [d];
    unsigned int i, j;
    for (i = 0; i < d; i++) {
        C[i] = new int[w];
        for (j = 0; j < w; j++) {
            C[i][j] = 0;
        }
    }

    // initialize d pairwise independent hashes
    hashes = new int[d];
    for (i = 0; i < d; i++) {
        hashes[i] = hash_gens[i];
    }

    // cout << w << " " << d << endl;
}

// CountMinSkectch destructor
CountMinSketch::~CountMinSketch() {
    // cout << w << " " << d << endl;
    
    // free array of counters, C
    unsigned int i;
    for (i = 0; i < d; i++) {
        delete[] C[i];
    }
    delete[] C;

    // free array of hash values
    delete[] hashes;
}

// CountMinSketch totalcount returns the
// total count of all items in the sketch
unsigned int CountMinSketch::totalcount() {
    return total;
}

// countMinSketch update item count (int)
void CountMinSketch::update(int item, int c) {
    total = total + c;

    for (unsigned int j = 0; j < d; j++) {
        boost::crc_basic<32> result(hashes[j], 0xFFFFFFFF, 0xFFFFFFFF, true, true);
        result.process_bytes(&item, 4); // 4 bytes
        int idx = result.checksum() % w;
        // cout << idx << "\n";
        C[j][idx] = C[j][idx] + c;
    }
}

// CountMinSketch estimate item count (int)
unsigned int CountMinSketch::estimate(int item) {
    int minval = numeric_limits<int>::max();
    for (unsigned int j = 0; j < d; j++) {
        boost::crc_basic<32> result(hashes[j], 0xFFFFFFFF, 0xFFFFFFFF, true, true);
        result.process_bytes(&item, 4); // 4 bytes
        int idx = result.checksum() % w;
        minval = MIN(minval, C[j][idx]);
    }
    return minval;
}
