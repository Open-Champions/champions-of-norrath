#include "storage.hpp"
// Observed cutMsgHandler at 0x00486a28 in .sbss: one word. cutInitNet at 0x0018e448
// stores the id returned by the message-handler registration here.
// Inferred: the word is an integer handle, as cutInitNet's authored source already treats it.
// Unknown: other readers of the word.
extern "C" {
int cutMsgHandler = 0;
}
