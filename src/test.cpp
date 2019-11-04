#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "cryptopp/nbtheory.h"
#include "cryptopp/hrtimer.h"
#include <iostream>

int main(int argc, char* argv[])
{
    using namespace CryptoPP;

    ThreadUserTimer timer;
    AutoSeededRandomPool prng;
    Integer x;

    timer.StartTimer();

    do {
        x.Randomize(prng, 4096);
    } while(!IsPrime(x));

    double t = timer.ElapsedTimeAsDouble();
    std::cout << "Time: " << t << " seconds" << std::endl;
    std::cout << std::hex << x << std::endl;

    return 0;
}
