// Host check for netInContact. Not an EE plan input.
#include "net_in_contact.cpp"
#include <cstring>
extern "C" {
int nmPartners;
unsigned char partners[70528];
}
static unsigned char* rec(int i) { return partners + i * 17632; }
int main() {
    std::memset(partners, 0, sizeof partners);
    nmPartners = 0;
    if (netInContact__Fv() != 1) return 1;
    nmPartners = -3;
    if (netInContact__Fv() != 1) return 2;
    nmPartners = 4;
    if (netInContact__Fv() != 0) return 3;  // all words zero
    for (int i = 0; i < 4; ++i) *(int*)(rec(i) + 17616) = 5;
    if (netInContact__Fv() != 1) return 4;
    *(int*)(rec(2) + 17616) = 0;
    if (netInContact__Fv() != 0) return 5;
    nmPartners = 2;  // record 2 is out of range
    if (netInContact__Fv() != 1) return 6;
    nmPartners = 4;
    for (int off = 17620; off <= 17622; ++off) {
        rec(2)[off] = 0x80;  // signed-negative still counts as set
        if (netInContact__Fv() != 1) return 7;
        rec(2)[off] = 0;
        if (netInContact__Fv() != 0) return 8;
    }
    rec(2)[17623] = 1;  // not one of the tested bytes
    if (netInContact__Fv() != 0) return 9;
    return 0;
}
