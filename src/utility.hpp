#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
#include <cryptopp/integer.h>

#ifndef INCLUDED_utility_H
#define INCLUDED_utility_H
using namespace CryptoPP;
typedef ECP::Point Point;

byte* H(ECP curve, Point seed1, Point seed2, Point p, SHA3* sha3);

#endif
