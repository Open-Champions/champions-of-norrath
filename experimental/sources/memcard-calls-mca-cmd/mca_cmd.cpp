// Observed: MCA_Cmd__Fiiii at 0x0025c790 is 116 bytes. It writes the four
// arguments into commandQueue[queueEnd], then sets queueEnd to the old
// value plus one. The new index becomes 0 when it would be 256. If that
// index equals queueStart, it counts down from 223 by 32 and returns.
// The four stores happen before the wrap compare.
// Inferred: commandQueue entries are 16 bytes, 256 of them.
// Unknown: whether a reader consumes the slot during the countdown.

extern "C" int queueEnd;
extern "C" int queueStart;

struct McCommand {
    int command;
    const void* text;
    int third;
    int fourth;
};

extern "C" McCommand commandQueue[256];

extern "C" void MCA_Cmd__Fiiii(int command, const void* text, int third, int fourth) {
    int slot = queueEnd;
    int next = queueEnd + 1;
    if ((next ^ 0x100) == 0) next = 0;
    commandQueue[slot].command = command;
    commandQueue[slot].text = text;
    commandQueue[slot].third = third;
    commandQueue[slot].fourth = fourth;
    queueEnd = next;
    // The original counts 223 down by 32 with no stores when the new index
    // meets queueStart. The countdown has no result; this empty barrier keeps
    // the queueStart read in the compiled object.
    if (next == queueStart) asm volatile("" : : "r"(queueStart) : "memory");
}
