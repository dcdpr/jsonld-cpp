#ifndef LIBJSONLD_CPP_REPORTWRITER_H
#define LIBJSONLD_CPP_REPORTWRITER_H

#include <ostream>

class ReportWriter
{
    private:
        std::ostream& out;

    public:
        ReportWriter(std::ostream&);
};

#endif //LIBJSONLD_CPP_REPORTWRITER_H
