// Observed runSequences__Fv at 0x0024d878, 516 bytes.
// It reads gameIsPaused once, then visits the 32 sequence records (76 bytes,
// 0x007d8110) in order. Records whose byte +4 is zero are skipped. For a
// live record the words and bytes used are: header pointer at +8, group byte
// at +60, flags halfword at +6, table index halfword at +38, group halfword
// at +56, object-list pointer at +40, a 12-byte position at +44, bytes at +58
// and +59, and the float volume at +64.
//   Byte +60 zero: the header's word at +56 is a table of 28-byte entries
//   indexed by the halfword at +38. If the table is missing, the entry's first
//   word is 0x7fffffff, or flag 0x80 of the +6 halfword is set, the record
//   ends: byte +4 is cleared and the 43 channel records (20-byte stride at
//   0x007d8f20) are searched; every record with flags 4 and 0x10 both set, a
//   halfword at +6 equal to the record index and a halfword at +8 equal to the
//   record's +56 halfword gets soundStopSound(channel index). The record is
//   then finished.
//   Byte +60 nonzero: if the float at +32 (the header's +8 plus 24) exceeds
//   (float)(word at header+64), that float is set to zero.
// An unfinished record is skipped when the pause result is nonzero and the
// byte +59 is zero. Otherwise, a non-null list pointer at +40 supplies an
// object through its word +12, and the 12 bytes at object+16 are copied to
// +44. A nonzero byte +58 then replaces +44 with the 12 bytes at
// worldCameraPos (0x0037ee80). sequenceVolume takes the float at +64 and
// sequenceInProgress the record index, then animAdvanceAnimation(record,
// record+44, 0, 0) is called. After the loop sequenceInProgress is -1 and
// sequenceVolume is 1.0.
// Inferred: +44 is a source position, +58 selects the listener position, +59
// lets a record run while paused. These are guesses from use. Unknown: what
// the +60 group byte means and the meaning of flag 0x80.
extern "C" {
int gameIsPaused__Fv();
void soundStopSound__Fi(int channel);
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(
    void* state, void* position, void* second, short* third);
extern unsigned char norrath_sound_sequence_records_view[];
extern unsigned char norrath_sound_channel_records_view[];
extern unsigned char worldCameraPos[];
extern int sequenceInProgress;
extern float sequenceVolume;
}

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void* norrath_host_pointer(unsigned address);
#endif

namespace {
unsigned char* pointer_at(unsigned address) {
#if defined(NORRATH_HOST_CONTRACT)
    return static_cast<unsigned char*>(norrath_host_pointer(address));
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(address));
#endif
}
unsigned word_at(const unsigned char* p) {
    return p[0] | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
int half_at(const unsigned char* p) { return static_cast<short>(p[0] | (p[1] << 8)); }
unsigned uhalf_at(const unsigned char* p) { return p[0] | (unsigned(p[1]) << 8); }
void copy12(unsigned char* to, const unsigned char* from) {
    for (int i = 0; i < 12; ++i) to[i] = from[i];
}
}

extern "C" void runSequences__Fv() {
    const int paused = gameIsPaused__Fv();
    for (int index = 0; index < 32; ++index) {
        unsigned char* const record = norrath_sound_sequence_records_view + index * 76;
        if (static_cast<signed char>(record[4]) == 0) continue;
        unsigned char* const header = pointer_at(word_at(record + 8));
        bool finished = false;
        if (static_cast<signed char>(record[60]) == 0) {
            const unsigned table_word = word_at(header + 56);
            if (table_word == 0u) {
                finished = true;
            } else {
                unsigned char* const table = pointer_at(table_word);
                if (word_at(table + half_at(record + 38) * 28) == 0x7fffffffu ||
                    (uhalf_at(record + 6) & 0x80u) != 0u)
                    finished = true;
            }
        } else {
            float* const countdown = reinterpret_cast<float*>(record + 32);
            if (static_cast<float>(static_cast<int>(word_at(header + 64))) < *countdown)
                *countdown = 0.0f;
        }
        if (finished) {
            record[4] = 0;
            for (int channel = 0; channel < 43; ++channel) {
                const unsigned char* const info = norrath_sound_channel_records_view + channel * 20;
                const unsigned flags = uhalf_at(info + 4);
                if ((flags & 4u) == 0u) continue;
                if ((flags & 0x10u) == 0u) continue;
                if (half_at(info + 6) != index) continue;
                if (half_at(info + 8) != static_cast<int>(uhalf_at(record + 56))) continue;
                soundStopSound__Fi(channel);
            }
            continue;
        }
        if (paused != 0 && static_cast<signed char>(record[59]) == 0) continue;
        unsigned char* const position = record + 44;
        const unsigned list_word = word_at(record + 40);
        if (list_word != 0u) {
            unsigned char* const object = pointer_at(word_at(pointer_at(list_word) + 12));
            copy12(position, object + 16);
        }
        if (static_cast<signed char>(record[58]) != 0) copy12(position, worldCameraPos);
        sequenceVolume = *reinterpret_cast<float*>(record + 64);
        sequenceInProgress = index;
        animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(record, position, nullptr, nullptr);
    }
    sequenceInProgress = -1;
    sequenceVolume = 1.0f;
}
