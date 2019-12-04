#include "cryptopp/integer.h"
#include "cryptopp/osrng.h"
#include "cryptopp/nbtheory.h"
#include "cryptopp/hrtimer.h"
#include <cryptopp/ecp.h>
#include "utility.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include<time.h>
#include <vector>
#include "runOT.hpp"

#ifndef INCLUDED_timeTest
#define INCLUDED_timeTest

void timeTestTransposes(int iterations, int min, int max);
void timeTestOT(int iterations, int min, int max);

#endif
