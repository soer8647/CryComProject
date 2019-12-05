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

      outFile << i << "," << (( ((float) normal_time) / CLOCKS_PER_SEC) /  l)  << "," << ((((float) fast_time) / CLOCKS_PER_SEC) / l) << std::endl;
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
  std::ofstream outFile;
  outFile.open("data/OTTest.csv");
  outFile << "size,normal,Ex128,Ex256" << std::endl;

  rep(i,min,max) {
    int l = pow(2,i);
    std::cout << "doing size: " << l << std::endl;

    auto choices = createChoices(l);
    auto messages = createMessages(l,2);
    auto ec = getEC();
    auto basePoint = getBasePoint();

    clock_t n_time = timeOT(choices, messages, ec, basePoint, l, 2, iterations);
    clock_t e1_time = timeExtension(choices, messages, ec, basePoint, l, 128, iterations);
    clock_t e2_time = timeExtension(choices, messages, ec, basePoint, l, 256, iterations);

    outFile << ((((float) n_time) / CLOCKS_PER_SEC) / l) << "," << (e1_time / CLOCKS_PER_SEC) / (float l) << "," << ((((float) fast_time) / CLOCKS_PER_SEC) / l) << "," << std::endl;
  }
}
