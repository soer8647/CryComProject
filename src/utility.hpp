#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
#include <cryptopp/integer.h>

#ifndef INCLUDED_utility_H
#define INCLUDED_utility_H
using namespace CryptoPP;
typedef ECP::Point Point;
#define rep(i, a, b) for(int i = a; i < (b); ++i)

byte* H(ECP curve, Point seed1, Point seed2, Point p, SHA3* sha3);

bool byte_xor(std::vector<byte> output, std::vector<byte> a, std::vector<byte> b);

bool bit_xor(std::vector<byte> output, std::vector<byte> a, std::vector<bool> b);

std::vector<byte> G(std::vector<byte> input);

std::vector<byte> G(std::vector<bool> input);

std::vector<byte> H_extension(int seed, std::vector<byte> input);

#endif
