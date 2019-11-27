#include "receiver.hpp"
#include "utility.hpp"

std::vector<byte> decrypt(std::vector<byte> cip, byte* key, int size_m, int size_k) {
  std::vector<byte> ct;
  for(int i=0; i<size_m; i++) {
    ct.push_back(cip[i]^key[i]);
  }
  for(int i=size_m; i<size_k; i++) {
    if(cip[i] != key[i]) {
      std::cout << "Error! invalid decryption " << cip[i] << "!=" << key[i] << std::endl;
      return std::vector<byte> ();
    }
  }
  return ct;
}

Receiver::Receiver(std::vector<int> choices, ECP curve, Point base, int size_msg, int m_rounds) {
  c_lst_p = choices;
  ec = curve;
  g = base;
  sha3 = new SHA3_512();
  size_m = size_msg;
  m = m_rounds;
}

std::vector<Point> Receiver::receive(Point S) {
  if (!ec.VerifyPoint(S)) {
    std::cout << "Error! not valid point" << std::endl;
    return {};
  }

  std::vector<Integer> xs;
  std::vector<Point> R_lst;

  for(int i=0; i<m; i++) {
    Integer x;
    int length = 4096;
    AutoSeededRandomPool prng;
    x.Randomize(prng, length);
    Point R = ec.Add(ec.Multiply(c_lst_p[i],S), ec.Multiply(x,g));
    byte* key = H(ec, S, R, ec.Multiply(x,S), sha3);

    xs.push_back(x);
    key_lst.push_back(key);
    R_lst.push_back(R);
  }

  return R_lst;
}

std::vector<std::vector<byte>> Receiver::compute(std::vector<std::vector<std::vector<byte>>> ciphers) {
  std::vector<std::vector<byte>> clear_texts;
  for(int i=0; i<m; i++) {
    std::vector<byte> clear_text = decrypt(ciphers[i][c_lst_p[i]], key_lst[i], size_m, ec.EncodedPointSize());
    clear_texts.push_back(clear_text);
  }

  return clear_texts;
}
