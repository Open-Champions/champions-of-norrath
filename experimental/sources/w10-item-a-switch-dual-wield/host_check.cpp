// Host check for inventorySwitchDualWield__FiPQ24item11description. Record words
// hold 32-bit item tokens, so the items are small integers cast to pointers.
// Not an EE plan input.
#include "switch_dual_wield.cpp"
#include <cstring>
#include <string>
unsigned char player[3 * 23984];
unsigned char animState[3 * 264];
int invMsgHandle = 41;
static std::string ev;
static bool clobber;
static unsigned& word(int p, int off) { return *reinterpret_cast<unsigned*>(player + p * 23984 + off); }
extern "C" {
void netSendMessage__FiPUci(int h, unsigned char* m, int n) {
    unsigned old; std::memcpy(&old, m + 4, 4); short who; std::memcpy(&who, m + 2, 2);
    ev += "S" + std::to_string(h) + "," + std::to_string(n) + "," + std::to_string(m[0]) + "," + std::to_string(m[1]) + "," + std::to_string(who) + "," + std::to_string(old);
    if (clobber) { word(1, 21900) = 9; word(1, 21968) = 9; }
}
void resetPlayerAnimations__Fi(int p) { ev += "R" + std::to_string(p); }
void inventorySetAnim__FiP14AnimationStatei(int p, void* s, int z) { ev += "A" + std::to_string(p) + "," + std::to_string(static_cast<unsigned char*>(s) - animState) + "," + std::to_string(z); }
}
static void* tok(unsigned v) { return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(v)); }
int main() {
    word(1, 21960) = 100; word(1, 21968) = 200; word(1, 21896) = 5; word(1, 21900) = 6;
    player[23984 + 23582] = 7;
    inventorySwitchDualWield__FiPQ24item11description(1, tok(300));
    if (!ev.empty() || word(1, 21960) != 100 || word(1, 21968) != 200) return 1;   // neither slot
    inventorySwitchDualWield__FiPQ24item11description(1, tok(100));
    if (ev != "S41,8,1,0,1,200") return 2;                                         // first slot sends
    if (word(1, 21960) != 0 || word(1, 21896) != 0 || word(1, 21900) != 0 || word(1, 21968) != 0) return 3;
    ev.clear(); word(1, 21960) = 100; word(1, 21968) = 200; word(1, 21900) = 6; clobber = true;
    inventorySwitchDualWield__FiPQ24item11description(1, tok(100));
    if (word(1, 21900) != 0 || word(1, 21968) != 0) return 4;                      // cleared after the send
    ev.clear(); clobber = false; word(1, 21960) = 100; word(1, 21968) = 200; word(1, 21900) = 6; word(1, 21896) = 5;
    inventorySwitchDualWield__FiPQ24item11description(1, tok(200));
    if (ev != "R1A1,264,0") return 5;                                              // second slot resets
    if (word(1, 21968) != 0 || word(1, 21900) != 0 || player[23984 + 23582] != 0) return 6;
    if (word(1, 21960) != 100 || word(1, 21896) != 5) return 7;                    // first slot untouched
    return 0;
}
