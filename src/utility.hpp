#ifndef INCLUDED_utility_H
#define INCLUDED_utility_H
using namespace CryptoPP;
typedef ECP::Point Point;

byte* H(ECP curve, Point p, SHA3* sha3);

#endif
