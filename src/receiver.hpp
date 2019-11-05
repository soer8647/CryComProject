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

public:
  Receiver(bool c, ECP curve, Point base);
  Point receive(Point A);
  int compute(std::pair<int,int> ciphertexts);
};
