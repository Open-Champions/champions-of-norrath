#define NORRATH_VSLOTS_B_HOST 1
#include "msg_run.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0, order_count = 0, order[4];
static void *seen_state, *seen_point, *seen_second, *seen_object;
static short* seen_last;
static int seen_value, seen_limit;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* point, void* second, short* last) {
    order[order_count++] = 1; seen_state = state; seen_point = point; seen_second = second; seen_last = last;
}
extern "C" int objectActiveProcessing__FP10GameObjectii(void* object, int value, int limit) {
    order[order_count++] = 2; seen_object = object; seen_value = value; seen_limit = limit;
    return value + 1000;
}
int main() {
    unsigned char object[360];
    std::memset(object, 0, sizeof object);
    vslots_b::write_word(vslots_b::address(object) + 348u, 0xfffffffeu);
    msg_run__13UserParamProp(object);
    expect(order_count == 2 && order[0] == 1 && order[1] == 2, "animation then processing");
    expect(seen_state == object + 84 && seen_point == object + 16, "state and point");
    expect(seen_second == nullptr && seen_last == nullptr, "null arguments");
    expect(seen_object == object && seen_value == -2 && seen_limit == 480, "processing arguments");
    expect(vslots_b::read_word(vslots_b::address(object) + 348u) == 998u, "result stored at object+348");
    return failures == 0 ? 0 : 1;
}
