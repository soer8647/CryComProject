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

int stringsToByte(std::string msgs[], int n, byte* bytes[]) {
  int size_m_max = 0;
  for(int i=0; i<n; i++) {
    std::string m = msgs[i];
    int size_m = m.length();
    byte* mbyte = new byte[size_m];

    for(int j=0; j<size_m; j++) {
      mbyte[j] = m.at(j);
    }

    if(size_m_max < size_m) {
      size_m_max = size_m;
    }

    bytes[i] = mbyte;
  }
  return size_m_max;
}

void printResult(byte* m_c, auto t1, auto t2, int size_m) {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    std::cout << "output: '";
    for(int i=0; i<size_m; i++) {
      std::cout << m_c[i];
    }
    std::cout << "' in time: " << duration << std::endl;
}

int main(int argc, char* argv[])
{
    using namespace CryptoPP;

    Integer p("0xDB7C2ABF62E35E668076BEAD208B");
    Integer a("0xDB7C2ABF62E35E668076BEAD2088");
    Integer b("0x659EF8BA043916EEDE8911702B22");
    Integer x("0x09487239995A5EE76B55F9C2F098");
    Integer y("0xA89CE5AF8724C0A23E0E0FF77500");

    int c = 3;
    std::string msgs[] = {"besked 0", "besked 1", "besked 2", "besked 3", "besked 4"};
    auto t1 = std::chrono::high_resolution_clock::now();
    static int n = (sizeof(msgs)/sizeof(*msgs));
    byte* bytes[n] = {};
    int size_m = stringsToByte(msgs, n, bytes);

    ECP ec = ECP(p,a,b);
    Point basePoint = Point(x,y);
    Sender* sender = new Sender(bytes, size_m, ec, basePoint, n);
    Receiver* receiver = new Receiver(c, ec, basePoint, size_m);

    if (!ec.VerifyPoint(basePoint)) {
      std::cout << "bad" << std::endl;
    }

    Point A = sender->choose();
    Point B = receiver->receive(A);
    byte** ciphertexts_p = sender->retrieve(B);
    byte* m_c = receiver->compute(ciphertexts_p);
    auto t2 = std::chrono::high_resolution_clock::now();
    printResult(m_c, t1, t2, size_m);

    return 0;
}
