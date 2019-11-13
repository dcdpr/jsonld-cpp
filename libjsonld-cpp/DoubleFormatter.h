#ifndef LIBJSONLD_CPP_DOUBLEFORMATTER_H
#define LIBJSONLD_CPP_DOUBLEFORMATTER_H

#include <string>

// We need a way to format doubles in the way that JSON-LD expects (java and c# do this natively)
namespace DoubleFormatter {
    std::string format(double d);
}

#endif //LIBJSONLD_CPP_DOUBLEFORMATTER_H
