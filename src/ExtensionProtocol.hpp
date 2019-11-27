#include "ExtensionSender.hpp"
#include "ExtensionReceiver.hpp"
#include "sender.hpp"
#include "receiver.hpp"
#include "utility.hpp"

std::vector<std::vector<byte>> OT_extension(ExtensionSender* S, ExtensionReceiver* R, int msg_size, ECP ec, Point basePoint, int n, int m);
