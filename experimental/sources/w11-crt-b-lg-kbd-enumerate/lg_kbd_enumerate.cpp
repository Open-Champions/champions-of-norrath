// Observed lgKbdEnumerate at 0x002e72a8, 288 bytes, from the keyboard library the image links in.
// Arguments: a device index and a destination for ten bytes of identification. The library state
// is the object GLibData (0x008ebdd8): the semaphore id is the word at offset 44, the record
// count is the word at offset 64, and the word at offset 72 is a running minimum of the indices
// asked for. The records are 272 bytes apart starting at offset 136; within a record the word at
// +24 is the owning index, the word at +28 is a state, and ten identification bytes sit at +6.
//   index negative or destination null: returns 0x80000004.
//   Otherwise the word at 72 is lowered to the index when the index is smaller, then the first
//   record decides, scanning from record 0 up to the count:
//     state 1: under the semaphore the state becomes 2 and the owning index the argument; the ten
//              bytes at +6 are copied to the destination; returns 0. (This test comes before the
//              index comparison.)
//     owning index equal to the argument: state 4 returns 0x80000005, state 6 returns
//              0x80000006, state 2 returns 0; any other state keeps scanning.
//     different owning index: keeps scanning.
//   No decision: returns 0x80000002.
// Inferred only: the states look like a device life cycle, but nothing here names them. The copy of
// the ten bytes is read from +6 and written to destination +0. Nothing was executed on the EE.
extern "C" {
extern unsigned char GLibData[];
int WaitSema(int semaphore);
int SignalSema(int semaphore);

static inline int& word(unsigned offset) { return *reinterpret_cast<int*>(GLibData + offset); }

int lgKbdEnumerate(int index, unsigned char* identification) {
    if (index < 0 || identification == 0) {
        return static_cast<int>(0x80000004u);
    }
    if (index < word(72)) {
        word(72) = index;
    }
    const int count = word(64);
    for (int i = 0; i < count; ++i) {
        unsigned char* record = GLibData + 136 + 272 * i;
        const int state = *reinterpret_cast<int*>(record + 28);
        if (state == 1) {
            WaitSema(word(44));
            *reinterpret_cast<int*>(record + 28) = 2;
            *reinterpret_cast<int*>(record + 24) = index;
            SignalSema(word(44));
            for (int b = 0; b < 10; ++b) {
                identification[b] = record[6 + b];
            }
            return 0;
        }
        if (*reinterpret_cast<int*>(record + 24) == index) {
            if (state == 4) return static_cast<int>(0x80000005u);
            if (state == 6) return static_cast<int>(0x80000006u);
            if (state == 2) return 0;
        }
    }
    return static_cast<int>(0x80000002u);
}
}
