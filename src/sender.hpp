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
  std::vector<std::vector<byte*>> msgs;
  ECP ec;
  Point g;
  Integer y;
  Point S;
  SHA3* sha3;
  int size_m;
  int n;
  int m;

public:
  Sender(std::vector<std::vector<byte*>> msgs, int size_m, ECP curve, Point base, int n, int m);
  Point choose();
  std::vector<std::vector<byte*>> retrieve(Point* R_lst_p);
};

