#ifndef LIBJSONLD_CPP_DOUBLEFORMATTER_H
#define LIBJSONLD_CPP_DOUBLEFORMATTER_H

#include <string>

/**
 * DoubleFormatter helps to format doubles in the way that JSON-LD expects (java and c# do
 * this natively).
 */
namespace DoubleFormatter {

    /**
     * Format doubles in the way that JSON-LD expects
     *
     * Printing a double in scientific notation with full std::numeric_limits<T>::digits10
     * precision looks a certain way when using C++ std ios, but that isn't what most other
     * JSON-LD libraries and examples seem to expect. This function will format a double as
     * a string that will match that expectation.
     *
     * Examples:
     *
     *               53000 formatted as "5.3E4" instead of "5.300000000000000E+04"
     *               0.053 formatted as "5.3E-2" instead of "5.300000000000000E-02"
     *   98340011.00004567 formatted as "9.834001100004567E7" instead of "9.834001100004567E+07"
     *
     * @param d the double to format
     * @return formatted string
     */
    std::string format(double d);

    /**
     * The JSON library we use dumps floating point numbers with no fractional part
     * such as "123.0" but certain tests don't like the ending ".0"
     *
     * @param s string possibly containing a floating point number with no fractional part
     * @return input string modified to have no ".0"s
     */
    std::string fixStringWithDoubles(const std::string & s);

}

#endif //LIBJSONLD_CPP_DOUBLEFORMATTER_H
