#include "ReportWriter.h"

ReportWriter::ReportWriter(std::ostream& os) : out{os}{}

/**
 * \brief Handles writing of the report to an output stream
 *
 * \details Abstracts away the details of outputting the Report
 */
void ReportWriter::write( const std::string& s )
{
    out << s << std::endl;
}
