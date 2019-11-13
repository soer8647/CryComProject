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
#include "ExtensionProtocol.hpp"

int main(int argc, char* argv[])
{
    using namespace CryptoPP;

    Integer p("0xDB7C2ABF62E35E668076BEAD208B");
    Integer a("0xDB7C2ABF62E35E668076BEAD2088");
    Integer b("0x659EF8BA043916EEDE8911702B22");
    Integer x("0x09487239995A5EE76B55F9C2F098");
    Integer y("0xA89CE5AF8724C0A23E0E0FF77500");

    auto t1 = std::chrono::high_resolution_clock::now();
    std::string s = "Hello W0rld";
    int size_m = s.length();
    byte* m_0 = new byte[size_m];
    byte* m_1 = new byte[size_m];

    for(int i=0; i<size_m; i++) {
      m_1[i] = s.at(i);
    }

    ECP ec = ECP(p,a,b);
    Point basePoint = Point(x,y);
    Sender* sender = new Sender(m_0,m_1,size_m,ec,basePoint);
    Receiver* receiver = new Receiver(false, ec, basePoint, size_m);

    if (!ec.VerifyPoint(basePoint)) {
      std::cout << "bad" << std::endl;
    }

    Point A = sender->choose();
    Point B = receiver->receive(A);
    std::pair<byte*,byte*> ciphertexts = sender->retrieve(B);
    byte* m_c = receiver->compute(ciphertexts);

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    std::cout << "output: ";
    for(int i=0; i<size_m; i++) {
      std::cout << m_c[i];
    }
    std::cout << " in time: " << duration << std::endl;

    int n = 3;
    int k = 2;
    int size_message = 2;

    std::vector<bool> choices;
    choices.push_back(true);
    choices.push_back(false);
    choices.push_back(true);

    std::vector<std::pair<std::vector<byte>,std::vector<byte>>> messages;
    messages.push_back(std::pair<std::vector<byte>,std::vector<byte>>(std::vector<byte> {'A' , 'A'} , std::vector<byte> {'B' , 'B'}));
    messages.push_back(std::pair<std::vector<byte>,std::vector<byte>>(std::vector<byte> {'A' , 'A'} , std::vector<byte> {'B' , 'B'}));
    messages.push_back(std::pair<std::vector<byte>,std::vector<byte>>(std::vector<byte> {'A' , 'A'} , std::vector<byte> {'B' , 'B'}));

    ExtensionSender* eSender = new ExtensionSender(messages,n,k,size_message);
    ExtensionReceiver* eReceiver = new ExtensionReceiver(choices,n,k,size_message);

    std::vector<std::vector<byte>> chosen_msg = f(eSender,eReceiver);
    std::cout << chosen_msg[0][0] << chosen_msg[1][0] << chosen_msg[2][0] << std::endl;
    return 0;
}
