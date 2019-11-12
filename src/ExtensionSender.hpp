#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/hrtimer.h>
#include <cryptopp/eccrypto.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
using namespace CryptoPP;
typedef ECP::Point Point;

class ExtensionSender {
private:
  int n;
  int k;
  std::vector<std::pair<std::vector<byte>,std::vector<byte>>> key;
  std::vector<std::pair<std::vector<byte>,std::vector<byte>>> x;
  int size_m;
  std::vector<bool> s;
  SHA3* sha3;
public:
  ExtensionSender(std::vector<std::pair<std::vector<byte>,std::vector<byte>>> messages, int m, int k_, int size_msg);
  std::vector<bool> basePhase();
  std::vector<std::pair<std::vector<byte>,std::vector<byte>>> extensionPhase2(std::vector<std::vector<byte>> u, std::vector<std::vector<byte>> chosenSeeds);
};
