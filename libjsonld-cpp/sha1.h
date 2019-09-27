/*
    sha1.h - header of

    ============
    SHA-1 in C++
    ============

    100% Public Domain.

    Original C Code
        -- Steve Reid <steve@edmweb.com>
    Small changes to fit into bglibs
        -- Bruce Guenter <bruce@untroubled.org>
    Translation to simpler C++ Code
        -- Volker Grabsch <vog@notjusthosting.com>
*/
#ifndef LIBJSONLD_CPP_SHA1_H
#define LIBJSONLD_CPP_SHA1_H

#include <iostream>
#include <string>
#include <vector>

class SHA1
{
public:
    SHA1();
    void update(const std::string &s);
    void update(std::istream &is);
    std::string final();
    // returns hex encoded digest string
    std::string digest();

private:
    typedef unsigned long int uint32;   /* just needs to be at least 32bit */
    typedef unsigned long long uint64;  /* just needs to be at least 64bit */

    static const unsigned int DIGEST_INTS = 5;  /* number of 32bit integers per SHA1 digest */
    static const unsigned int BLOCK_INTS = 16;  /* number of 32bit integers per SHA1 block */
    static const unsigned int BLOCK_BYTES = BLOCK_INTS * 4;

    uint32 digestbuf[DIGEST_INTS];
    std::string buffer;
    uint64 transforms;

    void reset();
    void transform(uint32 block[BLOCK_BYTES]);

    static void buffer_to_block(const std::string &buffer, uint32 block[BLOCK_BYTES]);
    static void read(std::istream &is, std::string &s, int max);
};

/**
 * computes hash of input string. returns hex encoded string of the hash.
 * @param input the input string
 * @return the hex encoded string of the hash
 */
std::string sha1(const std::string &input);

/**
 * computes hash of all input strings. returns hex encoded string of the hash.
 * @param input vector of input strings
 * @return the hex encoded string of the hash
 */
std::string sha1(const std::vector<std::string> & input);


#endif //LIBJSONLD_CPP_SHA1_H
