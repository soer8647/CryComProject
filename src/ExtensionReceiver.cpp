#include "ExtensionReceiver.hpp"
#include "utility.hpp"

ExtensionReceiver::ExtensionReceiver(std::vector<bool> choices, int m, int k_, int size_msg) {
  r = choices;
  n = m;
  k = k_;
  size_m = size_msg;
  sha3 = new SHA3_256();
}

std::vector<std::pair<std::vector<byte>,std::vector<byte>>> ExtensionReceiver::basePhase() {
  Integer b;
  int length = 1;
  AutoSeededRandomPool prng;
  rep(i,0,k) {
    byte* output = new byte[k];
    prng.GenerateBlock(output,k);
    std::vector<byte> k0(output, output);

    prng.GenerateBlock(output,k);
    std::vector<byte> k1(output, output);

    keys.push_back(std::pair<std::vector<byte>,std::vector<byte>>(k0,k1));
 }
  return keys;
}

std::vector<std::vector<byte>> ExtensionReceiver::extensionPhase1() {
  Integer b;
  int length = 4096;
  AutoSeededRandomPool prng;
  std::vector<std::vector<byte>> u;
  rep(i,0,k) {
    t.push_back(G(keys[i].first , n));
  }

  rep(i,0,k) {
    std::vector<byte> temp = byte_xor(t[i] , G(keys[i].second , n));
    std::vector<byte> ui = bit_xor(temp, r);
    u.push_back(ui);
  }
  return u;
}

std::vector<std::vector<byte>> ExtensionReceiver::extensionPhase3(std::vector<std::pair<std::vector<byte>,std::vector<byte>>> y) {
  std::vector<std::vector<byte>> t_(n);
  rep(j,0,n) {
    t_[j].reserve(k);
    rep(i,0,k) {
      t_[j][i] = t[i][j];
    }
  }

  std::vector<std::vector<byte>> x;
  rep(i,0,n) {
    std::vector<byte> xi;
    if (r[i]) {
      xi = byte_xor(y[i].second ,H_extension(i,t_[i],y[i].second.size(),sha3));
    }
    else {
      xi = byte_xor(y[i].first ,H_extension(i,t_[i],y[i].first.size(),sha3));
    }
    x.push_back(xi);
  }
  return x;
}
