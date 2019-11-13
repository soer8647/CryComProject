#include "receiver.hpp"
#include "utility.hpp"

byte* decrypt(byte* cip, byte* key, int size_m, int size_k) {
  byte* ct = new byte[size_m];
  for(int i=0; i<size_m; i++) {
    ct[i] = cip[i]^key[i];
  }
  for(int i=size_m; i<size_k; i++) {
    if(cip[i] != key[i]) {
      std::cout << "Error! invalid decryption " << cip[i] << "!=" << key[i] << std::endl;
      return NULL;
    }
  }
  return ct;
}

Receiver::Receiver(int* choices, ECP curve, Point base, int size_msg, int m_rounds) {
  c_lst_p = choices;
  ec = curve;
  g = base;
  sha3 = new SHA3_256();
  size_m = size_msg;
  m = m_rounds;
}

Point* Receiver::receive(Point S) {
  if (!ec.VerifyPoint(S)) {
    std::cout << "Error! not valid point" << std::endl;
    return NULL;
  }

  //TODO get static value m instead of 5
  static Integer xs[3] = {};
  static Point R_lst[3] = {};
  static byte* key_lst[3] = {};

  for(int i=0; i<m; i++) {
    Integer x;
    int length = 4096;
    AutoSeededRandomPool prng;
    x.Randomize(prng, length);
    Point R = ec.Add(ec.Multiply(*(c_lst_p+0),S), ec.Multiply(x,g));
    byte* key = H(ec, S, R, ec.Multiply(x,S), sha3);

    xs[i] = x;
    R_lst[i] = R;
    key_lst[i] = key;
  }

  keys_p = key_lst;

  return R_lst;
}

byte* Receiver::compute(byte*** rounds_p) {
  int i=0;
  byte** ciphertexts_p = *(rounds_p+i);
  return decrypt(*(ciphertexts_p+*(c_lst_p+i)), *(keys_p+i), size_m, ec.EncodedPointSize());
}
