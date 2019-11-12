#include "ExtensionReceiver.hpp"
#include "utility.hpp"

ExtensionReceiver::ExtensionReceiver(std::vector<bool> choices, int m, int k_, int size_msg) {
  r = choices;
  n = m;
  k = k_;
  size_m = size_msg;
}

std::vector<std::pair<std::vector<bool>,std::vector<bool>>> ExtensionReceiver::basePhase() {
  Integer b;
  int length = 1;
  AutoSeededRandomPool prng;
  rep(i,0,k) {
    std::vector<bool> k0;
    std::vector<bool> k1;
    rep(i,0,k) {
      b.Randomize(prng, length);
      k0.push_back(b.IsZero());
      b.Randomize(prng, length);
      k1.push_back(b.IsZero());
    }
    keys.push_back(std::pair<std::vector<bool>,std::vector<bool>>(k0,k1));
 }
  return keys;
}

std::vector<std::vector<byte>> ExtensionReceiver::extensionPhase1() {
  Integer b;
  int length = 4096;
  AutoSeededRandomPool prng;
  std::vector<std::vector<byte>> u;
  rep(i,0,k) {
    t.push_back(G(keys[i].first));
  }

  rep(i,0,k) {
    std::vector<byte> ui;
    std::vector<byte> temp;
    byte_xor(temp, t[i] , G(keys[i].second));
    bit_xor(ui, temp, r);
    u.push_back(ui);
  }
  return u;
}

std::vector<std::vector<byte>> ExtensionReceiver::extensionPhase3(std::vector<std::pair<std::vector<byte>,std::vector<byte>>> y) {
  // define t_j TODO
  std::vector<std::vector<byte>> t_;

  std::vector<std::vector<byte>> x;
  rep(i,0,n) {
    std::vector<byte> xi;
    if (r[i]) {
      byte_xor(xi , y[i].second ,H_extension(i,t_[i]));
    }
    else {
      byte_xor(xi , y[i].first ,H_extension(i,t_[i]));
    }
    x.push_back(xi);
  }
  return x;
}
