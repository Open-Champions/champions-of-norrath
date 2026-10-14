#include "sound_change_sequence_volume.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
extern "C" unsigned char norrath_sound_sequence_records_view[256 * 76] = {};
extern "C" unsigned char norrath_sound_channel_records_view[860] = {};
static struct { int channel; float left, right; } changes[64];
static int nchanges, npan;
static Point3 pan_seen;
static float pan_a, pan_b;
extern "C" void pan__FG6Point3ffPfT3(Point3 p, float a, float b, float* l, float* r) {
    ++npan; pan_seen = p; pan_a = a; pan_b = b; *l = p.x; *r = p.y;   // stands in for the real attenuation
}
extern "C" void soundChangeVolume__Fiff(int c, float l, float r) {
    changes[nchanges++] = {c, l, r};
}
static void setf(unsigned char* base, int off, float v) { std::memcpy(base + off, &v, 4); }
static void seth(unsigned char* base, int off, short v) { std::memcpy(base + off, &v, 2); }
int main() {
    unsigned char* seq = norrath_sound_sequence_records_view;
    unsigned char* ch = norrath_sound_channel_records_view;
    // sequence 3 uses channels 2, 5, 40; channel 6 has the right owner but no bit 0x4; channel 7 another owner
    unsigned char* s3 = seq + 3 * 76;
    setf(s3, 44, 10.0f); setf(s3, 48, -4.0f); setf(s3, 52, 7.0f); setf(s3, 68, 360.0f); setf(s3, 72, 840.0f);
    for (int c : {2, 5, 40, 6, 7}) { seth(ch + c * 20, 4, c == 6 ? 0x3 : 0x5); seth(ch + c * 20, 6, c == 7 ? 4 : 3); setf(ch + c * 20, 16, 0.5f); }
    setf(ch + 5 * 20, 16, 2.0f);
    soundChangeSequenceVolume__Fif(-1, 1.0f);
    soundChangeSequenceVolume__Fif(3, 1.0f);                       // no 0x8000
    soundChangeSequenceVolume__Fif(0x0103, 1.0f);
    if (nchanges != 0) return 1;
    float stored; std::memcpy(&stored, s3 + 64, 4);
    if (stored != 0.0f) return 2;
    soundChangeSequenceVolume__Fif(0x8103, 2.0f);                  // the high bits above the index byte are ignored
    if (nchanges != 3 || npan != 3) return 3;
    std::memcpy(&stored, s3 + 64, 4);
    if (stored != 2.0f) return 4;
    if (changes[0].channel != 2 || changes[1].channel != 5 || changes[2].channel != 40) return 5;
    if (changes[0].left != 10.0f || changes[0].right != 0.0f) return 6;   // 10*(2*0.5), -4 limited to 0
    if (changes[1].left != 40.0f || changes[2].left != 10.0f) return 7;                                   // scale 4.0 for channel 5
    if (pan_seen.x != 10.0f || pan_seen.y != -4.0f || pan_seen.z != 7.0f || pan_a != 360.0f || pan_b != 840.0f) return 8;
    seth(ch + 2 * 20, 6, -1);                                      // owner is compared as a signed halfword
    nchanges = 0;
    soundChangeSequenceVolume__Fif(0x80ff, 1.0f);                  // index 255 is accepted as given (the host array holds 256 records); no channel owns it
    if (nchanges != 0) return 9;
    std::puts("sound_change_sequence_volume passed");
    return 0;
}
