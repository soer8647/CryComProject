#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
using namespace CryptoPP;
typedef ECP::Point Point;

class Sender {
private:
  int m0,m1;
  ECP ec;
  Point g;
  Integer a;
  Point A;
  SHA3* sha3;

public:
  Sender(int message0 , int message1, ECP curve, Point base);
  Point choose();
  std::pair<int,int> retrieve(Point B);
};

