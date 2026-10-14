/* soundPlaySequence__FP15AnimationHeaderG6Point3f at 0x0024d4f8 is 424
 * bytes. It is the position overload of the object overload authored in
 * simloop-services-sound-play-sequence. The point is passed by reference
 * to a caller-made copy; this function copies its 12 bytes first and reads
 * them after the header test.
 *
 * A null header returns -1. The group byte is header+60. All 32 sequence
 * records (76 bytes, 0x007d8110) are scanned: a record with byte +4 zero is
 * free and the LAST free index is kept; a used record's header pointer
 * (word +8) with the same group byte raises a match count, and a nonzero
 * group whose count reaches the signed group-limit byte
 * (0x0042de58 + group) returns -1. No free index returns -1.
 *
 * The chosen record clears halfword +6, runs animAdd with the header and
 * flag 4, calls listDestroy on the list at +40 (nothing is linked in), and
 * stores the 12 point bytes at +44 and zero at byte +58. gameIsPaused
 * follows. The sequence id spinner (halfword, 0x00485940) is stored at +56,
 * the pause result at byte +59, the spinner becomes (old + 1) & 0x7f, byte
 * +60 is cleared, and +64, +68, +72 hold the gain, 360.0f and 840.0f. The
 * result is the index, the old spinner shifted left eight, and 0x8000.
 */
extern "C" {
struct Point3 {
    float x, y, z;
};
extern unsigned char norrath_sound_sequence_records_view[];
extern unsigned char norrath_sound_group_limits_view[];
extern unsigned char norrath_sound_sequence_id_spinner_view[];
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void listDestroy__FP8ListHead(void* head);
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
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (static_cast<unsigned>(slot[3]) << 24);
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

int soundPlaySequence__FP15AnimationHeaderG6Point3f(unsigned char* header, Point3 position, float gain) {
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
        matching += 1;
        if (group == 0) continue;
        if (!(matching < static_cast<signed char>(norrath_sound_group_limits_view[group]))) return -1;
    }
    if (chosen == -1) return -1;
    unsigned char* record = norrath_sound_sequence_records_view + chosen * 76;
    store_half(record + 6, 0);
    animAdd__FP14AnimationStateP15AnimationHeaderi(record, header, 4);
    listDestroy__FP8ListHead(record + 40);
    store_word(record + 44, bits(position.x));
    store_word(record + 48, bits(position.y));
    store_word(record + 52, bits(position.z));
    record[58] = 0;
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
