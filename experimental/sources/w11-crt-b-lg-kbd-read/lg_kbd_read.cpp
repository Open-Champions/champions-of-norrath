// Observed lgKbdRead at 0x002e64f0, 608 bytes, from the keyboard library the image links in.
// Arguments: a handle (as returned by lgKbdOpen: record position plus one) and a destination.
// The library state is the object GLibData (0x008ebdd8): the semaphore id is the word at offset
// 44, the record count the word at 64, the word at 96 an enable flag, the 64-bit word at 112 and
// the word at 120 two time values (see lgKbdSetRepeatRate), and the word at 128 the address of a
// no-argument time function returning a 64-bit value (lgKbMInit stores get_time there). Records
// are 272 bytes apart from offset 136; within a record: +16 a pending result word, +28 a state,
// +38 an unsigned halfword event count, +40 up to ten 16-byte events, +200 a 16-byte saved
// event, +212 a halfword read back as that event's kind, +216 a 64-bit due time.
//   index (handle - 1) above the count, negative, or destination null: returns 0x80000004.
//   Record state decides a status: 4 continues; 1 or 2 returns 0x8000000f; 6 returns 0x80000006;
//   anything else returns 0x80000000. A nonzero pending word at +16 is returned as it is.
//   Otherwise the time function is called, the semaphore is taken, and
//     events present: the count goes to destination halfword +2 and the events are copied to
//       destination +4 in 16-byte steps; the last event is saved at record +200; the due time
//       at +216 becomes now + the word at 120.
//     no events, flag at 96 set, saved event's halfword at +212 equal to 1, now not before the
//       due time, and IsEventModifier(record + 200) zero: the saved event is delivered (count 1
//       at +2, event at +4, then halfword 3 stored over destination +16) and the due time
//       becomes now + the 64-bit word at 112.
//     every other case delivers nothing.
//   In all of these the destination halfword +0 is then cleared, initialise_event_list(record)
//   runs, the semaphore is released and 0 is returned.
// Inferred only: the delivered-saved-event path is a key repeat. The meaning of the event bytes
// and of the stored 3 is not established. Nothing was executed on the EE.
extern "C" {
extern unsigned char GLibData[];
int WaitSema(int semaphore);
int SignalSema(int semaphore);
int IsEventModifier(void* event);
void initialise_event_list(void* record);

static inline int& word(unsigned offset) { return *reinterpret_cast<int*>(GLibData + offset); }

static inline void copy_event(unsigned char* to, const unsigned char* from) {
    for (int b = 0; b < 16; ++b) {
        to[b] = from[b];
    }
}

int lgKbdRead(int handle, unsigned char* out) {
    const int index = handle - 1;
    if (word(64) < index || index < 0 || out == 0) {
        return static_cast<int>(0x80000004u);
    }
    unsigned char* record = GLibData + 136 + 272 * index;
    const int state = *reinterpret_cast<int*>(record + 28);
    int status;
    if (state == 4) {
        status = 1;
    } else if (state < 5) {
        status = state < 3 ? (state > 0 ? static_cast<int>(0x8000000fu) : static_cast<int>(0x80000000u))
                           : static_cast<int>(0x80000000u);
    } else {
        status = state == 6 ? static_cast<int>(0x80000006u) : static_cast<int>(0x80000000u);
    }
    if (status != 1) {
        return status;
    }
    const int pending = *reinterpret_cast<int*>(record + 16);
    if (pending != 0) {
        return pending;
    }
    typedef long long (*TimeFunction)();
    const long long now = reinterpret_cast<TimeFunction>(static_cast<unsigned>(word(128)))();
    WaitSema(word(44));
    *reinterpret_cast<unsigned short*>(out + 2) = *reinterpret_cast<unsigned short*>(record + 38);
    if (*reinterpret_cast<unsigned short*>(record + 38) != 0) {
        for (int j = 0; j < *reinterpret_cast<unsigned short*>(record + 38); ++j) {
            copy_event(out + 4 + 16 * j, record + 40 + 16 * j);
        }
        const int count = *reinterpret_cast<unsigned short*>(record + 38);
        copy_event(record + 200, record + 24 + 16 * count);
        *reinterpret_cast<long long*>(record + 216) = now + word(120);
    } else if (word(96) != 0 && *reinterpret_cast<unsigned short*>(record + 212) == 1 &&
               static_cast<unsigned long long>(now) >= *reinterpret_cast<unsigned long long*>(record + 216) &&
               IsEventModifier(record + 200) == 0) {
        copy_event(out + 4, record + 200);
        *reinterpret_cast<unsigned short*>(out + 2) = 1;
        *reinterpret_cast<unsigned short*>(out + 16) = 3;
        *reinterpret_cast<long long*>(record + 216) = now + *reinterpret_cast<long long*>(GLibData + 112);
    }
    *reinterpret_cast<unsigned short*>(out) = 0;
    initialise_event_list(record);
    SignalSema(word(44));
    return 0;
}
}
