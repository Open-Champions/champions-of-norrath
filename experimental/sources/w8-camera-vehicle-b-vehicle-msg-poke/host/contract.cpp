// Host contract for msg_poke__7Vehiclei. Not an EE plan input.
#include <cstdio>
extern "C" unsigned msg_poke__7Vehiclei(void* vehicle, int seat);
static int calls = 0;
static void* seen_vehicle = nullptr;
static int seen_seat = 0;
static unsigned reply = 0;
extern "C" unsigned addPassenger__7Vehiclei(void* vehicle, int seat) {
    ++calls;
    seen_vehicle = vehicle;
    seen_seat = seat;
    return reply;
}
int main() {
    static unsigned char object[16];
    const int seats[3] = {-1, 0, 5};
    const unsigned replies[3] = {0u, 1u, 0x80000001u};
    for (int i = 0; i < 3; ++i) {
        calls = 0;
        reply = replies[i];
        const unsigned got = msg_poke__7Vehiclei(object, seats[i]);
        if (calls != 1) { std::fprintf(stderr, "fail: call count\n"); return 1; }
        if (seen_vehicle != object) { std::fprintf(stderr, "fail: vehicle\n"); return 1; }
        if (seen_seat != seats[i]) { std::fprintf(stderr, "fail: seat\n"); return 1; }
        if (got != replies[i]) { std::fprintf(stderr, "fail: result\n"); return 1; }
    }
    return 0;
}
