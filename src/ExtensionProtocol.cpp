#include "ExtensionSender.hpp"
#include "ExtensionReceiver.hpp"
#include "sender.hpp"
#include "receiver.hpp"
#include "utility.hpp"
#include "ExtensionProtocol.hpp"

std::vector<std::vector<byte>> OT_Protocol(std::vector<bool> s , std::vector<std::pair<std::vector<byte>,std::vector<byte>>> seeds)
{
  std::vector<std::vector<byte>> m;
  rep(i,0,s.size()) {
    if (s[i]) {
      m.push_back(seeds[i].second);
    } else {
      m.push_back(seeds[i].first);
    }
  }
  return m;
}

std::vector<std::vector<byte>> f(ExtensionSender* S, ExtensionReceiver* R) {
  // OT Phase:
  std::vector<bool> s = S->basePhase();
  std::vector<std::pair<std::vector<byte>,std::vector<byte>>> seeds = R->basePhase();
  std::vector<std::vector<byte>> chosenSeeds = OT_Protocol(s,seeds);
  std::vector<std::vector<byte>> u = R->extensionPhase1();
  std::vector<std::pair<std::vector<byte>,std::vector<byte>>> y = S->extensionPhase2(u,chosenSeeds);
  std::vector<std::vector<byte>> chosen_x = R->extensionPhase3(y);
  return chosen_x;
}
