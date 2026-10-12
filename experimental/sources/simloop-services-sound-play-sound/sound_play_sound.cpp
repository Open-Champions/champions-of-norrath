// Observed soundPlaySound__FP10_vagHeaderP6Point3fff at 0x0024c6a8, 1384 bytes.
// Arguments after the header and optional position are three floats: gain
// (a), volume scale (b), and delay seconds (c). All data below is by address;
// names are the executable's symbol names.
//
// A nonzero word at disableSoundPlaying (0x00486bd0) returns -1.
// c > 0: the first of the 32 delaySoundList entries (36 bytes, 0x007d8aa0)
// whose word +24 is zero receives the header address at +0, the position
// (12 bytes at +4, flag byte +32 set to 1; no position clears the flag),
// gain at +16, b * sequenceVolume at +20, sequenceInProgress at +28, and
// soundFrameNm plus truncated c * 60.0f at +24. The call returns -1 whether
// it queued or found no free entry.
// c <= 0: a header with a nonzero halfword at +40 needs sequenceInProgress
// not -1, and is rejected when a channel record already holds both this
// header and that sequence id (43 records of 20 bytes, halfword +6). Then
// findChannel supplies the channel; -1 is returned if there is none.
// The channel record gets the header address, flags halfword 4 (20 when the
// +40 halfword is nonzero), the clamped volume int(float(word +32) * gain)
// in 0..16383 at +12, the sequence id at +6, the sequence record's halfword
// +56 at +8, b at +16, and bit 8 of the flags when the sequence record's byte
// +59 is nonzero. pan runs with a copy of the position and either the
// sequence record's floats +68 and +72 or 360.0f and 840.0f; without a
// position both outputs are 1.0f. Each output is multiplied by b times
// sequenceVolume. If both are <= 0 and the +40 halfword is zero the call
// returns -1. soundChangeVolume follows. Up to five commands are then
// appended to batchBuffer (8-byte entries, limit 512, count at
// nmBatchCommands). The channelVoiceInfo bytes for the channel pick the
// kons/koffs word and bit. The channel number is returned.
extern "C" {
struct Point3 { float x, y, z; };

extern unsigned char delaySoundList[];
extern unsigned char batchBuffer[];
extern unsigned char norrath_sound_channel_records_view[];
extern unsigned char norrath_sound_channel_voice_map_view[];
extern unsigned char norrath_sound_sequence_records_view[];
extern unsigned char norrath_sound_key_on_words_view[];
extern unsigned char norrath_sound_key_off_words_view[];
extern int disableSoundPlaying;
extern int nmBatchCommands;
extern int sequenceInProgress;
extern int soundFrameNm;
extern float sequenceVolume;

int findChannel__Fv();
void pan__FG6Point3ffPfT3(Point3 position, float a, float b, float* left, float* right);
void soundChangeVolume__Fiff(int channel, float left, float right);

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
static int load_half_signed(const unsigned char* slot) {
    return static_cast<int>(static_cast<short>(slot[0] | (slot[1] << 8)));
}
static unsigned load_half(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8);
}
static void store_float(unsigned char* slot, float value) {
    store_word(slot, load_word(reinterpret_cast<const unsigned char*>(&value)));
}
static float load_float(const unsigned char* slot) {
    float value;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
    bytes[0] = slot[0]; bytes[1] = slot[1]; bytes[2] = slot[2]; bytes[3] = slot[3];
    return value;
}
static unsigned address_of(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static void copy_position(unsigned char* destination, const unsigned char* source) {
    store_word(destination, load_word(source));
    store_word(destination + 4, load_word(source + 4));
    store_word(destination + 8, load_word(source + 8));
}
static Point3 point_at(const unsigned char* slot) {
    Point3 point;
    point.x = load_float(slot);
    point.y = load_float(slot + 4);
    point.z = load_float(slot + 8);
    return point;
}
static void append_command(unsigned type, unsigned flags, unsigned value) {
    if (nmBatchCommands < 512) {
        unsigned char* entry = batchBuffer + nmBatchCommands * 8;
        nmBatchCommands += 1;
        store_half(entry, type);
        store_half(entry + 2, flags);
        store_word(entry + 4, value);
    }
}

int soundPlaySound__FP10_vagHeaderP6Point3fff(unsigned char* vag, unsigned char* position, float a, float b, float c) {
    if (disableSoundPlaying != 0) return -1;
    const float scale = sequenceVolume;
    if (c > 0.0f) {
        unsigned char* entry = delaySoundList;
        for (int index = 0; index < 32; ++index, entry += 36) {
            if (load_word(entry + 24) != 0) continue;
            store_word(entry, address_of(vag));
            if (position != nullptr) {
                copy_position(entry + 4, position);
                entry[32] = 1;
            } else {
                entry[32] = 0;
            }
            const int frames = static_cast<int>(c * 60.0f);
            store_float(entry + 16, a);
            store_float(entry + 20, b * scale);
            store_word(entry + 28, static_cast<unsigned>(sequenceInProgress));
            store_word(entry + 24, static_cast<unsigned>(soundFrameNm + frames));
            return -1;
        }
        return -1;
    }

    const int vag_flag = load_half_signed(vag + 40);
    if (vag_flag != 0) {
        const int sequence = sequenceInProgress;
        if (sequence != -1) {
            unsigned char* channel = norrath_sound_channel_records_view;
            for (int index = 0; index < 43; ++index, channel += 20) {
                if (load_half_signed(channel + 6) == sequence && load_word(channel) == address_of(vag)) return -1;
            }
        }
        if (sequence == -1) return -1;
    }

    const int found = findChannel__Fv();
    if (found == -1) return -1;
    unsigned char* channel = norrath_sound_channel_records_view + found * 20;
    store_half(channel + 4, 4);
    const unsigned char voice_group = norrath_sound_channel_voice_map_view[found * 2];
    const unsigned char voice_bit = norrath_sound_channel_voice_map_view[found * 2 + 1];
    int level = static_cast<int>(static_cast<float>(static_cast<int>(load_word(vag + 32))) * a);
    store_word(channel, address_of(vag));
    if (level < 0) level = 0;
    if (!(level < 16384)) level = 16383;
    if (vag_flag != 0) store_half(channel + 4, 20);
    store_word(channel + 12, static_cast<unsigned>(level));
    store_half(channel + 6, static_cast<unsigned>(sequenceInProgress));
    const int sequence = sequenceInProgress;
    unsigned char* sequence_record = sequence == -1 ? nullptr : norrath_sound_sequence_records_view + sequence * 76;
    if (sequence != -1) store_half(channel + 8, load_half(sequence_record + 56));
    store_float(channel + 16, b);
    const float volume = b * scale;
    if (sequence != -1 && static_cast<signed char>(sequence_record[59]) != 0) {
        store_half(channel + 4, load_half(channel + 4) | 8);
    }

    float left;
    float right;
    if (position != nullptr) {
        if (sequence != -1) {
            pan__FG6Point3ffPfT3(point_at(position), load_float(sequence_record + 68), load_float(sequence_record + 72), &left, &right);
        } else {
            pan__FG6Point3ffPfT3(point_at(position), 360.0f, 840.0f, &left, &right);
        }
    } else {
        right = 1.0f;
        left = 1.0f;
    }
    left = left * volume;
    right = right * volume;
    if (left <= 0.0f && right <= 0.0f && vag_flag == 0) return -1;
    soundChangeVolume__Fiff(found, left, right);

    const unsigned flags = static_cast<unsigned>(static_cast<int>(static_cast<signed char>(voice_group)) | (static_cast<int>(static_cast<signed char>(voice_bit)) << 1));
    append_command(1, flags | 0x200, static_cast<unsigned>(level));
    append_command(3, flags | 0x2040, load_word(vag + 36));
    append_command(1, flags | 0x300, 255);
    append_command(1, flags | 0x400, 8128);
    append_command(3, flags | 0x2140, 20480);

    const int group = static_cast<signed char>(voice_group);
    const int bit = static_cast<signed char>(voice_bit);
    const unsigned mask = 1u << (bit & 31);
    store_word(norrath_sound_key_on_words_view + group * 4, load_word(norrath_sound_key_on_words_view + group * 4) | mask);
    store_word(norrath_sound_key_off_words_view + group * 4, load_word(norrath_sound_key_off_words_view + group * 4) & ~mask);
    return found;
}
}
