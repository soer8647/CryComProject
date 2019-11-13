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

Sender::Sender(byte* msages[], int size_msg, ECP curve, Point base, int nr, int m_rounds) {
  size_m = size_msg;
  g = base;
  ec = curve;
  msgs = msages;
  sha3 = new SHA3_256();
  n = nr;
  m = m_rounds;
}

Point Sender::choose() {
  int length = 4096;
  AutoSeededRandomPool prng;
  y.Randomize(prng, length);
  S = ec.Multiply(y,g);

  return S;
}

byte*** Sender::retrieve(Point* R_lst_p) {
  static byte** rounds[3]; //TODO get static value m instead of 5

  for(int i=0; i<m; i++) {
    static byte* ciphers[5]; //TODO get static value n instead of 5

    for(int j=0; j<n; j++) {
      const Point yR = ec.Multiply(y, *(R_lst_p+0));
      const Point T = ec.Multiply(y, S);
      const Point jT = ec.Multiply(j, T);
      byte* kj = H(ec, S, *(R_lst_p+0), ec.Add(yR, ec.Inverse(jT)), sha3);
      byte* e = encrypt(msgs[j], kj, size_m, ec.EncodedPointSize());
      ciphers[j] = e;
    }

    rounds[i] = ciphers;
  }

  return rounds;
}
