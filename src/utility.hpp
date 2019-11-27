#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
#include <cryptopp/integer.h>
#include <iostream>

#ifndef INCLUDED_utility_H
#define INCLUDED_utility_H
using namespace CryptoPP;
typedef ECP::Point Point;
#define rep(i, a, b) for(int i = a; i < (b); ++i)

byte* H(ECP curve, Point seed1, Point seed2, Point p, SHA3* sha3);

std::vector<byte> byte_xor(std::vector<byte> a, std::vector<byte> b);

std::vector<byte> bit_xor(std::vector<byte> a, std::vector<int> b);

std::vector<byte> G(std::vector<byte> input, int size);

std::vector<byte> H_extension(int seed, std::vector<byte> input, int size, SHA3* sha3);

std::vector<std::vector<byte>> transpose(std::vector<std::vector<byte>> M);

std::vector<std::vector<byte>> fast_transpose(std::vector<std::vector<byte>> M);

#endif
