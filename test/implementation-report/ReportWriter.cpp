#include "ReportWriter.h"

ReportWriter::ReportWriter(std::ostream& os) : out{os}{}

void ReportWriter::write( const std::string& s )
{
    out << s << std::endl;
}
