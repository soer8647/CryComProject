#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "cryptopp/nbtheory.h"
#include "cryptopp/hrtimer.h"
#include <cryptopp/ecp.h>
#include "sender.hpp"
#include "receiver.hpp"
#include "utility.hpp"
#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include "ExtensionProtocol.hpp"

std::vector<std::vector<std::vector<byte>>> stringsToByte(std::vector<std::vector<std::string>> msgs, int m, int n) {
  int size_m_max = 0;
  std::vector<std::vector<std::vector<byte>>> bytes;

  for(int r=0; r<m; r++) {
    std::vector<std::vector<byte>> round;
    for(int i=0; i<n; i++) {
      std::string m = msgs[r][i];
      int size_m = m.length();
      std::vector<byte> mbyte;

      for(int j=0; j<size_m; j++) {
        mbyte.push_back(m.at(j));
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

void testFastTranspose(int l , int h) {
  // Testing fast transpose
  std::vector<std::vector<byte>> M;
  int kkk = 65;
  rep(i,0,l) {
    std::vector<byte> Mi;
    rep(j,0,h) {
      char c = kkk++;
      Mi.push_back(c);
    }
    M.push_back(Mi);
  }
  rep(i,0,l) {
    rep(j,0,h) {
      std::cout << M[i][j];
    }
    std::cout << std::endl;
  }


  std::vector<std::vector<byte>> T;
  std::vector<std::vector<byte>> FT;

  T = transpose(M);
  FT = fast_transpose(M);

  bool allTrue = true;
  rep(i,0,h) {
    rep(j,0,l) {
      if (FT[i][j] != T[i][j]) {
        allTrue = false;
      }
    }
  }
  std::cout << allTrue << std::endl;
  if (!allTrue) {
    rep(i,0,h) {
      rep(j,0,l) {
        std::cout << FT[i][j];
      }
      std::cout << std::endl;

    }

    std::cout << std::endl;
    rep(i,0,h) {
      rep(j,0,l) {
        std::cout << T[i][j];
      }
      std::cout << std::endl;

    }
  }
}

void printResult(std::vector<std::vector<byte>> clear_texts, long duration, int size_m, int m) {

    std::cout << "output:" << std::endl;
    for(int j=0; j<0; j++) {
      for(int i=0; i<size_m; i++) {
        std::cout << clear_texts[j][i];
      }
      std::cout << "'" << std::endl;
    }
    std::cout << "' in time: " << duration << std::endl;
}

int main(int argc, char* argv[]) {
    using namespace CryptoPP;
    testFastTranspose(4,15);
    /**
    Integer p("0xDB7C2ABF62E35E668076BEAD208B");
    Integer a("0xDB7C2ABF62E35E668076BEAD2088");
    Integer b("0x659EF8BA043916EEDE8911702B22");
    Integer x("0x09487239995A5EE76B55F9C2F098");
    Integer y("0xA89CE5AF8724C0A23E0E0FF77500");
    **/

    Integer p("0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF");
    Integer a("0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC");
    Integer b("0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B");
    Integer x("0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296");
    Integer y("0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5");

    /**
    Integer p("0x01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    Integer a("0x01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC");
    Integer b("0x0051953EB9618E1C9A1F929A21A0B68540EEA2DA725B99B315F3B8B489918EF109E156193951EC7E937B1652C0BD3BB1BF073573DF883D2C34F1EF451FD46B503F00");
    Integer x("0x00C6858E06B70404E9CD9E3ECB662395B4429C648139053FB521F828AF606B4D3DBAA14B5E77EFE75928FE1DC127A2FFA8DE3348B3C1856A429BF97E7E31C2E5BD66");
    Integer y("0x011839296A789A3BC0045C8A5FB42C7D1BD998F54449579B446817AFBD17273E662C97EE72995EF42640C550B9013FAD0761353C7086A272C24088BE94769FD16650");
    **/

    ECP ec = ECP(p,a,b);
    Point basePoint = Point(x,y);
    if (!ec.VerifyPoint(basePoint)) {
      std::cout << "bad" << std::endl;
    }

    int m = 100;
    int n = 2;
    int size_message = 4;

    // Create Choices
    std::vector<int> c_lst;
    rep(i,0,m) {
      c_lst.push_back(1);
    }

    //Create Messages
    std::vector<std::vector<std::string>> msgs;
    rep(i,0,m) {
      std::vector<std::string> msgs_i;
      rep(j,0,n) {
        msgs_i.push_back(std::to_string(i)+ "," + std::to_string(j));
      }
      msgs.push_back(msgs_i);
    }


    int k = 10;
    std::vector<std::vector<std::vector<byte>>> bytes = stringsToByte(msgs, m, n);

    auto t1 = std::chrono::high_resolution_clock::now();

    Sender* sender = new Sender(bytes, size_message, ec, basePoint, n, m);
    Receiver* receiver = new Receiver(c_lst, ec, basePoint, size_message, m);

    Point S = sender->choose();
    std::vector<Point> R_lst = receiver->receive(S);
    std::vector<std::vector<std::vector<byte>>> ciphers = sender->retrieve(R_lst);
    std::vector<std::vector<byte>> clear_texts = receiver->compute(ciphers);

    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    
    printResult(clear_texts, duration, size_message, m);

    // OT extension
    t1 = std::chrono::high_resolution_clock::now();

    ExtensionSender* eSender = new ExtensionSender(bytes,m,k,size_message);
    ExtensionReceiver* eReceiver = new ExtensionReceiver(c_lst,m,k,size_message);

    std::vector<std::vector<byte>> chosen_msg = OT_extension(eSender,eReceiver, size_message, ec, basePoint, n, m);

    t2 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    printResult(chosen_msg, duration, size_message, m);


    return 0;
}
