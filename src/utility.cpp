#include "utility.hpp"
#include "cryptopp/randpool.h"

byte* H(ECP curve, Point seed1, Point seed2, Point p, SHA3* sha3) {
  size_t size = curve.EncodedPointSize();
  byte* output = new byte[size];
  byte* decodedseed1 = new byte[size];
  curve.EncodePoint(decodedseed1,seed1,false);
  sha3->Update(decodedseed1,size);
  byte* decodedseed2 = new byte[size];
  curve.EncodePoint(decodedseed2,seed2,false);
  sha3->Update(decodedseed2,size);
  byte* decodedPoint = new byte[size];
  curve.EncodePoint(decodedPoint,p,false);
  sha3->Update(decodedPoint,size);
  sha3->TruncatedFinal(output,size - 1);
  output[size - 1] = '0';
  /**
  sha3->TruncatedFinal(output,32);
  rep(i,33,size) { // TODO Fix this
    output[i] = '0';
  }
  **/
  return output;
}

std::vector<byte> byte_xor(std::vector<byte> a, std::vector<byte> b) {
  std::vector<byte> output;
  for(int i=0; i<a.size(); i++) {
    output.push_back(a[i] ^ b[i]);
  }
  return output;
}

std::vector<byte> bit_xor(std::vector<byte> a, std::vector<int> b) {
  std::vector<byte> output;
  for(int i=0; i<a.size(); i++) {
    output.push_back(a[i]^b[i]);
  }
  return output;
}

std::vector<byte> G(std::vector<byte> input, int size){
  RandomPool rng;
  rng.IncorporateEntropy(&input[0], size);
  byte* output = new byte[size];
  rng.GenerateBlock(output,size);
  std::vector<byte> output_vector(output, size + output);
  return output_vector;
}

std::vector<byte> H_extension(int seed, std::vector<byte> input, int size, SHA3* sha3){
  sha3->Update(static_cast<byte*>(static_cast<void*>(&seed)),sizeof seed);
  sha3->Update(&input[0],input.size());
  byte* output = new byte[size];
  sha3->TruncatedFinal(output,size);
  std::vector<byte> output_vector(output,size + output);
  return output_vector;
}

std::vector<std::vector<byte>> transpose(std::vector<std::vector<byte>> M) {
  int m = M.size();
  int n = M[0].size();

  std::vector<std::vector<byte>> T;
  rep(j,0,n) {
    std::vector<byte> Tj;
    rep(i,0,m) {
      Tj.push_back(M[i][j]);
    }
    T.push_back(Tj);
  }
  return T;
}


int resize_matrix(std::vector<std::vector<byte>>* M_pointer) {
  int l = (*M_pointer).size();
  int h = (*M_pointer)[0].size();
  int n;
  if (l > h) {
    n = l;
  } else {
    n = h;
  }
  if (ceil(log2(n)) != floor(log2(n))) {
    n = pow(2,ceil(log2(n)));
  }
  (*M_pointer).resize(n);
  if (h < n) {
    rep(i,0,n) {
      (*M_pointer)[i].resize(n);
    }
  }
  return n;
}

void fast_transpose_aux(std::vector<std::vector<byte>>* M_pointer, int start_x, int start_y, int n) {
  if (n == 1) {
    return;
  }
  int half = n/2;
  rep(i,start_x,start_x + half) {
    rep(j,start_y,start_y + half) {
      auto temp = (*M_pointer)[i][+ j + half];
      (*M_pointer)[i][j + half] = (*M_pointer)[i + half][j];
      (*M_pointer)[i + half][j] = temp;
    }
  }
  fast_transpose_aux(M_pointer,start_x,start_y,half);
  fast_transpose_aux(M_pointer,start_x,start_y + half,half);
  fast_transpose_aux(M_pointer,start_x + half,start_y + half,half);
  fast_transpose_aux(M_pointer,start_x + half,start_y,half);
  return;
}

std::vector<std::vector<byte>> fast_transpose(std::vector<std::vector<byte>> M) {
  int k = M.size();
  int m = M[0].size();
  int i = 0;

  std::vector<std::vector<byte>> result;
  int tmp = m;
  while (tmp > 0) {
    std::vector<std::vector<byte>> subM;
    rep(q,0,k) {
      std::vector<byte>::const_iterator first = M[q].begin() + (i * k);
      std::vector<byte>::const_iterator last = M[q].begin() + ((i + 1) * k);
      std::vector<byte> subvector(first, last);
      subvector.resize(k);
      subM.push_back(subvector);
    }

    fast_transpose_aux(std::addressof(subM),0,0,k);

    rep(j,0,k) {
      result.push_back(subM[j]);
    }
    i++;
    tmp -= k;
  }
  result.resize(m);
  return result;
}
