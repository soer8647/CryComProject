#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
using namespace CryptoPP;
typedef ECP::Point Point;

class Receiver {
private:
  bool c;
  ECP ec;
  Point g;
  byte* key;
  SHA3* sha3;
  int size_m;

public:
  Receiver(bool c, ECP curve, Point base, int size_m);
  Point receive(Point A);
  byte* compute(std::pair<byte*,byte*> ciphertexts);
};
