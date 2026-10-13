// Observed msg_getPokeName__8JumpGatePUsi at 0x002830d8, 472 bytes.
// msg_getPokeName(o, out, index): writes the poke text for a jump gate into the halfword string
// `out` through ustrcpy__FPUsT0 and returns 1, or returns 0 with nothing written when there is no
// poke. In order:
//  - A signed byte at o+79 that is nonzero returns 0.
//  - engineTaskActive__FPc("jumpgate") nonzero returns 0.
//  - When findTagString__10GameObjectPci(o, "script", 0) is nonzero, it is asked a second time and
//    the result goes to scriptStart__FPc; a zero result from that returns 0.
//  - When the signed byte at o+77 is zero: it is set to 1, jumpGateAddDestination__Fv runs, and
//    hudSetInfoMessage__FPUsiic is called with textGetString__FPc("JumpGateActivate"), 180,
//    playersAlive[index] and 0.
//  - When the signed byte jumpGateDestCount is 1: out gets textGetString("JumpGateVirgin") and 0 is returned.
//  - Otherwise, for each slot i from 0 below nmPlayersAlive except i == index, the player record
//    player[playersAlive[i]] (23984 bytes each) has an object pointer at +21872; if the squared
//    distance, taken in the order (dx*dx + dy*dy) + dz*dz, between the position at o+16 and the
//    position at that object+16 exceeds 9216.0, out gets textGetString("JumpGateNeedPlayers") and 0
//    is returned. When no player is that far, out gets textGetString("JumpGate") and 1 is returned.
// Unknown: what the destination count, the byte at +77 and the byte at +79 mean. Nothing here ran
// on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
int engineTaskActive__FPc(char* name);
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
int scriptStart__FPc(char* script);
void jumpGateAddDestination__Fv();
unsigned short* textGetString__FPc(char* key);
void hudSetInfoMessage__FPUsiic(unsigned short* text, int a, int b, char c);
void ustrcpy__FPUsT0(unsigned short* to, unsigned short* from);
extern signed char jumpGateDestCount;
extern int playersAlive[4];
extern int nmPlayersAlive;
extern unsigned char player[];
}

extern "C" int msg_getPokeName__8JumpGatePUsi(void* self, unsigned short* out, int index) {
    unsigned char* o = static_cast<unsigned char*>(self);
    if (static_cast<signed char>(o[79]) != 0) return 0;
    if (engineTaskActive__FPc(const_cast<char*>("jumpgate")) != 0) return 0;
    if (findTagString__10GameObjectPci(self, "script", 0) != nullptr) {
        if (scriptStart__FPc(findTagString__10GameObjectPci(self, "script", 0)) == 0) return 0;
    }
    if (static_cast<signed char>(o[77]) == 0) {
        o[77] = 1;
        jumpGateAddDestination__Fv();
        unsigned short* message = textGetString__FPc(const_cast<char*>("JumpGateActivate"));
        hudSetInfoMessage__FPUsiic(message, 180, playersAlive[index], 0);
    }
    if (jumpGateDestCount == 1) {
        ustrcpy__FPUsT0(out, textGetString__FPc(const_cast<char*>("JumpGateVirgin")));
        return 0;
    }
    const int count = nmPlayersAlive;
    for (int i = 0; i < count; ++i) {
        if (i == index) continue;
        const unsigned char* record = player + playersAlive[i] * 23984;
        const unsigned char* other = w10h::from_word(*reinterpret_cast<const unsigned*>(record + 21872));
        const float* mine = reinterpret_cast<const float*>(o + 16);
        const float* theirs = reinterpret_cast<const float*>(other + 16);
        float dx = mine[0] - theirs[0];
        float dy = mine[1] - theirs[1];
        float dz = mine[2] - theirs[2];
        float sum = dx * dx + dy * dy;
        sum = sum + dz * dz;
        if (9216.0f < sum) {
            ustrcpy__FPUsT0(out, textGetString__FPc(const_cast<char*>("JumpGateNeedPlayers")));
            return 0;
        }
    }
    ustrcpy__FPUsT0(out, textGetString__FPc(const_cast<char*>("JumpGate")));
    return 1;
}
