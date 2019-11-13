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

    int c_lst[] = {0, 1, 2, 3, 4};
    std::string msgs[3][5] = {{"besked a0", "besked a1", "besked a2", "besked a3", "besked a4"},
                              {"besked b0", "besked b1", "besked b2", "besked b3", "besked b4"},
                              {"besked c0", "besked c1", "besked c2", "besked c3", "besked c4"}};

    auto t1 = std::chrono::high_resolution_clock::now();
    static int n = 5; //TODO
    static int m = 3; //TODO
    byte* bytes[n] = {};
    int size_m = stringsToByte(msgs[0], n, bytes); //TODO

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
