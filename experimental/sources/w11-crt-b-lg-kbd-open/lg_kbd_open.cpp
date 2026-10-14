// Observed lgKbdOpen at 0x002e63d0, 284 bytes, from the keyboard library the image links in.
// Arguments: a device index and a destination for a handle word. The library state is the object
// GLibData (0x008ebdd8): the semaphore id is the word at offset 44, the record count is the word
// at offset 64, and the word at offset 88 is a counter. Records are 272 bytes apart starting at
// offset 136; within a record the word at +20 is a handle, +24 the owning index and +28 a state
// (the same record layout lgKbdEnumerate scans).
//   index above the count, index negative, or destination null: returns 0x80000004.
//   Otherwise the destination is first set to -1, then records are scanned from 0 to the count:
//     owning index equal to the argument and state 4: returns 0x80000005; state 6: returns
//     0x80000006; state 2: under the semaphore the record's handle becomes its position plus one,
//     its state becomes 4 and the counter at 88 is incremented; the destination receives the
//     handle and 0 is returned. Any other state, or a different owner, keeps scanning.
//   No decision: returns 0x80000002 (the destination stays -1).
// The test is "count < index", so an index equal to the count passes the first check and then
// finds no record. Inferred only: the state meanings (2 appears to be the state lgKbdEnumerate
// leaves a claimed record in; 4 the opened state). Nothing was executed on the EE.
extern "C" {
extern unsigned char GLibData[];
int WaitSema(int semaphore);
int SignalSema(int semaphore);

static inline int& word(unsigned offset) { return *reinterpret_cast<int*>(GLibData + offset); }

int lgKbdOpen(int index, int* handle) {
    if (word(64) < index || index < 0 || handle == 0) {
        return static_cast<int>(0x80000004u);
    }
    *handle = -1;
    const int count = word(64);
    for (int i = 0; i < count; ++i) {
        unsigned char* record = GLibData + 136 + 272 * i;
        if (*reinterpret_cast<int*>(record + 24) != index) {
            continue;
        }
        const int state = *reinterpret_cast<int*>(record + 28);
        if (state == 4) return static_cast<int>(0x80000005u);
        if (state == 6) return static_cast<int>(0x80000006u);
        if (state == 2) {
            WaitSema(word(44));
            *reinterpret_cast<int*>(record + 20) = i + 1;
            *reinterpret_cast<int*>(record + 28) = 4;
            word(88) = word(88) + 1;
            SignalSema(word(44));
            *handle = *reinterpret_cast<int*>(record + 20);
            return 0;
        }
    }
    return static_cast<int>(0x80000002u);
}
}
