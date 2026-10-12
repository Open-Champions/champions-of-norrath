// Observed soundPlaySequence__FP15AnimationHeaderP10GameObjectf at
// 0x0024d308, 492 bytes.
//
// A null header returns -1. The group byte is header+60. All 32 sequence
// records (76 bytes each, 0x007d8110) are scanned; a record whose byte +4 is
// zero is free and the LAST free index is kept, not the first. A used
// record's header pointer (word +8) is read, and when its group byte matches
// and the group is not zero, a match count rises; once the count is not below
// the group-limit byte (0x0042de58 plus the group) the result is -1. The scan
// then needs a free index or returns -1.
//
// The chosen record clears its halfword +6 and calls animAdd on the record
// with the header and flag 4. A non-null object clears the record's list at
// +40, links the object into it, copies the 12 bytes at object+16 into +44 and
// clears byte +58. A null object sets byte +58 to 1. gameIsPaused follows. The
// halfword at 0x00485940 is stored at +56, the pause result goes to byte +59,
// the halfword becomes (previous + 1) & 0x7f, byte +60 is cleared, and +64,
// +68, +72 hold the gain argument, 360.0f, and 840.0f. The result is the
// index, the previous halfword shifted left eight, and 0x8000.
extern "C" {
extern unsigned char norrath_sound_sequence_records_view[];
extern unsigned char norrath_sound_group_limits_view[];
extern unsigned char norrath_sound_sequence_id_spinner_view[];
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void listDestroy__FP8ListHead(void* head);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* object);
int gameIsPaused__Fv();

#if defined(NORRATH_HOST_CONTRACT)
void* norrath_host_pointer(unsigned address);
#endif
static unsigned char* pointer_at(unsigned address) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(address));
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}

static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

static void store_half(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
}

static unsigned bits(float value) {
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&value);
    return load_word(bytes);
}

int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(unsigned char* header, unsigned char* object, float gain) {
    if (header == nullptr) return -1;
    const unsigned group = header[60];
    int chosen = -1;
    int matching = 0;
    for (int index = 0; index < 32; ++index) {
        unsigned char* record = norrath_sound_sequence_records_view + index * 76;
        if (record[4] == 0) {
            chosen = index;
            continue;
        }
        const unsigned char* active = pointer_at(load_word(record + 8));
        if (active[60] != group) continue;
        if (group == 0) continue;
        matching += 1;
        if (!(matching < static_cast<signed char>(norrath_sound_group_limits_view[group]))) return -1;
    }
    if (chosen == -1) return -1;
    unsigned char* record = norrath_sound_sequence_records_view + chosen * 76;
    store_half(record + 6, 0);
    animAdd__FP14AnimationStateP15AnimationHeaderi(record, header, 4);
    if (object != nullptr) {
        listDestroy__FP8ListHead(record + 40);
        listOnewayLink__FP8ListHeadT0Pv(record + 40, object, object);
        const unsigned first = load_word(object + 16);
        const unsigned second = load_word(object + 20);
        const unsigned third = load_word(object + 24);
        store_word(record + 44, first);
        store_word(record + 48, second);
        store_word(record + 52, third);
        record[58] = 0;
    } else {
        record[58] = 1;
    }
    const int paused = gameIsPaused__Fv();
    const unsigned previous = norrath_sound_sequence_id_spinner_view[0] | (norrath_sound_sequence_id_spinner_view[1] << 8);
    store_half(record + 56, previous);
    record[59] = static_cast<unsigned char>(paused);
    store_half(norrath_sound_sequence_id_spinner_view, (previous + 1) & 0x7f);
    record[60] = 0;
    store_word(record + 64, bits(gain));
    store_word(record + 68, 0x43b40000u);
    store_word(record + 72, 0x44520000u);
    return static_cast<int>(static_cast<unsigned>(chosen) | (previous << 8) | 0x8000u);
}
}
