#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "../src/utility.hpp"
#include <cryptopp/integer.h>
#include <cryptopp/osrng.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/hrtimer.h>
#include <cryptopp/eccrypto.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cryptopp/ecp.h>
#include <cryptopp/sha3.h>
using namespace CryptoPP;
typedef ECP::Point Point;

TEST_CASE( "Test hash correct", "[Hash]" ) {
  Integer p("0xDB7C2ABF62E35E668076BEAD208B");
  Integer a("0xDB7C2ABF62E35E668076BEAD2088");
  Integer b("0x659EF8BA043916EEDE8911702B22");
  Integer x("0x09487239995A5EE76B55F9C2F098");
  Integer y("0xA89CE5AF8724C0A23E0E0FF77500");

  ECP ec = ECP(p,a,b);
  Point g = Point(x,y); // Maybe Wrong

  int length = 4096;
  AutoSeededRandomPool prng;
  a.Randomize(prng, length);
  Point p1 = ec.Multiply(a,g);
  a.Randomize(prng, length);
  Point p2 = ec.Multiply(a,g);
  a.Randomize(prng, length);
  Point p3 = ec.Multiply(a,g);

  SHA3* sha = new SHA3_256();
  SHA3* sha_ = new SHA3_256();

  REQUIRE( H(ec, p1, p2, p3, sha) == H(ec, p1, p2, p3, sha_) );
}
