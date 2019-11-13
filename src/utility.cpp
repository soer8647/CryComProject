#include "utility.hpp"
#include "cryptopp/randpool.h"

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

std::vector<byte> byte_xor(std::vector<byte> a, std::vector<byte> b) {
  std::vector<byte> output;
  for(int i=0; i<a.size(); i++) {
    output.push_back(a[i] ^ b[i]);
  }
  return output;
}

std::vector<byte> bit_xor(std::vector<byte> a, std::vector<bool> b) {
  std::vector<byte> output;
  for(int i=0; i<a.size(); i++) {
    output.push_back(a[i]^b[i]);
  }
  return output;
}

std::vector<byte> G(std::vector<byte> input, int size){
  RandomPool rng;
  rng.IncorporateEntropy(&input[0], size);
  byte* output = new byte[size];
  rng.GenerateBlock(output,size);
  std::vector<byte> output_vector(output, size + output);
  return output_vector;
}

std::vector<byte> H_extension(int seed, std::vector<byte> input, int size, SHA3* sha3){
  sha3->Update(static_cast<byte*>(static_cast<void*>(&seed)),sizeof seed);
  sha3->Update(&input[0],input.size());
  byte* output = new byte[size];
  sha3->TruncatedFinal(output,size);
  std::vector<byte> output_vector(output,size + output);
  return output_vector;
}
