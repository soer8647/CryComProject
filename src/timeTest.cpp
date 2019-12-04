#include "timeTest.hpp"

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

void timeTestTransposes(int iterations, int min, int max, bool square, int h) {
  std::ofstream outFile;
  if (square) {
    outFile.open("data/transposeSquareTest.csv");
  } else {
    outFile.open("data/transpose" + std::to_string(h) + "Test.csv");
  }
  outFile << "size,normal,Eklundh" << std::endl;

  rep(i,min,max) {
      int l = pow(2,i);
      if (square) {
        h = l;
      }
      std::cout << "doing size: " << l << std::endl;
      // create matrix
      std::vector<std::vector<byte>> M;
      int msg = 65;
      rep(i,0,h) {
        std::vector<byte> Mi;
        rep(j,0,l) {
          char c = msg + (l % 24);
          Mi.push_back(c);
        }
        M.push_back(Mi);
      }

      clock_t normal_time = timeTranspose(M,iterations);
      clock_t fast_time = timeTranspose(M,iterations);

      outFile << i << "," << (normal_time / (float) l)  << "," << (fast_time / (float) l) << std::endl;
  }
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
