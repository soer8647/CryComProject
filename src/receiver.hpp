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

class Receiver {
private:
  int c;
  ECP ec;
  Point g;
  byte* key;
  SHA3* sha3;
  int size_m;

public:
  Receiver(int c, ECP curve, Point base, int size_m);
  Point receive(Point A);
  byte* compute(byte** ciphertexts);
};
