#ifndef LIBJSONLD_CPP_REPORTWRITER_H
#define LIBJSONLD_CPP_REPORTWRITER_H

#include <ostream>
#include <string>

class ReportWriter
{
    private:
        std::ostream& out;

    public:
        ReportWriter(std::ostream&);
        void write( const std::string& );
};

#endif //LIBJSONLD_CPP_REPORTWRITER_H
