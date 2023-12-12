#include "jsonld-cpp/Permutator.h"

#include <utility>
#include <algorithm>

using std::vector;
using std::map;
using std::string;

Permutator::Permutator(vector<string> istrings)
        : strings(std::move(istrings)), done(false)
{
    std::sort(strings.begin(), strings.end());
}

bool Permutator::hasNext() const {
    return !done;
}

vector<string> Permutator::next() {
    std::vector<std::string> result;
    std::copy(strings.begin(), strings.end(),
              std::back_inserter(result));

    done = !std::next_permutation(strings.begin(), strings.end());

    return result;
}
