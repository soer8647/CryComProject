#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/hrtimer.h>
#include <cryptopp/eccrypto.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
using namespace CryptoPP;
typedef ECP::Point Point;

#ifndef INCLUDED_sender
#define INCLUDED_sender
class Sender {
private:
  byte* m0;
  byte* m1;
  ECP ec;
  Point g;
  Integer a;
  Point A;
  SHA3* sha3;
  int size_m;

public:
  Sender(byte* message0 , byte* message1, int size_m, ECP curve, Point base);
  Point choose();
  std::pair<byte*,byte*> retrieve(Point B);
};

#endif
