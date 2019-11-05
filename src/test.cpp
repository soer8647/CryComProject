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
    ECP ec = ECP(p,a,b);
    Point basePoint = Point(a,b); // Maybe Wrong
    Sender* sender = new Sender(5,1,ec,basePoint);
    Receiver* receiver = new Receiver(true, ec, basePoint);

    Point A = sender->choose();
    Point B = receiver->receive(A);
    std::pair<int,int> ciphertexts = sender->retrieve(B);

    std::cout << "done" << std::endl;

    return 0;
}
