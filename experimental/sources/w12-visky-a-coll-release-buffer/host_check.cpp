// Host check only; not a build input. Native build: c++ -std=c++17 -ffp-contract=off host_check.cpp
// Entry and buffer addresses are 32-bit words mapped onto an arena whose base is address 0, so the
// zero-entry case really reads and writes address 4, as the target routine does.
#define NORRATH_HOST_ARENA
#include <cstring>
#include <string>
static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "release_coll_buffer.cpp"

static std::string trace;
static int erase_result;
extern "C" {
int Erase__t6VIPool1ZP12VICollBufferi(unsigned char* pool, int index) {
    trace += "erase:" + std::to_string(pool - arena) + "," + std::to_string(index) + ";";
    return erase_result;
}
void _$_12VICollBuffer(unsigned char* buffer, int flags) { trace += "dtor:" + std::to_string(buffer - arena) + "," + std::to_string(flags) + ";"; }
}

static void sw(unsigned off, int v) { std::memcpy(arena + off, &v, 4); }
static int rw(unsigned off) { int v; std::memcpy(&v, arena + off, 4); return v; }
// Collide object at 0x100 (pool at 0x100+528, entry array pointer at pool+8), entries at 0x2000,
// buffer at 0x3000 whose count is at +4.
static void arm(int index, unsigned entry_word, int count) {
    std::memset(arena, 0x11, sizeof arena);
    sw(0x100 + 528 + 8, 0x2000);
    sw(0x2000 + 12 * index, static_cast<int>(entry_word));
    sw(0x3000 + 4, count);
    sw(4, 100);
    trace.clear();
}

int main() {
    const std::string pool = std::to_string(0x100 + 528);
    // Count stays positive: only the decrement.
    arm(2, 0x3000, 3);
    if (ReleaseCollBuffer__9VICollidei(arena + 0x100, 2) != 0 || rw(0x3004) != 2 || !trace.empty()) return 1;
    // Count reaches zero: erase, then destroy with flags 3.
    arm(2, 0x3000, 1); erase_result = 4;
    if (ReleaseCollBuffer__9VICollidei(arena + 0x100, 2) != 0 || rw(0x3004) != 0) return 2;
    if (trace != "erase:" + pool + ",2;dtor:12288,3;") return 3;
    // Count below zero also erases.
    arm(1, 0x3000, -4); erase_result = 0;
    if (ReleaseCollBuffer__9VICollidei(arena + 0x100, 1) != 0 || rw(0x3004) != -5 || trace != "erase:" + pool + ",1;dtor:12288,3;") return 4;
    // Failed erase: -1 and no destroy.
    arm(0, 0x3000, 1); erase_result = -1;
    if (ReleaseCollBuffer__9VICollidei(arena + 0x100, 0) != -1 || trace != "erase:" + pool + ",0;") return 5;
    // Zero entry word: the count at address 4 is decremented, the erase happens, nothing is destroyed.
    arm(0, 0, 3); erase_result = 0;
    if (ReleaseCollBuffer__9VICollidei(arena + 0x100, 0) != 0 || rw(4) != 99 || rw(0x3004) != 3) return 6;
    if (!trace.empty()) return 7;
    arm(0, 0, 1); sw(4, 1); erase_result = 0;
    if (ReleaseCollBuffer__9VICollidei(arena + 0x100, 0) != 0 || rw(4) != 0 || trace != "erase:" + pool + ",0;") return 8;
    return 0;
}
