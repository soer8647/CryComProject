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
  int* c_lst_p;
  ECP ec;
  Point g;
  byte** keys_p;
  SHA3* sha3;
  int size_m;
  int m;

public:
  Receiver(int* c_lst_p, ECP curve, Point base, int size_m, int m);
  Point* receive(Point A);
  byte* compute(byte*** rounds_p);
};

#endif
