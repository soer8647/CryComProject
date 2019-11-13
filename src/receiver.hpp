#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/hrtimer.h>
#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
#include <iostream>
#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
using namespace CryptoPP;
typedef ECP::Point Point;

#ifndef INCLUDED_receiver
#define INCLUDED_receiver
class Receiver {
private:
  bool c;
  ECP ec;
  Point g;
  byte* key;
  SHA3* sha3;
  int size_m;

public:
  Receiver(bool c, ECP curve, Point base, int size_m);
  Point receive(Point A);
  byte* compute(std::pair<byte*,byte*> ciphertexts);
};

#endif
