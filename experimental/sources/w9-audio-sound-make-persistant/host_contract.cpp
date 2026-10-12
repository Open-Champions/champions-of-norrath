#include "sound_make_persistant.cpp"
#include <cstdio>
#include <cstring>
extern "C" unsigned char norrath_sound_sequence_records_view[256 * 76] = {};
extern "C" unsigned char norrath_sound_channel_records_view[860] = {};
static unsigned char seq_copy[256 * 76], ch_copy[860];
static void snap() { std::memcpy(seq_copy, norrath_sound_sequence_records_view, sizeof seq_copy); std::memcpy(ch_copy, norrath_sound_channel_records_view, sizeof ch_copy); }
static bool unchanged() { return !std::memcmp(seq_copy, norrath_sound_sequence_records_view, sizeof seq_copy) && !std::memcmp(ch_copy, norrath_sound_channel_records_view, sizeof ch_copy); }
int main() {
    std::memset(norrath_sound_sequence_records_view, 0xaa, sizeof norrath_sound_sequence_records_view);
    std::memset(norrath_sound_channel_records_view, 0x10, sizeof norrath_sound_channel_records_view);
    snap();
    soundMakePersistant__Fi(-1);
    soundMakePersistant__Fi(-0x8001);               // negative, bit 0x8000 clear
    if (!unchanged()) return 1;
    // channel 3: bit 0x1 of the halfword at +4 only
    soundMakePersistant__Fi(3);
    unsigned short flags; std::memcpy(&flags, ch_copy + 3 * 20 + 4, 2);
    ch_copy[3 * 20 + 4] |= 1;
    if (!unchanged() || flags != 0x1010) return 2;
    // sequence 5
    unsigned char* rec = norrath_sound_sequence_records_view + 5 * 76;
    soundMakePersistant__Fi(0x8105);
    float f16, f24; int w20, w28; std::memcpy(&f16, rec + 16, 4); std::memcpy(&f24, rec + 24, 4);
    std::memcpy(&w20, rec + 20, 4); std::memcpy(&w28, rec + 28, 4); std::memcpy(&flags, rec + 36, 2);
    if (rec[60] != 1 || f16 != 1.0f || f24 != 1.0f || w20 != 0 || w28 != 0) return 3;
    if (flags != 0xaaab) return 4;                       // 0xaaaa | 1, bit 2 cleared (it was clear)
    std::memcpy(seq_copy + 5 * 76, rec, 76);
    // neighbouring records and all channels are untouched
    if (!unchanged()) return 5;
    // bit 0x4 is cleared when set
    std::memset(norrath_sound_sequence_records_view, 0xff, 76 * 256);
    soundMakePersistant__Fi(0x8000);
    std::memcpy(&flags, norrath_sound_sequence_records_view + 36, 2);
    if (flags != 0xfffb) return 6;
    std::puts("sound_make_persistant passed");
    return 0;
}
