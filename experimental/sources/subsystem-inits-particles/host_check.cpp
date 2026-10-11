// Host contract for P_Init__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" void P_Init__Fv();
extern "C" void P_Update__Fv();
extern "C" void drawParticlesTask__Fv();
extern "C" alignas(16) unsigned char wigglyTab[544] = {};
extern "C" unsigned char descListMark[128] = {};
extern "C" alignas(16) unsigned char block[128 * 4096] = {};
extern "C" unsigned char invisible[80] = {};
extern "C" unsigned nmSingle = 0;
extern "C" unsigned nmLines = 0;
extern "C" unsigned short P_FrameNum = 0;

namespace {
int failures = 0;
int sine_calls = 0;
int cosine_calls = 0;
int angles[64];
int phase = 0;
int tasks = 0;
void (*task_fn[2])();
int task_order[2];
int task_flag[2];
const char* task_name[2];

void expect(bool condition, const char* message) {
    if (!condition) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

unsigned load_word(const unsigned char* slot) {
    return static_cast<unsigned>(slot[0]) | (static_cast<unsigned>(slot[1]) << 8) |
           (static_cast<unsigned>(slot[2]) << 16) | (static_cast<unsigned>(slot[3]) << 24);
}
}

extern "C" float isin__Fi(int angle) {
    if (sine_calls < 64) angles[sine_calls] = angle;
    ++sine_calls;
    return angle == 0 ? -1.0f : 2.0f;
}
extern "C" float icos__Fi(int angle) {
    ++cosine_calls;
    expect(angle == (cosine_calls - 1) * 1024, "cosine angle");
    return angle == 0 ? 1.0f : -1.0f;
}
extern "C" void allocDescriptor__FP11ParticleDef(unsigned char* descriptor) {
    expect(phase == 0, "descriptor phase");
    expect(descriptor == invisible, "invisible descriptor");
    expect(block[0] == 0xa5, "block not filled yet");
    expect(sine_calls == 64 && cosine_calls == 64, "angles before descriptor");
    phase = 1;
}
extern "C" void P_Clear__Fv() {
    expect(phase == 1, "clear phase");
    expect(load_word(block + 8) == 0x04000240u, "block filled before clear");
    phase = 2;
}
extern "C" void initTrails__Fv() {
    expect(phase == 2, "trails phase");
    phase = 3;
}
extern "C" void initParticleSystems__Fv() {
    expect(phase == 3, "systems phase");
    phase = 4;
}
extern "C" void P_Update__Fv() {}
extern "C" void drawParticlesTask__Fv() {}
extern "C" void engineAddTask__FPFv_viPci(void (*task)(), int order, char* name, int flag) {
    expect(phase == 4, "tasks after systems");
    expect(tasks < 2, "task capacity");
    if (tasks >= 2) return;
    task_fn[tasks] = task;
    task_order[tasks] = order;
    task_flag[tasks] = flag;
    task_name[tasks] = name;
    ++tasks;
}

int main() {
    std::memset(wigglyTab, 0x3c, sizeof wigglyTab);
    std::memset(descListMark, 0x5a, sizeof descListMark);
    std::memset(block, 0xa5, sizeof block);
    std::memset(invisible, 0x11, sizeof invisible);
    nmSingle = 9;
    nmLines = 8;
    P_FrameNum = 7;
    P_Init__Fv();

    expect(sine_calls == 64 && cosine_calls == 64, "trig calls");
    for (int index = 0; index < 64; ++index) expect(angles[index] == index * 1024, "sine angle");
    expect(load_word(wigglyTab) == 0xfffffffbu, "first sine");
    expect(load_word(wigglyTab + 4) == 5u, "first cosine");
    expect(load_word(wigglyTab + 8) == 10u, "second sine");
    expect(load_word(wigglyTab + 12) == 0xfffffffbu, "second cosine");
    for (int byte = 512; byte < 544; ++byte) expect(wigglyTab[byte] == 0x3c, "wiggly tail");
    expect(nmSingle == 0 && nmLines == 0 && P_FrameNum == 0, "cleared counters");
    for (int word = 0; word < 32; ++word) expect(load_word(descListMark + word * 4) == 0xffffffffu, "mark");

    for (int page = 0; page < 128; ++page) {
        unsigned toggle = 0;
        for (int record = 0; record < 256; ++record) {
            const unsigned char* bytes = block + page * 4096 + record * 16;
            if ((record & 31) == 0) {
                const unsigned value = toggle == 0 ? 576u : 128u;
                toggle ^= 1u;
                expect(load_word(bytes) == 0 && load_word(bytes + 4) == 0, "header zeros");
                expect(load_word(bytes + 8) == (value | 0x04000000u), "header low");
                expect(load_word(bytes + 12) == (value | 0x6d3c0000u), "header high");
            } else if ((record & 31) == 31) {
                expect(load_word(bytes) == 0x14000018u, "terminator");
                expect(load_word(bytes + 4) == 0 && load_word(bytes + 8) == 0 && load_word(bytes + 12) == 0,
                       "terminator tail");
            } else {
                expect(bytes[14] == 0 && bytes[15] == 0, "middle half");
                for (int byte = 0; byte < 14; ++byte) expect(bytes[byte] == 0xa5, "middle kept");
            }
        }
    }
    for (unsigned char byte : invisible) expect(byte == 0x11, "invisible untouched");
    expect(phase == 4 && tasks == 2, "call tail");
    expect(task_fn[0] == P_Update__Fv && task_order[0] == 15 && task_flag[0] == 2, "update task");
    expect(task_name[0] != nullptr && std::strcmp(task_name[0], "updateParticles") == 0, "update name");
    expect(task_fn[1] == drawParticlesTask__Fv && task_order[1] == 65 && task_flag[1] == 1, "draw task");
    expect(task_name[1] != nullptr && std::strcmp(task_name[1], "drawParticles") == 0, "draw name");
    if (failures != 0) {
        std::fprintf(stderr, "%d failures\n", failures);
        return 1;
    }
    return 0;
}
