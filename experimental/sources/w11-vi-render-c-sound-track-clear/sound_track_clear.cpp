// Observed Clear__17VISceneSoundTrack at 0x0114e8d8, 140 bytes. Stores only; offsets are bytes
// from the object, and the result is zero.
//   +0 = 1                       +4 = 1.0f
//   +8 = -1                      +16 = 64-bit 15000
//   +24 = 64-bit 0               +32 = -1, +36 = -1
//   +40 = 64-bit 0               +48 = 1.0f
//   +56 = 0                      +64 = 64-bit 0
//   +72 = 0.0f, +76 = 0.0f       +80 = -1, +84 = -1, +88 = 0, +92 = -1
//   +96 = 0                      +104 = 64-bit 0
//   +112 = 0.0f, +116 = 0.0f     +120 = -1, +124 = -1, +128 = 0, +132 = -1
// Inferred from the repeated shapes: three records at +56 and +96 follow the head fields, each
// ending in -1; this is arithmetic on the stores, not a recovered layout. Names and the units
// of 15000 are unknown.
extern "C" {
struct VISceneSoundTrack;

int Clear__17VISceneSoundTrack(VISceneSoundTrack* track) {
    char* b = reinterpret_cast<char*>(track);
    *reinterpret_cast<int*>(b + 0) = 1;
    *reinterpret_cast<int*>(b + 8) = -1;
    *reinterpret_cast<int*>(b + 32) = -1;
    *reinterpret_cast<int*>(b + 36) = -1;
    *reinterpret_cast<float*>(b + 48) = 1.0f;
    *reinterpret_cast<int*>(b + 92) = -1;
    *reinterpret_cast<int*>(b + 56) = 0;
    *reinterpret_cast<int*>(b + 80) = -1;
    *reinterpret_cast<int*>(b + 84) = -1;
    *reinterpret_cast<int*>(b + 88) = 0;
    *reinterpret_cast<float*>(b + 4) = 1.0f;
    *reinterpret_cast<unsigned long long*>(b + 16) = 15000;
    *reinterpret_cast<unsigned long long*>(b + 24) = 0;
    *reinterpret_cast<unsigned long long*>(b + 40) = 0;
    *reinterpret_cast<float*>(b + 72) = 0.0f;
    *reinterpret_cast<float*>(b + 76) = 0.0f;
    *reinterpret_cast<unsigned long long*>(b + 64) = 0;
    *reinterpret_cast<int*>(b + 132) = -1;
    *reinterpret_cast<float*>(b + 116) = 0.0f;
    *reinterpret_cast<int*>(b + 96) = 0;
    *reinterpret_cast<unsigned long long*>(b + 104) = 0;
    *reinterpret_cast<float*>(b + 112) = 0.0f;
    *reinterpret_cast<int*>(b + 120) = -1;
    *reinterpret_cast<int*>(b + 124) = -1;
    *reinterpret_cast<int*>(b + 128) = 0;
    return 0;
}
}
