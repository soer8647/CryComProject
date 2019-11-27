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
  std::vector<int> c_lst_p;
  ECP ec;
  Point g;
  std::vector<byte*> key_lst;
  SHA3* sha3;
  int size_m;
  int m;

public:
  Receiver(std::vector<int> c_lst_p, ECP curve, Point base, int size_m, int m);
  std::vector<Point> receive(Point A);
  std::vector<std::vector<byte>> compute(std::vector<std::vector<byte*>> ciphers);
};

#endif
