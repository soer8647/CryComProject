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

Sender::Sender(byte* message0 , byte* message1, int size_msg, ECP curve, Point base)
{
  size_m = size_msg;
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
  byte* k0 = H(ec, A, B, ec.Multiply(a,B),sha3);
  const Point A_neg = ec.Inverse(A);
  const Point D = ec.Add(B,A_neg);
  byte* k1 = H(ec, A, B, ec.Multiply(a,D),sha3);

  byte* e0;
  byte* e1;
  e0 = encrypt(m0 , k0, size_m, ec.EncodedPointSize());
  e1 = encrypt(m1 , k1, size_m, ec.EncodedPointSize());
  std::pair<byte*,byte*> kpair (e0,e1);
  return kpair;
}
