#include "ExtensionSender.hpp"
#include "ExtensionReceiver.hpp"
#include "sender.hpp"
#include "receiver.hpp"


std::vector<std::vector<byte>> OT_Protocol(std::vector<bool> s , std::vector<std::pair<std::vector<bool>,std::vector<bool>>> seeds)
{
  std::vector<std::vector<byte>> m;
  return m;
}

void f(ExtensionSender S, ExtensionReceiver R) {
  // OT Phase:
  std::vector<bool> s = S.basePhase();
  std::vector<std::pair<std::vector<bool>,std::vector<bool>>> seeds = R.basePhase();
  std::vector<std::vector<byte>> chosenSeeds = OT_Protocol(s,seeds);
  std::vector<std::vector<byte>> u = R.extensionPhase1();
  std::vector<std::pair<std::vector<byte>,std::vector<byte>>> y = S.extensionPhase2(u,chosenSeeds);
  std::vector<std::vector<byte>> chosen_x = R.extensionPhase3(y);
  
}
