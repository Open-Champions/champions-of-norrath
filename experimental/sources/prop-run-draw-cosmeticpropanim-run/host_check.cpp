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
    order[order_count++] = 2; seen_state = state; seen_point = point; seen_second = second; seen_last = last;
}
extern "C" int objectActiveProcessing__FP10GameObjectii(void* object, int value, int limit) {
    order[order_count++] = 1; seen_object = object; seen_value = value; seen_limit = limit;
    return 0x1234;  // only the low byte is stored
}
int main() {
    unsigned char object[160];
    std::memset(object, 0xaa, sizeof object);
    object[140] = 0xf0;
    msg_run__16CosmeticPropAnim(object);
    expect(order_count == 2 && order[0] == 1 && order[1] == 2, "processing then animation");
    expect(seen_object == object && seen_value == -16 && seen_limit == 480, "signed byte and limit");
    expect(object[140] == 0x34 && object[139] == 0xaa && object[141] == 0xaa, "only the low byte is stored");
    expect(seen_state == object + 100 && seen_point == object + 16, "state and point");
    expect(seen_second == nullptr && seen_last == nullptr, "null arguments");
    return failures == 0 ? 0 : 1;
}
