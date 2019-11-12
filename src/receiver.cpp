#include "receiver.hpp"
#include "utility.hpp"

byte* decrypt(byte* cip, byte* key, int size_m, int size_k) {
  byte* m = new byte[size_m];
  for(int i=0; i<size_m; i++) {
    m[i] = cip[i]^key[i];
  }
  for(int i=size_m; i<size_k; i++) {
    if(cip[i] != key[i]) {
      std::cout << "Error! invalid decryption " << cip[i] << "!=" << key[i] << std::endl;
      return NULL;
    }
  }
  return m;
}

Receiver::Receiver(int choice, ECP curve, Point base, int size_msg) {
  c = choice;
  ec = curve;
  g = base;
  sha3 = new SHA3_256();
  size_m = size_msg;
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
  key = H(ec, A, B, ec.Multiply(b,A), sha3);
  return B;
}

byte* Receiver::compute(byte** ciphertexts_p) {
  return decrypt(*(ciphertexts_p+c), key, size_m, ec.EncodedPointSize());
}
