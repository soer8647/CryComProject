#include "utility.hpp"

byte* H(ECP curve, Point seed1, Point seed2, Point p, SHA3* sha3) {
  size_t size = curve.EncodedPointSize();
  byte* output = new byte[size];
  byte* decodedseed1 = new byte[size];
  curve.EncodePoint(decodedseed1,seed1,false);
  sha3->Update(decodedseed1,size);
  byte* decodedseed2 = new byte[size];
  curve.EncodePoint(decodedseed2,seed2,false);
  sha3->Update(decodedseed2,size);
  byte* decodedPoint = new byte[size];
  curve.EncodePoint(decodedPoint,p,false);
  sha3->Update(decodedPoint,size);
  sha3->TruncatedFinal(output,size);
  return output;
}

