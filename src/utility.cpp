#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
#include <cryptopp/integer.h>
#include "utility.hpp"
using namespace CryptoPP;
typedef ECP::Point Point;

byte* H(ECP curve, Point p, SHA3* sha3) {
  // SEED IT using A and B
  size_t size = curve.EncodedPointSize();
  byte* output;
  byte* decodedPoint;
  curve.DecodePoint(p,decodedPoint,size);
  sha3->Update(decodedPoint,size);
  sha3->TruncatedFinal(output,size);
  return output;
}

