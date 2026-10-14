/* Host contract for topThread. Not an EE plan input. The loop never returns,
 * so the fake WaitSema throws after a set number of waits. */
#include "top_thread.cpp"

#include <cstdio>
#include <cstring>
#include <string>

extern "C" int topSema = 0x1234;
static int waits_allowed;
static int waits_seen;
static int semaphore_seen;
static std::string log_text;
struct Stop {};

extern "C" int WaitSema(int semaphore) {
    semaphore_seen = semaphore;
    log_text += 'W';
    if (waits_seen++ >= waits_allowed) throw Stop();
    return 0;
}
extern "C" int WakeupThread(int thread) { log_text += 'U' + std::to_string(thread) + ';'; return 0; }
extern "C" int RotateThreadReadyQueue(int value) { log_text += 'R' + std::to_string(value) + ';'; return 0; }
extern "C" int SuspendThread(int thread) { log_text += 'S' + std::to_string(thread) + ';'; return 0; }
extern "C" void kprintf(const char* format, ...) {
    log_text += 'P';
    log_text += '[';
    log_text += format;
    log_text += ']';
}

alignas(8) static unsigned char ring_bytes[1080];

static void request(unsigned slot, unsigned char kind, unsigned char thread) {
    ring_bytes[8 + slot * 2] = kind;
    ring_bytes[8 + slot * 2 + 1] = thread;
}
static unsigned& cursor() { return *reinterpret_cast<unsigned*>(ring_bytes); }

static void run(int waits) {
    waits_allowed = waits;
    waits_seen = 0;
    log_text.clear();
    try {
        topThread(reinterpret_cast<unsigned*>(ring_bytes));
    } catch (const Stop&) {
    }
}

int main() {
    std::memset(ring_bytes, 0xee, sizeof ring_bytes);
    cursor() = 0;
    request(0, 0, 7);
    request(1, 1, 8);
    request(2, 2, 9);
    request(3, 3, 10);
    request(4, 0xff, 11);
    run(5);
    if (log_text != "WU7;WR8;WS9;WP[## internel error in libkernl.a!\n]WP[## internel error in libkernl.a!\n]W")
        return 1;
    if (semaphore_seen != 0x1234) return 2;
    if (cursor() != 5) return 3;
    // The loop waits before it reads: with no wait allowed nothing is consumed.
    cursor() = 0;
    run(0);
    if (log_text != "W" || cursor() != 0) return 4;
    // The cursor is masked to 511 on read and stored back as index + 1.
    cursor() = 511;
    request(511, 0, 21);
    request(0, 2, 22);
    run(2);
    if (log_text != "WU21;WS22;W" || cursor() != 1) return 5;
    // Only the low nine bits of the cursor choose the slot.
    cursor() = 0xfffffe03u;
    request(3, 1, 33);
    run(1);
    if (log_text != "WR33;W" || cursor() != 4) return 6;
    // The bytes past the queue and the other cursor word are never touched.
    unsigned char before[1080];
    std::memcpy(before, ring_bytes, sizeof before);
    cursor() = 0;
    run(3);
    unsigned char after[1080];
    std::memcpy(after, ring_bytes, sizeof after);
    before[0] = after[0] = 0;
    if (std::memcmp(before, after, sizeof before) != 0) return 7;
    std::puts("top thread contract passed");
    return 0;
}
