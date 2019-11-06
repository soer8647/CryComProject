#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/hrtimer.h>
#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
#include <iostream>
#include "receiver.hpp"
#include "utility.hpp"
using namespace CryptoPP;
typedef ECP::Point Point;


byte* decrypt(byte* c, byte* key) {
  return c;
}

Receiver::Receiver(bool choice, ECP curve, Point base) {
  c = choice;
  ec = curve;
  g = base;
  sha3 = new SHA3_256();
}

Point Receiver::receive(Point A) {
  if (!ec.VerifyPoint(A)) {
    std::cout << "Error! not valid point" << std::endl;
    return A;
  }
  Integer b;
  int length = 4096;
  AutoSeededRandomPool prng;
  b.Randomize(prng, length);
  Point B = ec.Add(ec.Multiply(c,A),ec.Multiply(b,g));
  key = H(ec, ec.Multiply(b,A) , sha3);
  return B;
}

byte* Receiver::compute(std::pair<byte*,byte*> ciphertexts) {
  byte* m = decrypt(ciphertexts.first,key);
  if (m == NULL) { //replace with proper failstate
    return decrypt(ciphertexts.second,key);
  } else {
    return m;
  }
}



