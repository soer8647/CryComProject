#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "cryptopp/nbtheory.h"
#include "cryptopp/hrtimer.h"
#include <cryptopp/ecp.h>
#include "sender.hpp"
#include "receiver.hpp"
#include "utility.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include<time.h>
#include <vector>
#include "ExtensionProtocol.hpp"
using namespace CryptoPP;

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


clock_t timeTranspose(std::vector<std::vector<byte>> M, int iterations) {
  clock_t allTime = 0, time;
  for(int i = 0; i < iterations; i++) {
    time = clock();
    transpose(M);
    time = clock() - time;
    allTime += time;
  }
  return allTime / iterations;
}

clock_t timefastTranspose(std::vector<std::vector<byte>> M, int iterations) {
  clock_t allTime = 0, time;
  for(int i = 0; i < iterations; i++) {
    time = clock();
    fast_transpose(M);
    time = clock() - time;
    allTime += time;
  }
  return allTime / iterations;
}

void timeTestTransposes() {
  std::string sizeString = "size, ";
  std::string normalString = "normal, ";
  std::string fastString   = "fast, ";
  int iterations = 10;

  rep(i,5,15) {
      int l = pow(2,i);
      sizeString = sizeString + std::to_string(l) + ",";
      // create matrix
      std::vector<std::vector<byte>> M;
      int msg = 65;
      rep(i,0,l) {
        std::vector<byte> Mi;
        rep(j,0,l) {
          char c = msg++;
          Mi.push_back(c);
        }
        M.push_back(Mi);
      }

      clock_t normal_time = timeTranspose(M,iterations);
      clock_t fast_time = timeTranspose(M,iterations);

      normalString = normalString + std::to_string(normal_time) + ",";
      fastString = fastString + std::to_string(fast_time) + ",";
  }
  std::ofstream outFile;
  outFile.open("transposeTest.csv");
  outFile << sizeString << std::endl;
  outFile << normalString << std::endl;
  outFile << fastString << std::endl;
}

void testFastTranspose(int l , int h) {
  // Testing fast transpose
  std::vector<std::vector<byte>> M;
  int msg = 65;
  rep(i,0,l) {
    std::vector<byte> Mi;
    rep(j,0,h) {
      char c = msg++;
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

std::vector<int> createChoices(int size) {
  // Create Choices
  std::vector<int> c_lst;
  rep(i,0,size) {
    c_lst.push_back(1);
  }
  return c_lst;
}

std::vector<std::vector<std::vector<byte>>> createMessages(int m, int n) {
  // Create Choices
  std::vector<std::vector<std::string>> msgs;
  rep(i,0,m) {
    std::vector<std::string> msgs_i;
    rep(j,0,n) {
      msgs_i.push_back(std::to_string(i % 10) + std::to_string(j % 10) + ".");
    }
    msgs.push_back(msgs_i);
  }
  std::vector<std::vector<std::vector<byte>>> bytes = stringsToByte(msgs, m, n);
  return bytes;
}

ECP getEC() {
    /**
    Integer p("0xDB7C2ABF62E35E668076BEAD208B");
    Integer a("0xDB7C2ABF62E35E668076BEAD2088");
    Integer b("0x659EF8BA043916EEDE8911702B22");
    **/

    Integer p("0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF");
    Integer a("0xFFFFFFFF00000001000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFC");
    Integer b("0x5AC635D8AA3A93E7B3EBBD55769886BC651D06B0CC53B0F63BCE3C3E27D2604B");
 
    /**
    Integer p("0x01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    Integer a("0x01FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC");
    Integer b("0x0051953EB9618E1C9A1F929A21A0B68540EEA2DA725B99B315F3B8B489918EF109E156193951EC7E937B1652C0BD3BB1BF073573DF883D2C34F1EF451FD46B503F00");
    **/

    return ECP(p,a,b);
}

Point getBasePoint() {
  /**
     Integer x("0x09487239995A5EE76B55F9C2F098");
     Integer y("0xA89CE5AF8724C0A23E0E0FF77500");
   **/
  Integer x("0x6B17D1F2E12C4247F8BCE6E563A440F277037D812DEB33A0F4A13945D898C296");
  Integer y("0x4FE342E2FE1A7F9B8EE7EB4A7C0F9E162BCE33576B315ECECBB6406837BF51F5");
  /**
     Integer x("0x00C6858E06B70404E9CD9E3ECB662395B4429C648139053FB521F828AF606B4D3DBAA14B5E77EFE75928FE1DC127A2FFA8DE3348B3C1856A429BF97E7E31C2E5BD66");
     Integer y("0x011839296A789A3BC0045C8A5FB42C7D1BD998F54449579B446817AFBD17273E662C97EE72995EF42640C550B9013FAD0761353C7086A272C24088BE94769FD16650");
   **/
  return Point(x,y);
}

std::vector<std::vector<byte>> perform_OT(std::vector<int> choices, std::vector<std::vector<std::vector<byte>>> messages, ECP ec, Point basePoint, int rounds,int n) {
  int msg_size = messages[0][0].size();
  Sender* sender = new Sender(messages, msg_size, ec, basePoint, n, rounds);
  Receiver* receiver = new Receiver(choices, ec, basePoint, msg_size, rounds);

  auto S = sender->choose();
  std::vector<Point> R_lst = receiver->receive(S);
  std::vector<std::vector<std::vector<byte>>> ciphers = sender->retrieve(R_lst);
  std::vector<std::vector<byte>> clear_texts = receiver->compute(ciphers);

  return clear_texts;
}

std::vector<std::vector<byte>> perform_OT_extension(std::vector<int> choices, std::vector<std::vector<std::vector<byte>>> messages, ECP ec, Point basePoint, int rounds, int k) {
  int msg_size = messages[0][0].size();
  ExtensionSender* eSender = new ExtensionSender(messages, rounds, k, msg_size);
  ExtensionReceiver* eReceiver = new ExtensionReceiver(choices, rounds, k, msg_size);

  std::vector<std::vector<byte>> chosen_msgs = OT_extension(eSender,eReceiver, msg_size, ec, basePoint, 2, rounds);

  return chosen_msgs;
}

clock_t timeOT(std::vector<int> choices, std::vector<std::vector<std::vector<byte>>> messages, ECP ec, Point basePoint, int rounds, int n, int iterations) {
  clock_t allTime = 0, time;
  for(int i = 0; i < iterations; i++) {
    time = clock();
    perform_OT(choices, messages, ec, basePoint, rounds,n);
    time = clock() - time;
    allTime += time;
  }
  return allTime / iterations;
}

clock_t timeExtension(std::vector<int> choices, std::vector<std::vector<std::vector<byte>>> messages, ECP ec, Point basePoint, int rounds, int k, int iterations) {
  clock_t allTime = 0, time;
  for(int i = 0; i < iterations; i++) {
    time = clock();
    perform_OT_extension(choices, messages, ec, basePoint, rounds,k);
    time = clock() - time;
    allTime += time;
  }
  return allTime / iterations;
}

void timeTestOT(int iterations, int min, int max) {
  std::string sizeString = "size, ";
  std::string normalString = "normal, ";
  std::string extension128String   = "Ex128, ";
  std::string extension256String   = "Ex256, ";

  rep(i,min,max) {
    int l = pow(2,i);
    sizeString = sizeString + std::to_string(l) + ",";
    std::cout << "doing size: " << l << std::endl;

    auto choices = createChoices(l);
    auto messages = createMessages(l,2);
    auto ec = getEC();
    auto basePoint = getBasePoint();

    clock_t n_time = timeOT(choices, messages, ec, basePoint, l, 2, iterations);
    clock_t e1_time = timeExtension(choices, messages, ec, basePoint, l, 128, iterations);
    clock_t e2_time = timeExtension(choices, messages, ec, basePoint, l, 256, iterations);

    normalString = normalString + std::to_string(n_time) + ",";
    extension128String = extension128String + std::to_string(e1_time) + ",";
    extension256String = extension256String + std::to_string(e2_time) + ",";
  }
  std::ofstream outFile;
  outFile.open("data/OTTest.csv");
  outFile << sizeString << std::endl;
  outFile << normalString << std::endl;
  outFile << extension128String << std::endl;
  outFile << extension256String << std::endl;
}

void printResult(std::vector<std::vector<byte>> clear_texts, long duration, int size_m, int m) {
  std::cout << "output:" << std::endl;
  for(int j=0; j<m; j++) {
    std::cout << "'";
    for(int i=0; i<size_m; i++) {
      std::cout << clear_texts[j][i];
    }
    std::cout << "'" << std::endl;
  }
  std::cout << " in time: " << duration << std::endl;
}

void runAndPrintOT() {
  int m = 9;

  int n = 2;
  int size_message = 3; // should match "createMessages" function
  int k = 128;

  auto choices = createChoices(m);
  auto messages = createMessages(m,n);
  auto ec = getEC();
  auto basePoint = getBasePoint();

  auto clear_texts = perform_OT(choices, messages, ec, basePoint, m,n);
  int duration = 0;
  printResult(clear_texts, duration, size_message, m);

  auto chosen_msg = perform_OT_extension(choices, messages, ec, basePoint, m,k);
  printResult(chosen_msg, duration, size_message, m);
}

int main(int argc, char* argv[]) {
  //testFastTranspose(4,15);
  //timeTestTransposes();
  timeTestOT(2,1,4);
  //runAndPrintOT();
  return 0;
}
