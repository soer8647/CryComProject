#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
using namespace CryptoPP;
typedef ECP::Point Point;

class Sender {
private:
  byte* m0;
  byte* m1;
  ECP ec;
  Point g;
  Integer a;
  Point A;
  SHA3* sha3;

public:
  Sender(byte* message0 , byte* message1, ECP curve, Point base);
  Point choose();
  std::pair<byte*,byte*> retrieve(Point B);
};

