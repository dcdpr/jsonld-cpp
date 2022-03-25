#ifndef LIBJSONLD_CPP_REPORTWRITER_H
#define LIBJSONLD_CPP_REPORTWRITER_H

#include <ostream>
#include <string>

/**
 * \brief   A class to provide an interface that we'll use to write out our
 *          implementation report
 *
 * \details I wanted a way to make sure that I wasn't tying myself doeen to a
 *          particular ostream --- either writing to the console, file or both
 *          --- so I thought it would be best to create a class that could be
 *          modified and/or extended in future and as long as it maintained the
 *          same interrface then it wouldn't impact other areas of the program.
 */
class ReportWriter
{
    private:
        std::ostream& out;

    public:
        /**
         * \brief Constructor for the ReportWriter
         *
         * \param stream A reference to an output stream that will be used to
         *        write the implementation report to
         *
         * \return An instance of ReportWriter
        */
        ReportWriter(std::ostream& stream);

        /**
         * Destructor for the ReportWriter
         */
        ~ReportWriter(){};

        /**
         * \brief method to enable writing of data to the outputstream
         *
         * \param data The data that is to be written to the outputstream
         */
        void write( const std::string& data );
};

#endif //LIBJSONLD_CPP_REPORTWRITER_H
