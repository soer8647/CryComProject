#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "cryptopp/nbtheory.h"
#include "cryptopp/hrtimer.h"
#include <cryptopp/ecp.h>
#include "sender.hpp"
#include "receiver.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    using namespace CryptoPP;

    Integer p("0xDB7C2ABF62E35E668076BEAD208B");
    Integer a("0xDB7C2ABF62E35E668076BEAD2088");
    Integer b("0x659EF8BA043916EEDE8911702B22");
    Integer x("0x09487239995A5EE76B55F9C2F098");
    Integer y("0xA89CE5AF8724C0A23E0E0FF77500");
    int size_m = 1;
    byte* m_0 = new byte[size_m];
    byte* m_1 = new byte[size_m];

    m_1[0] = (byte)57;

    ECP ec = ECP(p,a,b);
    Point basePoint = Point(x,y); // Maybe Wrong
    Sender* sender = new Sender(m_0,m_1,size_m,ec,basePoint);
    Receiver* receiver = new Receiver(true, ec, basePoint, size_m);

    if (!ec.VerifyPoint(basePoint)) {
      std::cout << "bad" << std::endl;
    }

    Point A = sender->choose();
    Point B = receiver->receive(A);
    std::pair<byte*,byte*> ciphertexts = sender->retrieve(B);
    byte* m_c = receiver->compute(ciphertexts);

    std::cout << "done: " << m_c[0] << std::endl;

    return 0;
}
