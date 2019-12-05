#include "ExtensionReceiver.hpp"
#include "utility.hpp"

ExtensionReceiver::ExtensionReceiver(std::vector<int> choices, int m, int k_, int size_msg) {
  r = choices;
  n = m;
  k = k_;
  size_m = size_msg;
  sha3 = new SHA3_256();
}

std::vector<std::vector<std::vector<byte>>> ExtensionReceiver::basePhase() {
  AutoSeededRandomPool prng;
  rep(i,0,k) {
    byte* output = new byte[k];
    prng.GenerateBlock(output,k);
    std::vector<byte> k0(output, k + output);

    prng.GenerateBlock(output,k);
    std::vector<byte> k1(output, k + output);

    std::vector<std::vector<byte>> keys_i;
    keys_i.push_back(k0);
    keys_i.push_back(k1);
    keys.push_back(keys_i);
 }
  return keys;
}

std::vector<std::vector<byte>> ExtensionReceiver::extensionPhase1() {
  Integer b;
  int length = 4096;
  AutoSeededRandomPool prng;
  std::vector<std::vector<byte>> u;
  rep(i,0,k) {
    t.push_back(G(keys[i][0] , n));
  }

  rep(i,0,k) {
    std::vector<byte> temp = byte_xor(t[i] , G(keys[i][1] , n));
    std::vector<byte> ui = bit_xor(temp, r);
    u.push_back(ui);
  }
  return u;
}

std::vector<std::vector<byte>> ExtensionReceiver::extensionPhase3(std::vector<std::vector<std::vector<byte>>> y) {
  std::vector<std::vector<byte>> t_;
  t_ = fast_transpose(t);

  std::vector<std::vector<byte>> x;
  rep(i,0,n) {
    std::vector<byte> xi;
    if (r[i]) {
      xi = byte_xor(y[i][1] ,H_extension(i,t_[i],y[i][1].size(),sha3));
    }
    else {
      xi = byte_xor(y[i][0] ,H_extension(i,t_[i],y[i][0].size(),sha3));
    }
    x.push_back(xi);
  }
  return x;
}
