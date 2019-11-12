#include "ExtensionSender.hpp"
#include "utility.hpp"
#include <bits/stdc++.h>

ExtensionSender::ExtensionSender(std::vector<std::pair<std::vector<byte>,std::vector<byte>>> messages, int m, int k_, int size_msg)
{
  n = m;
  k = k_;
  size_m = size_msg;
  sha3 = new SHA3_256();
}

std::vector<bool> ExtensionSender::basePhase() {
  Integer b;
  int length = 1;
  AutoSeededRandomPool prng;
  rep(i,0,k) {
    b.Randomize(prng, length);
    s.push_back(b.IsZero());
  }
  return s;
}

std::vector<std::pair<std::vector<byte>,std::vector<byte>>> ExtensionSender::extensionPhase2(std::vector<std::vector<byte>> u, std::vector<std::vector<byte>> chosenSeeds) {
  std::vector<std::vector<byte>> q;
  rep(i,0,k) {
    std::vector<byte> qi;
    if (s[i]) {
      byte_xor(qi, u[i] , G(key[i].second));
    } else {
      qi = G(key[i].first);
    }
    q.push_back(qi);
  }
  std::vector<std::vector<byte>> q_;
  // TODO define q_j

  std::vector<std::pair<std::vector<byte>,std::vector<byte>>> ys;
  rep(i,0,n) {
    std::vector<byte> y0;
    std::vector<byte> y1;
    byte_xor(y0, x[i].first , H_extension(i,q_[i]));
    std::vector<byte> tmp;
    bit_xor(tmp, q_[i], s);
    byte_xor(y1, x[i].second , H_extension(i,tmp));
    ys.push_back(std::pair<std::vector<byte>,std::vector<byte>>(y0,y1));
  }
  return ys;
}
