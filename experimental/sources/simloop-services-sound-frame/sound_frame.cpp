// Observed soundFrame__Fv at 0x0024b7f8, 492 bytes.
//
// runMusicDucking runs first. Each of the 32 delaySoundList entries (36
// bytes, 0x007d8aa0) with a nonzero due word at +24 that is not after
// soundFrameNm is played: sequenceInProgress takes the word at +28, then
// soundPlaySound receives the header at +0, the position at +4 unless the
// byte at +32 is zero, the floats at +16 and +20, and 0.0f. The due word is
// cleared afterwards. sequenceInProgress then becomes -1 and pauseProcess
// runs. runScreaming runs when the low two bits of soundFrameNm are zero,
// then runSequences, runAmbient and soundFlushCommandBuffer.
//
// soundFrameNm is incremented. When its low three bits then equal 1,
// sceSdRemote(1, 0x8040, 0x1700, 0) and (1, 0x8040, 0x1701, 0) give two
// words. For each of the 43 channel records, channelVoiceInfo supplies a
// word index and bit number; when that bit is set in the word and bit 0x10 of
// the record's halfword +4 is clear, bit 4 is cleared from it.
//
// soundGetStreamPlayStatus(0) without bit 0x20 sets stream 0's volume to
// min(1.0f, 2 * soundMusicVolume) * 0.78125f. A positive disableSoundPlaying
// then decreases by one. soundUpdateAdjustments runs when the low four bits of
// soundFrameNm are zero.
extern "C" {
extern unsigned char delaySoundList[];
extern unsigned char norrath_sound_channel_records_view[];
extern unsigned char norrath_sound_channel_voice_map_view[];
extern int soundFrameNm;
extern int sequenceInProgress;
extern int disableSoundPlaying;
extern float soundMusicVolume;

void runMusicDucking__Fv();
void pauseProcess__Fv();
void runScreaming__Fv();
void runSequences__Fv();
void runAmbient__Fv();
void soundFlushCommandBuffer__Fv();
void soundUpdateAdjustments__Fv();
int soundGetStreamPlayStatus__Fi(int index);
void soundSetStreamVolume__Fif(int index, float volume);
int soundPlaySound__FP10_vagHeaderP6Point3fff(void* header, void* position, float gain, float scale, float delay);
int sceSdRemote(int one, int command, int argument, int extra);

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static float load_float(const unsigned char* slot) {
    float value;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
    bytes[0] = slot[0]; bytes[1] = slot[1]; bytes[2] = slot[2]; bytes[3] = slot[3];
    return value;
}
static float bits(unsigned value) {
    float out;
    unsigned char* bytes = reinterpret_cast<unsigned char*>(&out);
    bytes[0] = static_cast<unsigned char>(value);
    bytes[1] = static_cast<unsigned char>(value >> 8);
    bytes[2] = static_cast<unsigned char>(value >> 16);
    bytes[3] = static_cast<unsigned char>(value >> 24);
    return out;
}
static void* pointer_at(unsigned address) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(address));
}

void soundFrame__Fv() {
    runMusicDucking__Fv();
    unsigned char* entry = delaySoundList;
    for (int index = 31; index >= 0; --index, entry += 36) {
        const int due = static_cast<int>(load_word(entry + 24));
        if (due == 0) continue;
        if (soundFrameNm < due) continue;
        const unsigned sequence = load_word(entry + 28);
        void* position = static_cast<signed char>(entry[32]) == 0 ? nullptr : entry + 4;
        sequenceInProgress = static_cast<int>(sequence);
        soundPlaySound__FP10_vagHeaderP6Point3fff(pointer_at(load_word(entry)), position, load_float(entry + 16),
                                                 load_float(entry + 20), bits(0));
        store_word(entry + 24, 0);
    }
    sequenceInProgress = -1;
    pauseProcess__Fv();
    if ((soundFrameNm & 3) == 0) runScreaming__Fv();
    runSequences__Fv();
    runAmbient__Fv();
    soundFlushCommandBuffer__Fv();
    const int next = soundFrameNm + 1;
    soundFrameNm = next;
    if ((next & 7) == 1) {
        int words[2];
        words[0] = sceSdRemote(1, 0x8040, 0x1700, 0);
        words[1] = sceSdRemote(1, 0x8040, 0x1701, 0);
        unsigned char* channel = norrath_sound_channel_records_view;
        for (int index = 0; index < 43; ++index, channel += 20) {
            const int group = static_cast<signed char>(norrath_sound_channel_voice_map_view[index * 2]);
            const int bit = static_cast<signed char>(norrath_sound_channel_voice_map_view[index * 2 + 1]);
            if (((words[group] >> (bit & 31)) & 1) == 0) continue;
            const unsigned flags = channel[4] | (channel[5] << 8);
            if ((flags & 0x10u) == 0) {
                const unsigned cleared = flags & 0xfffffffbu;
                channel[4] = static_cast<unsigned char>(cleared);
                channel[5] = static_cast<unsigned char>(cleared >> 8);
            }
        }
    }
    if ((soundGetStreamPlayStatus__Fi(0) & 0x20) == 0) {
        const float doubled = soundMusicVolume + soundMusicVolume;
        const float one = bits(0x3f800000u);
        const float limited = one < doubled ? one : doubled;
        soundSetStreamVolume__Fif(0, limited * bits(0x3f480000u));
    }
    if (disableSoundPlaying > 0) disableSoundPlaying = disableSoundPlaying - 1;
    if ((soundFrameNm & 0xf) == 0) soundUpdateAdjustments__Fv();
}
}
