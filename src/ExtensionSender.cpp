#include "ExtensionSender.hpp"
#include "utility.hpp"
#include <bits/stdc++.h>

ExtensionSender::ExtensionSender(std::vector<std::vector<std::vector<byte>>> messages, int m, int k_, int size_msg)
{
  x = messages;
  n = m;
  k = k_;
  size_m = size_msg;
  sha3 = new SHA3_256();
}

std::vector<int> ExtensionSender::basePhase() {
  Integer b;
  int length = 1;
  AutoSeededRandomPool prng;
  rep(i,0,k) {
    b.Randomize(prng, length);
    s.push_back(b.ConvertToLong());
  }
  return s;
}

std::vector<std::vector<std::vector<byte>>> ExtensionSender::extensionPhase2(std::vector<std::vector<byte>> u, std::vector<std::vector<byte>> key) {
  std::vector<std::vector<byte>> q;
  rep(i,0,k) {
    std::vector<byte> qi;
    if (s[i]) {
      qi = byte_xor(u[i] , G(key[i], n));
    } else {
      qi = G(key[i], n);
    }
    q.push_back(qi);
  }

  std::vector<std::vector<byte>> q_;
  q_ = transpose(q);

  std::vector<std::vector<std::vector<byte>>> ys;
  rep(i,0,n) {
    std::vector<byte> H_res = H_extension(i,q_[i], size_m, sha3);
    std::vector<byte> y0 = byte_xor(x[i][0] , H_res);
    std::vector<byte> tmp = bit_xor(q_[i], s);
    std::vector<byte> y1 = byte_xor(x[i][1] , H_extension(i,tmp, size_m, sha3));
    ys.push_back(std::vector<std::vector<byte>>{y0,y1});
  }
  return ys;
}
