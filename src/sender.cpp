#include "sender.hpp"
#include "utility.hpp"

std::vector<byte> encrypt(std::vector<byte> m, byte* key, int size_m, int size_k) {
  std::vector<byte> cip;
  for(int i=0; i<size_m; i++) {
    cip.push_back(m[i]^key[i]);
  }
  for(int i=size_m; i<size_k; i++) {
    cip.push_back(key[i]);
  }

  return cip;
}

Sender::Sender(std::vector<std::vector<std::vector<byte>>> msages, int size_msg, ECP curve, Point base, int nr, int m_rounds) {
  size_m = size_msg;
  g = base;
  ec = curve;
  msgs = msages;
  sha3 = new SHA3_512();
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

std::vector<std::vector<std::vector<byte>>> Sender::retrieve(std::vector<Point> R_lst) {
  std::vector<std::vector<std::vector<byte>>> rounds;

  for(int i=0; i<m; i++) {
    std::vector<std::vector<byte>> ciphers2;

    for(int j=0; j<n; j++) {
      const Point yR = ec.Multiply(y, R_lst[i]);
      const Point T = ec.Multiply(y, S);
      const Point jT = ec.Multiply(j, T);
      byte* kj = H(ec, S, R_lst[i], ec.Add(yR, ec.Inverse(jT)), sha3);
      std::vector<byte> e = encrypt(msgs[i][j], kj, size_m, ec.EncodedPointSize());
      ciphers2.push_back(e);
    }
    rounds.push_back(ciphers2);
  }

  return rounds;
}
