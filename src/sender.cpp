#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/hrtimer.h>
#include <cryptopp/eccrypto.h>
#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include "sender.hpp"
#include "utility.hpp"
using namespace CryptoPP;
typedef ECP::Point Point;


byte* encrypt(byte* m, byte* key) {
  return m;
}

Sender::Sender(byte* message0 , byte* message1, ECP curve, Point base)
{
  g = base;
  ec = curve;
  m0 = message0;
  m1 = message1;
  sha3 = new SHA3_256();
}
Point Sender::choose()
{
  int length = 4096;
  AutoSeededRandomPool prng;
  a.Randomize(prng, length);
  A = ec.Multiply(a,g);

  return A;
}

std::pair<byte*,byte*> Sender::retrieve(Point B)
{
  byte* k0;
  byte* k1;
  k0 = H(ec,ec.Multiply(a,B),sha3);
  const Point A_neg = ec.Inverse(A);
  const Point D = ec.Add(B,A_neg);
  k1 = H(ec,ec.Multiply(a,D),sha3);

  byte* e0;
  byte* e1;
  e0 = encrypt(m0 , k0);
  e1 = encrypt(m1 , k1);
  std::pair<byte*,byte*> kpair (e0,e1);
  return kpair;
}
