/*
    sha1.cpp - source code of

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
    Changed some names and added some extra functionality to
    more closely match how Java's MessageDigest works
        -- Dan Pape <dpape@dpape.com>, Oct 2019

*/

#include "jsonld-cpp/sha1.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include <vector>

/* Help macros */
#define SHA1_ROL(value, bits) (((value) << (bits)) | (((value) & 0xffffffffu) >> (32u - (bits))))
#define SHA1_BLK(i) (block[i&15u] = SHA1_ROL(block[(i+13u)&15u] ^ block[(i+8u)&15u] ^ block[(i+2u)&15u] ^ block[i&15u],1u))

/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define SHA1_R0(v,w,x,y,z,i) z += ((w&(x^y))^y)     + block[i]    + 0x5a827999 + SHA1_ROL(v,5u); w=SHA1_ROL(w,30u);
#define SHA1_R1(v,w,x,y,z,i) z += ((w&(x^y))^y)     + SHA1_BLK(i) + 0x5a827999 + SHA1_ROL(v,5u); w=SHA1_ROL(w,30u);
#define SHA1_R2(v,w,x,y,z,i) z += (w^x^y)           + SHA1_BLK(i) + 0x6ed9eba1 + SHA1_ROL(v,5u); w=SHA1_ROL(w,30u);
#define SHA1_R3(v,w,x,y,z,i) z += (((w|x)&y)|(w&x)) + SHA1_BLK(i) + 0x8f1bbcdc + SHA1_ROL(v,5u); w=SHA1_ROL(w,30u);
#define SHA1_R4(v,w,x,y,z,i) z += (w^x^y)           + SHA1_BLK(i) + 0xca62c1d6 + SHA1_ROL(v,5u); w=SHA1_ROL(w,30u);

SHA1::SHA1()
{
    reset();
}


void SHA1::update(const std::string &s)
{
    std::istringstream is(s);
    update(is);
}


void SHA1::update(std::istream &is)
{
    std::string rest_of_buffer;
    assert(buffer.size() <= BLOCK_BYTES);
    size_t bytesToRead = BLOCK_BYTES - buffer.size();
    read(is, rest_of_buffer, bytesToRead);// run some tests and step through this
    buffer += rest_of_buffer;

    while (is)
    {
        uint32 block[BLOCK_INTS];
        buffer_to_block(buffer, block);
        transform(block);
        read(is, buffer, BLOCK_BYTES);
    }
}


/*
 * Add padding and return the message digest.
 */

std::string SHA1::final()
{
    /* Total number of hashed bits */
    uint64 total_bits = (transforms*BLOCK_BYTES + buffer.size()) * 8;

    /* Padding */
    buffer += static_cast<char>(0x80);
    size_t orig_size = buffer.size();
    while (buffer.size() < BLOCK_BYTES)
    {
        buffer += static_cast<char>(0x00);
    }

    uint32 block[BLOCK_INTS];
    buffer_to_block(buffer, block);

    if (orig_size > BLOCK_BYTES - 8)
    {
        transform(block);
        for (unsigned int i = 0; i < BLOCK_INTS - 2; i++)
        {
            block[i] = 0;
        }
    }

    /* Append total_bits, split this uint64 into two uint32 */
    block[BLOCK_INTS - 1] = total_bits;
    block[BLOCK_INTS - 2] = (total_bits >> 32u);
    transform(block);

    /* Hex std::string */
    std::ostringstream result;
    for (unsigned long i : digestbuf)
    {
        result << std::hex << std::setfill('0') << std::setw(8);
        result << (i & 0xffffffff);
    }

    /* Reset for next run */
    reset();

    return result.str();
}

void SHA1::reset()
{
    /* SHA1 initialization constants */
    digestbuf[0] = 0x67452301;
    digestbuf[1] = 0xefcdab89;
    digestbuf[2] = 0x98badcfe;
    digestbuf[3] = 0x10325476;
    digestbuf[4] = 0xc3d2e1f0;

    /* Reset counters */
    transforms = 0;
    buffer = "";
}


/*
 * Hash a single 512-bit block. This is the core of the algorithm.
 */

void SHA1::transform(uint32 block[BLOCK_BYTES])
{
    /* Copy digest[] to working vars */
    uint32 a = digestbuf[0];
    uint32 b = digestbuf[1];
    uint32 c = digestbuf[2];
    uint32 d = digestbuf[3];
    uint32 e = digestbuf[4];


    /* 4 rounds of 20 operations each. Loop unrolled. */
    SHA1_R0(a,b,c,d,e, 0u)
    SHA1_R0(e,a,b,c,d, 1u)
    SHA1_R0(d,e,a,b,c, 2u)
    SHA1_R0(c,d,e,a,b, 3u)
    SHA1_R0(b,c,d,e,a, 4u)
    SHA1_R0(a,b,c,d,e, 5u)
    SHA1_R0(e,a,b,c,d, 6u)
    SHA1_R0(d,e,a,b,c, 7u)
    SHA1_R0(c,d,e,a,b, 8u)
    SHA1_R0(b,c,d,e,a, 9u)
    SHA1_R0(a,b,c,d,e,10u)
    SHA1_R0(e,a,b,c,d,11u)
    SHA1_R0(d,e,a,b,c,12u)
    SHA1_R0(c,d,e,a,b,13u)
    SHA1_R0(b,c,d,e,a,14u)
    SHA1_R0(a,b,c,d,e,15u)
    SHA1_R1(e,a,b,c,d,16u)
    SHA1_R1(d,e,a,b,c,17u)
    SHA1_R1(c,d,e,a,b,18u)
    SHA1_R1(b,c,d,e,a,19u)
    SHA1_R2(a,b,c,d,e,20u)
    SHA1_R2(e,a,b,c,d,21u)
    SHA1_R2(d,e,a,b,c,22u)
    SHA1_R2(c,d,e,a,b,23u)
    SHA1_R2(b,c,d,e,a,24u)
    SHA1_R2(a,b,c,d,e,25u)
    SHA1_R2(e,a,b,c,d,26u)
    SHA1_R2(d,e,a,b,c,27u)
    SHA1_R2(c,d,e,a,b,28u)
    SHA1_R2(b,c,d,e,a,29u)
    SHA1_R2(a,b,c,d,e,30u)
    SHA1_R2(e,a,b,c,d,31u)
    SHA1_R2(d,e,a,b,c,32u)
    SHA1_R2(c,d,e,a,b,33u)
    SHA1_R2(b,c,d,e,a,34u)
    SHA1_R2(a,b,c,d,e,35u)
    SHA1_R2(e,a,b,c,d,36u)
    SHA1_R2(d,e,a,b,c,37u)
    SHA1_R2(c,d,e,a,b,38u)
    SHA1_R2(b,c,d,e,a,39u)
    SHA1_R3(a,b,c,d,e,40u)
    SHA1_R3(e,a,b,c,d,41u)
    SHA1_R3(d,e,a,b,c,42u)
    SHA1_R3(c,d,e,a,b,43u)
    SHA1_R3(b,c,d,e,a,44u)
    SHA1_R3(a,b,c,d,e,45u)
    SHA1_R3(e,a,b,c,d,46u)
    SHA1_R3(d,e,a,b,c,47u)
    SHA1_R3(c,d,e,a,b,48u)
    SHA1_R3(b,c,d,e,a,49u)
    SHA1_R3(a,b,c,d,e,50u)
    SHA1_R3(e,a,b,c,d,51u)
    SHA1_R3(d,e,a,b,c,52u)
    SHA1_R3(c,d,e,a,b,53u)
    SHA1_R3(b,c,d,e,a,54u)
    SHA1_R3(a,b,c,d,e,55u)
    SHA1_R3(e,a,b,c,d,56u)
    SHA1_R3(d,e,a,b,c,57u)
    SHA1_R3(c,d,e,a,b,58u)
    SHA1_R3(b,c,d,e,a,59u)
    SHA1_R4(a,b,c,d,e,60u)
    SHA1_R4(e,a,b,c,d,61u)
    SHA1_R4(d,e,a,b,c,62u)
    SHA1_R4(c,d,e,a,b,63u)
    SHA1_R4(b,c,d,e,a,64u)
    SHA1_R4(a,b,c,d,e,65u)
    SHA1_R4(e,a,b,c,d,66u)
    SHA1_R4(d,e,a,b,c,67u)
    SHA1_R4(c,d,e,a,b,68u)
    SHA1_R4(b,c,d,e,a,69u)
    SHA1_R4(a,b,c,d,e,70u)
    SHA1_R4(e,a,b,c,d,71u)
    SHA1_R4(d,e,a,b,c,72u)
    SHA1_R4(c,d,e,a,b,73u)
    SHA1_R4(b,c,d,e,a,74u)
    SHA1_R4(a,b,c,d,e,75u)
    SHA1_R4(e,a,b,c,d,76u)
    SHA1_R4(d,e,a,b,c,77u)
    SHA1_R4(c,d,e,a,b,78u)
    SHA1_R4(b,c,d,e,a,79u)

    /* Add the working vars back into digest[] */
    digestbuf[0] += a;
    digestbuf[1] += b;
    digestbuf[2] += c;
    digestbuf[3] += d;
    digestbuf[4] += e;

    /* Count the number of transformations */
    transforms++;
}


void SHA1::buffer_to_block(const std::string &buffer, uint32 block[BLOCK_BYTES])
{
    /* Convert the std::string (byte buffer) to a uint32 array (MSB) */
    for (unsigned int i = 0; i < BLOCK_INTS; i++)
    {
        block[i] = (static_cast<unsigned int>(buffer[4*i+3]) & 0xffu)
                   | (static_cast<unsigned int>(buffer[4*i+2]) & 0xffu)<<8u
                   | (static_cast<unsigned int>(buffer[4*i+1]) & 0xffu)<<16u
                   | (static_cast<unsigned int>(buffer[4*i+0]) & 0xffu)<<24u;
    }
}


void SHA1::read(std::istream &is, std::string &s, size_t max)
{
    std::vector<char> sbuf(max);
    is.read(sbuf.data(), static_cast<std::streamsize>(max));
    s.assign(sbuf.data(), static_cast<size_t>(is.gcount()));
}

std::string SHA1::digest() {
    return final();
}


std::string sha1(const std::string &input)
{
    SHA1 checksum;
    checksum.update(input);
    return checksum.final();
}

std::string sha1(const std::vector<std::string> &input) {
    SHA1 checksum;
    for(const auto& s : input)
        checksum.update(s);
    return checksum.final();
}
