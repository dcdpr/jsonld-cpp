#include "Permutator.h"

#include <utility>
#include <algorithm>
#include <iostream>

using std::vector;
using std::map;
using std::string;

Permutator::Permutator(vector<string> instrings)
        : strings(std::move(instrings)), done(false)
{
    std::sort(strings.begin(), strings.end());
    for(const auto& s : strings) {
        left[s] = true;
    }
}

bool Permutator::hasNext() {
    return !done;
}

vector<string> Permutator::next() {
    vector<string> rval = strings;

    // Calculate the next permutation using Steinhaus-Johnson-Trotter
    // permutation algoritm

    // get largest mobile element k
    // (mobile: element is greater than the one it is looking at)

    string k;
    string *kp = nullptr;
    string::size_type pos = 0;
    string::size_type length = strings.size();
    for (string::size_type i = 0; i < length; ++i) {
        string element = strings[i];
        bool isLeft = left[element];
        if ((kp == nullptr || element > *kp)
            && ((isLeft && i > 0 && element > strings[i-1])
                || (!isLeft && i < (length - 1) && element > strings[i+1]))) {
            k = element; kp = &k;
            pos = i;
        }
    }

    // no more permutations
    if (kp == nullptr) {
        done = true;
    } else {
        // swap k and the element it is looking at
        string::size_type swap;
        if(left[*kp])
            swap = pos - 1;
        else
            swap = pos + 1;
        strings[pos] = strings[swap];
        strings[swap] = k;

        // reverse the direction of all elements larger than k
        for (string::size_type i = 0; i < length; i++) {
            if (strings[i] > k) {
                left[strings[i]] = !left[strings[i]];
            }
        }
    }

    std::cout << "returning from permutator::next(): " << std::endl;
    for(auto s: rval)
        std::cout << s << " ";
    std::cout <<  std::endl;
    return rval;

}
