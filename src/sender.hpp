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

class Sender {
private:
  byte** msgs;
  ECP ec;
  Point g;
  Integer a;
  Point A;
  SHA3* sha3;
  int size_m;
  int n;

public:
  Sender(byte* msages[], int size_m, ECP curve, Point base, int nr);
  Point choose();
  byte** retrieve(Point B);
};

