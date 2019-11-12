#include "sender.hpp"
#include "utility.hpp"

byte* encrypt(byte* m, byte* key, int size_m, int size_k) {
  byte* cip = new byte[size_k];
  for(int i=0; i<size_m; i++) {
    cip[i] = m[i]^key[i];
  }
  for(int i=size_m; i<size_k; i++) {
    cip[i] = key[i];
  }

  return cip;
}

Sender::Sender(byte* msages[], int size_msg, ECP curve, Point base, int nr) {
  size_m = size_msg;
  g = base;
  ec = curve;
  msgs = msages;
  sha3 = new SHA3_256();
  n = nr;
}

Point Sender::choose() {
  int length = 4096;
  AutoSeededRandomPool prng;
  a.Randomize(prng, length);
  A = ec.Multiply(a,g);

  return A;
}

byte** Sender::retrieve(Point B) {
  static byte* ciphers[3]; //TODO get static value n instead

  for(int i=0; i<n; i++) {
   const Point yR = ec.Multiply(a, B);
   const Point T = ec.Multiply(a, A);
   const Point jT = ec.Multiply(i, T);
    byte* ki = H(ec, A, B, ec.Add(yR, ec.Inverse(jT)), sha3);
    byte* e = encrypt(msgs[i], ki, size_m, ec.EncodedPointSize());
    ciphers[i] = e;
  }

  return ciphers;
}
