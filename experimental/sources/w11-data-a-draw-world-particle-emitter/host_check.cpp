// Host contract for drawWorldParticleEmitter__FP13PolysetHeader. Not an EE plan
// input. Build with -DNORRATH_HOST_CONTRACT: the header's data word holds the low
// 32 bits of a host pointer and norrath_host_pointer maps it back.
#include <cstdio>
#include <cstring>

struct Point3 { float x, y, z; };
extern "C" {
unsigned char ParticleEmitterArray[16 * 152];
int frameNm = 123;
void drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(
    Point3 p, void* def, void* data, int frame, short angle, bool a, bool b, bool c);
void drawWorldParticleEmitter__FP13PolysetHeader(void* header);
}

extern "C" void* norrath_host_pointer(unsigned address) {
    const unsigned long base = reinterpret_cast<unsigned long>(&frameNm);
    return reinterpret_cast<void*>((base & ~0xffffffffUL) | address);
}
static Point3 seen_pos; static void* seen_def; static void* seen_data; static int seen_frame, seen_angle, seen_flags;
static signed char next_flag; static int next_word;
extern "C" void drawParticleEmitter__FG6Point3P11ParticleDefP19ParticleEmitterDataisbN25(
    Point3 p, void* def, void* data, int frame, short angle, bool a, bool b, bool c) {
    seen_pos = p; seen_def = def; seen_data = data; seen_frame = frame; seen_angle = angle;
    seen_flags = (a ? 1 : 0) | (b ? 2 : 0) | (c ? 4 : 0);
    std::memcpy(static_cast<unsigned char*>(data) + 56, &next_word, 4);
    static_cast<signed char*>(data)[60] = next_flag;
}

int main() {
    alignas(8) static unsigned char data[128];
    // The mapping assumes data shares the upper address half of frameNm.
    alignas(8) static unsigned char header[64];
    unsigned data_word = static_cast<unsigned>(reinterpret_cast<unsigned long>(data));
    std::memcpy(header, &data_word, 4);
    unsigned w4 = 0x80000003u; std::memcpy(header + 4, &w4, 4);
    int x = 8, y = -4, z = 2, flags = 0x50000, w32 = 99;
    std::memcpy(header + 40, &x, 4); std::memcpy(header + 44, &y, 4); std::memcpy(header + 48, &z, 4);
    std::memcpy(header + 52, &flags, 4); std::memcpy(header + 32, &w32, 4);
    next_flag = 0; next_word = 55;
    drawWorldParticleEmitter__FP13PolysetHeader(header);
    if (seen_pos.x != 2.0f || seen_pos.y != -1.0f || seen_pos.z != 0.5f) return 1;
    if (seen_def != ParticleEmitterArray + 3 * 152 + 72 || seen_data != static_cast<void*>(data)) return 2;
    if (seen_frame != 123 || seen_angle != 0 || seen_flags != 5) return 3;
    int out32; std::memcpy(&out32, header + 32, 4); std::memcpy(&w4, header + 4, 4);
    if (out32 != 55 || w4 != 3u) return 4;
    next_flag = -1;
    drawWorldParticleEmitter__FP13PolysetHeader(header);
    std::memcpy(&w4, header + 4, 4);
    if (w4 != 0x80000003u) return 5;
    return 0;
}
