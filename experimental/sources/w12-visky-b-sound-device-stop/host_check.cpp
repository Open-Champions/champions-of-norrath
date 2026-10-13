static void* host_sound;
#define NORRATH_WORD_ADDRESS(word) (((word) == 0x7001u) ? host_sound : static_cast<void*>(0))
#include "stop.cpp"

#include <cstring>
#include <vector>

extern "C" int norrath_overlay_sIopFunctionData_11edf08[128];
int norrath_overlay_sIopFunctionData_11edf08[128];

struct Ev { int kind; long a; long long b; };
static std::vector<Ev> ev;
static void* playback_result;
static int type_calls;
static int type_results[2];
static unsigned char sound_object[16];
static unsigned char device_object[4096];
static unsigned char pb_object[64];
static int iop_data_at_call[2];

extern "C" void* Playback__13VISoundDevicei(void* d, int id) { ev.push_back({1, (long)id, (long)(d == device_object)}); return playback_result; }
extern "C" int Type__C7VISound(const void* s) {
    ev.push_back({2, s == sound_object, 0});
    return type_results[type_calls++ > 0 ? 1 : 0];
}
extern "C" void SetIopFunction__FUii(unsigned f, int n) {
    iop_data_at_call[0] = norrath_overlay_sIopFunctionData_11edf08[0];
    iop_data_at_call[1] = norrath_overlay_sIopFunctionData_11edf08[1];
    ev.push_back({3, (long)f, n});
}
extern "C" void StopChannels__13VISoundDeviceUl(void* d, unsigned long long c) { ev.push_back({4, d == device_object, (long long)c}); }
extern "C" void DeallocateChannels__13VISoundDeviceUl(void* d, unsigned long long c) { ev.push_back({5, d == device_object, (long long)c}); }
extern "C" void DeletePlayback__13VISoundDeviceP10VIPlayback(void* d, void* p) {
    ev.push_back({6, d == device_object, p == pb_object});
}

static int failures;
static void expect(bool ok, int code) { if (!ok) { failures = failures ? failures : code; } }

static unsigned slot_word(int index) { unsigned v; std::memcpy(&v, device_object + 3100 + index * 4, 4); return v; }

static void run(long long channels, int slot, int t0, int t1, unsigned sound_word, int id, unsigned char key) {
    std::memset(device_object, 0, sizeof device_object);
    std::memset(pb_object, 0, sizeof pb_object);
    std::memset(norrath_overlay_sIopFunctionData_11edf08, 0x77, 16);
    for (int i = 0; i < 46; ++i) std::memcpy(device_object + 3100 + i * 4, "\xaa\xbb\xcc\xdd", 4);
    std::memcpy(pb_object + 16, &channels, 8);
    std::memcpy(pb_object + 28, &slot, 4);
    pb_object[32] = key;
    if (slot != -1) std::memcpy(device_object + 2584 + slot * 4, &sound_word, 4);
    host_sound = sound_object;
    type_calls = 0; type_results[0] = t0; type_results[1] = t1;
    ev.clear();
    playback_result = pb_object;
    Stop__13VISoundDevicei(device_object, id);
}

int main() {
    // Type 1 sound: IOP call with the mask split.
    long long mask = 0x0123456789abcdefll;
    run(mask, 3, 1, 1, 0x7001u, 0x1234, 7);
    expect(ev.size() == 7, 1);
    expect(ev[0].kind == 1 && ev[0].a == 0x1234 && ev[0].b == 1, 2);
    expect(ev[1].kind == 2 && ev[2].kind == 2 && ev[3].kind == 3 && ev[3].a == 9 && ev[3].b == 2, 3);
    expect(iop_data_at_call[0] == (int)(mask & 0xffffff), 4);
    expect(iop_data_at_call[1] == (int)(mask << 8 >> 32), 5);
    expect(ev[4].kind == 4 && ev[4].b == mask && ev[5].kind == 5 && ev[5].b == mask, 6);
    expect(ev[6].kind == 6 && ev[6].a == 1 && ev[6].b == 1, 7);
    expect(slot_word(7 % 46) == 0, 8);
    expect(slot_word(6) == 0xddccbbaau && slot_word(8) == 0xddccbbaau, 9);
    // Sign comes from bit 55.
    long long neg = (long long)0x0080000000123456ll;
    run(neg, 0, 1, 1, 0x7001u, 1, 50);
    expect(iop_data_at_call[1] == (int)(neg << 8 >> 32) && iop_data_at_call[1] < 0, 10);
    expect(iop_data_at_call[0] == 0x123456, 11);
    expect(slot_word(50 % 46) == 0, 12);
    // Type other than 1: no IOP call, Type called twice.
    run(mask, 2, 5, 5, 0x7001u, 9, 1);
    expect(ev.size() == 6 && ev[1].kind == 2 && ev[2].kind == 2 && ev[3].kind == 4, 13);
    // Type zero: queried once.
    run(mask, 2, 0, 1, 0x7001u, 9, 1);
    expect(ev.size() == 5 && ev[1].kind == 2 && ev[2].kind == 4, 14);
    // No sound (slot -1), or a null sound word.
    run(mask, -1, 1, 1, 0x7001u, 9, 1);
    expect(ev.size() == 4 && ev[1].kind == 4, 15);
    run(mask, 4, 1, 1, 0, 9, 1);
    expect(ev.size() == 4 && ev[1].kind == 4, 16);
    // Null playback: nothing else happens.
    std::memset(device_object, 0, sizeof device_object);
    ev.clear(); playback_result = 0;
    Stop__13VISoundDevicei(device_object, 5);
    expect(ev.size() == 1, 17);
    return failures;
}
