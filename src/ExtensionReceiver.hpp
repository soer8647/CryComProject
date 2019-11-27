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

#ifndef INCLUDED_Exten_receiver
#define INCLUDED_Exten_receiver

class ExtensionReceiver {
private:
  int n;
  int k;
  int size_m;
  bool* s;
  SHA3* sha3;
  std::vector<int> r;
  std::vector<std::vector<std::vector<byte>>> keys;
  std::vector<std::vector<byte>> t;

public:
  ExtensionReceiver(std::vector<int> choices, int m, int k_, int size_msg);
  std::vector<std::vector<std::vector<byte>>> basePhase();
  std::vector<std::vector<byte>> extensionPhase1();
  std::vector<std::vector<byte>> extensionPhase3(std::vector<std::vector<std::vector<byte>>> y);
};

#endif
