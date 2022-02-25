#include "DoubleFormatter.h"
#include <sstream>
#include <regex>
#include <iomanip>
#include <limits>

std::string DoubleFormatter::format(double d) {
    std::stringstream ss;
    auto numDigits = std::numeric_limits<double>::digits10;
    ss << std::uppercase << std::scientific << std::setprecision(numDigits) << d;
    std::string s = ss.str();

    // todo: this is a pretty annoying back-and-forth dance with all these
    // regexes. There must be a nicer way to do this.
    std::regex preceding_zeros("(\\.[1-9]+)0+E");
    std::string s2 = std::regex_replace(s, preceding_zeros, "$1E");

    std::regex preceding_only_zeros("\\.0+E");
    s = std::regex_replace(s2, preceding_only_zeros, ".0E");

    std::regex exponent_plus_sign("(E\\+)");
    s2 = std::regex_replace(s, exponent_plus_sign, "E");

    std::regex leading_zero_from_positive_power("(E0)");
    s = std::regex_replace(s2, leading_zero_from_positive_power, "E");

    std::regex leading_zero_from_negative_power("(E-0)");
    s2 = std::regex_replace(s, leading_zero_from_negative_power, "E-");

    return s2;
}
