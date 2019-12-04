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

#ifndef INCLUDED_runOT
#define INCLUDED_runOT

std::vector<int> createChoices(int size);
std::vector<std::vector<std::vector<byte>>> createMessages(int m, int n);
ECP getEC();
Point getBasePoint();
std::vector<std::vector<byte>> perform_OT_extension(std::vector<int> choices, std::vector<std::vector<std::vector<byte>>> messages, ECP ec, Point basePoint, int rounds, int k);
std::vector<std::vector<byte>> perform_OT(std::vector<int> choices, std::vector<std::vector<std::vector<byte>>> messages, ECP ec, Point basePoint, int rounds,int n);
void printResult(std::vector<std::vector<byte>> clear_texts, long duration, int size_m, int m);

#endif
