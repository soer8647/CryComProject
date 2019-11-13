#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "cryptopp/nbtheory.h"
#include "cryptopp/hrtimer.h"
#include <cryptopp/ecp.h>
#include "sender.hpp"
#include "receiver.hpp"
#include <iostream>
#include <chrono>
#include <string>
#include <vector>

std::vector<std::vector<byte*>> stringsToByte(std::vector<std::vector<std::string>> msgs, int m, int n) {
  int size_m_max = 0;
  std::vector<std::vector<byte*>> bytes;
  for(int r=0; r<m; r++) {
    std::vector<byte*> round;
    for(int i=0; i<n; i++) {
      std::string m = msgs[r][i];
      int size_m = m.length();
      byte* mbyte = new byte[size_m];

      for(int j=0; j<size_m; j++) {
        mbyte[j] = m.at(j);
      }

      if(size_m_max < size_m) {
        size_m_max = size_m;
      }

      round.push_back(mbyte);
    }
    bytes.push_back(round);
  }

  return bytes;
}

void printResult(byte* m_c, auto t1, auto t2, int size_m) {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "output: '";
    for(int i=0; i<size_m; i++) {
      std::cout << m_c[i];
    }
    std::cout << "' in time: " << duration << std::endl;
}

int main(int argc, char* argv[]) {
    using namespace CryptoPP;

    Integer p("0xDB7C2ABF62E35E668076BEAD208B");
    Integer a("0xDB7C2ABF62E35E668076BEAD2088");
    Integer b("0x659EF8BA043916EEDE8911702B22");
    Integer x("0x09487239995A5EE76B55F9C2F098");
    Integer y("0xA89CE5AF8724C0A23E0E0FF77500");

    int c_lst[] = {0, 1, 2, 3, 4};
    std::vector<std::vector<std::string>> msgs = {{"besked a0", "besked a1", "besked a2", "besked a3", "besked a4"},
                                                  {"besked b0", "besked b1", "besked b2", "besked b3", "besked b4"},
                                                  {"besked c0", "besked c1", "besked c2", "besked c3", "besked c4"}};

    auto t1 = std::chrono::high_resolution_clock::now();
    static int m = 3; //TODO
    static int n = 5; //TODO
    int size_m = 10; //TODO

    std::vector<std::vector<byte*>> bytes = stringsToByte(msgs, m, n); //TODO

    ECP ec = ECP(p,a,b);
    Point basePoint = Point(x,y);
    Sender* sender = new Sender(bytes, size_m, ec, basePoint, n, m);
    Receiver* receiver = new Receiver(c_lst, ec, basePoint, size_m, m);

    if (!ec.VerifyPoint(basePoint)) {
      std::cout << "bad" << std::endl;
    }

    Point S = sender->choose();
    Point* R_lst_p = receiver->receive(S);
    byte*** rounds_p = sender->retrieve(R_lst_p);
    byte* m_c = receiver->compute(rounds_p);
    auto t2 = std::chrono::high_resolution_clock::now();
    printResult(m_c, t1, t2, size_m);

    return 0;
}
