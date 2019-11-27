#include "ExtensionSender.hpp"
#include "ExtensionReceiver.hpp"
#include "sender.hpp"
#include "receiver.hpp"
#include "utility.hpp"
#include "ExtensionProtocol.hpp"

std::vector<std::vector<byte>> OT_Protocol(std::vector<int> s , std::vector<std::vector<std::vector<byte>>> seeds, int msg_size, ECP ec, Point basePoint)
{
  int m = seeds.size();
  Sender* sender = new Sender(seeds, msg_size, ec, basePoint, 2 , m);
  Receiver* receiver = new Receiver(s, ec, basePoint, msg_size, m);

  Point S = sender->choose();
  std::vector<Point> R_lst = receiver->receive(S);
  std::vector<std::vector<byte*>> ciphers = sender->retrieve(R_lst);
  std::vector<byte*> clear_texts = receiver->compute(ciphers);

  std::vector<std::vector<byte>> output;
  rep(i,0,clear_texts.size()) {
    std::vector<byte> mi (clear_texts[i], msg_size + clear_texts[i]);
    output.push_back(mi);
  }
  return output;
}

std::vector<std::vector<byte>> OT_extension(ExtensionSender* S, ExtensionReceiver* R, int msg_size, ECP ec, Point basePoint, int n, int m) {
  // OT Phase:
  std::vector<int> s = S->basePhase();
  std::vector<std::vector<std::vector<byte>>> seeds = R->basePhase();
  std::vector<std::vector<byte>> chosenSeeds = OT_Protocol(s,seeds, msg_size, ec, basePoint);
  std::vector<std::vector<byte>> u = R->extensionPhase1();
  std::vector<std::vector<std::vector<byte>>> y = S->extensionPhase2(u,chosenSeeds);
  std::vector<std::vector<byte>> chosen_x = R->extensionPhase3(y);
  return chosen_x;
}
