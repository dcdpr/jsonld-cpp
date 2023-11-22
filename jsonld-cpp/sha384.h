/*
    sha384.h - header of

    ============
    SHA-384 in C++
    ============

    Original Code
        -- Copyright (C) 2005, 2007 Olivier Gay <olivier.gay@a3.epfl.ch>

    Updated to C++, zedwood.com 2012

    Changed some names and added some extra functionality to
    more closely match how Java's MessageDigest works
        -- Dan Pape <dpape@dpape.com>, Nov 2023
*/
#ifndef LIBJSONLD_CPP_SHA384_H
#define LIBJSONLD_CPP_SHA384_H

#include <string>
#include <vector>

class SHA384
{
protected:
    typedef unsigned char uint8;
    typedef unsigned int uint32;
    typedef unsigned long long uint64;

    const static uint64 sha512_k[];
    static const std::size_t SHA384_512_BLOCK_SIZE = (1024/8);
    static const unsigned int DIGEST_SIZE = (384/8);

public:
    SHA384();

    void init();
    void update(const unsigned char *message, std::size_t len);
    void update(const std::string & s);
    std::string final();
    std::string digest();

protected:
    void transform(const unsigned char *message, std::size_t block_nb);
    std::size_t m_tot_len;
    std::size_t m_len;
    uint8 m_block[2*SHA384_512_BLOCK_SIZE];
    uint64 m_h[8];
};

/**
 * computes hash of input string. returns hex encoded string of the hash.
 * @param input the input string
 * @return the hex encoded string of the hash
 */
std::string sha384(const std::string & input);

/**
 * computes hash of all input strings. returns hex encoded string of the hash.
 * @param input vector of input strings
 * @return the hex encoded string of the hash
 */
std::string sha384(const std::vector<std::string> & input);


#endif //LIBJSONLD_CPP_SHA384_H
