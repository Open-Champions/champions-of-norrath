// Host contract for P_Update__Fv. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char particlesPaused = 0;
unsigned char objectDrawEnable = 1;
unsigned char cutMode = 0;
unsigned char pushLevel[4] = {0};
unsigned char globalSystem[256] = {0};
unsigned char PS[16 * 312] = {0};
unsigned short P_FrameNum = 0;
void P_Update__Fv();
}

static int failures = 0;
static int order = 0;
static int paused_result = 0;
static int wrapped_at = 0, trails_at = 0, global_at = 0;
static unsigned short frame_at_wrap = 0xffff;
static int ps_calls[16];
static int ps_order[16];
static int ps_result[16];
static int deleted[16], delete_count = 0;
static int ps_total = 0;

static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
static unsigned word(const void* base, unsigned offset) {
    unsigned v;
    std::memcpy(&v, static_cast<const unsigned char*>(base) + offset, 4);
    return v;
}
static void set_word(void* base, unsigned offset, unsigned v) {
    std::memcpy(static_cast<unsigned char*>(base) + offset, &v, 4);
}
extern "C" int gameIsPaused__Fv() { return paused_result; }
extern "C" void frameNumWrapped__Fv() { wrapped_at = ++order; frame_at_wrap = P_FrameNum; }
extern "C" void updateTrails__Fv() { trails_at = ++order; }
extern "C" int updatePS__FP14ParticleSystem(void* system) {
    ++ps_total;
    if (system == globalSystem) {
        global_at = ++order;
        return 0;
    }
    const int index = static_cast<int>((static_cast<unsigned char*>(system) - PS) / 312);
    ps_calls[index] += 1;
    ps_order[index] = ++order;
    return ps_result[index];
}
extern "C" void P_DeleteParticleSystem__Fi(int system) { deleted[delete_count++] = system; }

static void reset() {
    particlesPaused = 0;
    objectDrawEnable = 1;
    cutMode = 0;
    paused_result = 0;
    std::memset(PS, 0, sizeof PS);
    std::memset(pushLevel, 0, sizeof pushLevel);
    std::memset(ps_calls, 0, sizeof ps_calls);
    std::memset(ps_result, 0, sizeof ps_result);
    delete_count = 0;
    order = wrapped_at = trails_at = global_at = ps_total = 0;
    frame_at_wrap = 0xffff;
    P_FrameNum = 5;
}

int main() {
    // Gates.
    reset(); particlesPaused = 1; P_Update__Fv();
    expect(P_FrameNum == 5 && ps_total == 0, "particlesPaused stops everything");
    reset(); objectDrawEnable = 0; P_Update__Fv();
    expect(P_FrameNum == 5 && ps_total == 0, "objectDrawEnable zero stops everything");
    reset(); paused_result = 1; P_Update__Fv();
    expect(P_FrameNum == 5 && ps_total == 0, "gameIsPaused stops everything");

    // Normal frame: frame number, trails, global system, in that order.
    reset();
    set_word(PS + 2 * 312, 304, 11u);
    ps_result[2] = 1;
    set_word(PS + 2 * 312, 308, 9u);
    P_Update__Fv();
    expect(P_FrameNum == 6, "frame number advances");
    expect(trails_at == 1 && global_at == 2 && ps_order[2] == 3, "trails, then global system, then records");
    expect(word(PS + 2 * 312, 308) == 0u, "an active record clears its idle count");
    expect(ps_calls[0] == 0 && ps_calls[1] == 0 && ps_calls[3] == 0, "records with no +304 word are skipped");
    expect(delete_count == 0, "no deletion");

    // Idle record counts up and is deleted once the old count is 31.
    reset();
    set_word(PS + 4 * 312, 304, 77u);
    set_word(PS + 4 * 312, 308, 30u);
    P_Update__Fv();
    expect(word(PS + 4 * 312, 308) == 31u && delete_count == 0, "idle count 30 becomes 31 without deletion");
    P_Update__Fv();
    expect(word(PS + 4 * 312, 308) == 32u && delete_count == 1 && deleted[0] == 77, "idle count 31 deletes system 77");

    // pushLevel nonzero skips the trails; cutMode nonzero skips the records.
    reset();
    set_word(pushLevel, 0, 1u);
    set_word(PS, 304, 5u);
    cutMode = 1;
    P_Update__Fv();
    expect(trails_at == 0, "trails skipped while a level is pushed");
    expect(global_at == 1 && ps_calls[0] == 0, "global system still updates, records do not, in cut mode");

    // Wrap: the new value -32768 stores zero first and then calls frameNumWrapped.
    reset();
    P_FrameNum = 0x7fff;
    P_Update__Fv();
    expect(wrapped_at == 1 && frame_at_wrap == 0u && P_FrameNum == 0u, "wrap stores zero before the call");
    expect(trails_at == 2, "trails follow the wrap call");
    reset();
    P_FrameNum = 0xffff;   // 65535 is -1; next is 0, not a wrap
    P_Update__Fv();
    expect(wrapped_at == 0 && P_FrameNum == 0u, "no wrap call for 0xffff to 0");
    if (failures != 0) return 1;
    std::printf("p-update contract passed\n");
    return 0;
}
