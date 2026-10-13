// Host check for sendAlertMessage__FifG6Point3P10GameObject. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
// Host scope: box construction, the 128 count and the empty-list case. The table
// at object + 72 holds 32-bit EE pointers, which a 64-bit host cannot lay out, so
// the virtual calls are covered only by the private paired run against the image.
#include <cstdint>
#include <vector>
#define sendAlertMessage__FifG6Point3P10GameObject sendAlertMessage_host
#include "send_alert_message.cpp"
#undef sendAlertMessage__FifG6Point3P10GameObject

struct Call { void* self; int kind; float radius; Point3 from; void* sender; };
static std::vector<Call> calls;
static Point3 seen_low, seen_high;
static int seen_count;
static void* fake_list[4];
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** list, int count) {
    seen_low = low; seen_high = high; seen_count = count;
    for (int i = 0; i < 4; ++i) list[i] = fake_list[i];
}
int main() {
    Point3 c = {10.0f, -20.0f, 5.5f};
    fake_list[0] = fake_list[1] = fake_list[2] = fake_list[3] = 0;
    // The null-list case needs no object memory.
    sendAlertMessage_host(3, 2.5f, &c, 0);
    if (seen_count != 128) return 1;
    if (seen_low.x != 7.5f || seen_low.y != -22.5f || seen_low.z != 3.0f) return 2;
    if (seen_high.x != 12.5f || seen_high.y != -17.5f || seen_high.z != 8.0f) return 3;
    if (!calls.empty()) return 4;
    return 0;
}
