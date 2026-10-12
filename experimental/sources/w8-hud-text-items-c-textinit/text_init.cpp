// Observed textInit__Fv at 0x002cb388, 136 bytes.
// While the word at 0x00483f30 (frontEndRunning) is zero, the function clears
// the 1008-byte table at 0x008d8938 (inMsg), then writes 0xfe into byte 2 of
// each of its eight 126-byte entries, last entry first. It then sets the
// block at 0x008d88b8 (outMsg): byte 0 = 1, byte 1 = 0, byte 3 = 32,
// byte 4 = 0. When the word is nonzero nothing is touched.
extern "C" void* memset(void* destination, int value, unsigned count);

struct NorrathInMsgEntry {
    unsigned char before[2];
    signed char marker;
    unsigned char after[123];
};

struct NorrathOutMsgHead {
    unsigned char first;
    unsigned char second;
    unsigned char unobserved;
    unsigned char third;
    unsigned char fourth;
};

extern "C" int frontEndRunning;
extern "C" NorrathInMsgEntry norrath_in_msg_entries[8];
extern "C" NorrathOutMsgHead norrath_out_msg_head;

extern "C" void textInit__Fv() {
    if (frontEndRunning != 0) return;
    memset(norrath_in_msg_entries, 0, sizeof norrath_in_msg_entries);
    norrath_out_msg_head.fourth = 0;
    norrath_out_msg_head.second = 0;
    norrath_out_msg_head.first = 1;
    norrath_out_msg_head.third = 32;
    for (int i = 7; i >= 0; i--) norrath_in_msg_entries[i].marker = -2;
}
