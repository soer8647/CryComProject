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
#include "ExtensionProtocol.hpp"

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

void printResult(std::vector<byte*> clear_texts, auto t1, auto t2, int size_m, int m) {
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    std::cout << "output:" << std::endl;
    for(int j=0; j<m; j++) {
      std::cout << "round " << j << " '";
      for(int i=0; i<size_m; i++) {
        std::cout << clear_texts[j][i];
      }
      std::cout << "'" << std::endl;
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
    ECP ec = ECP(p,a,b);
    Point basePoint = Point(x,y);
    if (!ec.VerifyPoint(basePoint)) {
      std::cout << "bad" << std::endl;
    }

    int c_lst[] = {1, 0, 1};
    std::vector<std::vector<std::string>> msgs = {{"a0", "a1"},
                                                  {"b0", "b1"},
                                                  {"c0", "c1"}};

    auto t1 = std::chrono::high_resolution_clock::now();
    static int m = msgs.size();
    static int n = msgs[0].size();
    int k = 2;
    int size_message = 2;
    std::vector<std::vector<byte*>> bytes = stringsToByte(msgs, m, n);

    Sender* sender = new Sender(bytes, size_message, ec, basePoint, n, m);
    Receiver* receiver = new Receiver(c_lst, ec, basePoint, size_message, m);

    Point S = sender->choose();
    std::vector<Point> R_lst = receiver->receive(S);
    std::vector<std::vector<byte*>> ciphers = sender->retrieve(R_lst);
    std::vector<byte*> clear_texts = receiver->compute(ciphers);

    auto t2 = std::chrono::high_resolution_clock::now();
    printResult(clear_texts, t1, t2, size_message, m);

    std::vector<bool> choices;
    choices.push_back(true);
    choices.push_back(false);
    choices.push_back(true);
    std::vector<std::pair<std::vector<byte>,std::vector<byte>>> messages;
    messages.push_back(std::pair<std::vector<byte>,std::vector<byte>>(std::vector<byte> {'A' , 'A'} , std::vector<byte> {'B' , 'B'}));
    messages.push_back(std::pair<std::vector<byte>,std::vector<byte>>(std::vector<byte> {'A' , 'A'} , std::vector<byte> {'B' , 'B'}));
    messages.push_back(std::pair<std::vector<byte>,std::vector<byte>>(std::vector<byte> {'A' , 'A'} , std::vector<byte> {'B' , 'B'}));

    ExtensionSender* eSender = new ExtensionSender(messages,m,k,size_message);
    ExtensionReceiver* eReceiver = new ExtensionReceiver(choices,m,k,size_message);

    std::vector<std::vector<byte>> chosen_msg = f(eSender,eReceiver);
    std::cout << chosen_msg[0][0] << chosen_msg[1][0] << chosen_msg[2][0] << std::endl;

    return 0;
}
