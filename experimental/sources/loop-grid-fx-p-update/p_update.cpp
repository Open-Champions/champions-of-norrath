// Observed P_Update__Fv at 0x1812e0, extent 336. Registered as the
// updateParticles task. In order:
//  1. A nonzero particlesPaused byte (0x484300) returns. So does a zero
//     objectDrawEnable byte (0x483ac8) and a nonzero gameIsPaused result.
//  2. The P_FrameNum halfword (0x4869f4) is incremented. When the new value
//     read as a signed halfword is -32768, P_FrameNum is stored as zero first
//     and then frameNumWrapped__Fv runs (store before call).
//  3. With pushLevel (0x484304) zero, updateTrails__Fv runs.
//  4. updatePS__FP14ParticleSystem runs on globalSystem (0x37f730).
//  5. With cutMode (0x484318) zero, each of the 16 PS records (0x6f5ad0, stride
//     312) whose word at +304 is nonzero is updated. A nonzero result stores
//     zero at +308. A zero result adds one to +308, and when the old value was
//     31 or more the record's +304 value goes to P_DeleteParticleSystem__Fi.
// The original wraps this in a loop counter that starts at one and is only
// decremented, so the body runs once.
extern "C" {
extern unsigned char particlesPaused;
extern unsigned char objectDrawEnable;
extern unsigned char cutMode;
extern unsigned char pushLevel[];
extern unsigned char globalSystem[];
extern unsigned char PS[];
extern unsigned short P_FrameNum;
int gameIsPaused__Fv();
void frameNumWrapped__Fv();
void updateTrails__Fv();
int updatePS__FP14ParticleSystem(void* system);
void P_DeleteParticleSystem__Fi(int system);
}

namespace {
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
}

extern "C" void P_Update__Fv() {
    if (particlesPaused != 0) return;
    if (objectDrawEnable == 0) return;
    if (gameIsPaused__Fv() != 0) return;
    const unsigned short next = static_cast<unsigned short>(P_FrameNum + 1u);
    if (static_cast<short>(next) == -32768) {
        P_FrameNum = 0;
        frameNumWrapped__Fv();
    } else {
        P_FrameNum = next;
    }
    if (read_word(pushLevel) == 0u) updateTrails__Fv();
    updatePS__FP14ParticleSystem(globalSystem);
    if (cutMode != 0) return;
    for (unsigned index = 0u; index < 16u; ++index) {
        unsigned char* record = PS + index * 312u;
        if (read_word(record + 304u) == 0u) continue;
        if (updatePS__FP14ParticleSystem(record) != 0) {
            write_word(record + 308u, 0u);
        } else {
            const unsigned idle = read_word(record + 308u);
            write_word(record + 308u, idle + 1u);
            if (static_cast<int>(idle) >= 31) P_DeleteParticleSystem__Fi(static_cast<int>(read_word(record + 304u)));
        }
    }
}
