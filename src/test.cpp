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
#include "timeTest.hpp"
#include "runOT.hpp"

using namespace CryptoPP;

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
  timeTestTransposes(3,5,17);
  //timeTestTransposes(3,5,16,false,128);
  //timeTestTransposes(3,5,16,false,256);
  //timeTestOT(2,1,4);
  //runAndPrintOT();
  return 0;
}
