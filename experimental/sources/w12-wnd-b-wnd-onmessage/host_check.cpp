// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the 32-bit parent and table
// words translate to host objects through a tag table. Checks the McErr tail call, forwarding with
// all four trailing arguments and the table adjustment, and the no-parent return.
#define NORRATH_HOST_CONTRACT 1
#include "onmessage.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char sWndMcErr[0x210];
static unsigned char parent_object[128], other[16];
static VtEntry parent_table[8];
static std::string trace;
static const unsigned TAG_PARENT = 0x1000, TAG_TABLE = 0x2000;
static const char* who(const void* p) { return p == other ? "O" : p == sWndMcErr ? "E" : "?"; }

extern "C" char* norrath_host_translate(unsigned address) {
    switch (address) {
    case TAG_PARENT: return reinterpret_cast<char*>(parent_object);
    case TAG_TABLE: return reinterpret_cast<char*>(parent_table);
    default: return nullptr;
    }
}
extern "C" void OnMessage__10VIWndMcErrP5VIWndiii(void* self, void* from, int message, int a, int b) {
    trace += std::string("mcerr:") + who(self) + "," + who(from) + "," + std::to_string(message) + "," + std::to_string(a) + "," + std::to_string(b) + ";";
}
static void parent_message(void* self, void* from, int message, int a, int b) {
    trace += "parent:" + std::to_string(static_cast<unsigned char*>(self) - parent_object) + "," + who(from) + "," +
             std::to_string(message) + "," + std::to_string(a) + "," + std::to_string(b) + ";";
}

int main() {
    bool ok = true;
    static unsigned char child[128];
    parent_table[6] = VtEntry{-16, 6, parent_message};
    parent_table[4] = VtEntry{99, 4, nullptr};
    parent_table[5] = VtEntry{98, 5, nullptr};
    std::memcpy(parent_object + 32, &TAG_TABLE, 4);

    std::memset(sWndMcErr, 0, sizeof sWndMcErr);
    std::memcpy(sWndMcErr + 20, &TAG_PARENT, 4);
    trace.clear();
    OnMessage__5VIWndP5VIWndiii(sWndMcErr, other, 295, 1, 2);
    ok = ok && trace == "mcerr:E,O,295,1,2;";

    std::memset(child, 0, sizeof child);
    std::memcpy(child + 20, &TAG_PARENT, 4);
    trace.clear();
    OnMessage__5VIWndP5VIWndiii(child, other, 295, -7, 9);
    ok = ok && trace == "parent:-16,O,295,-7,9;";

    std::memset(child + 20, 0, 4);
    trace.clear();
    OnMessage__5VIWndP5VIWndiii(child, other, 1, 2, 3);
    ok = ok && trace.empty();
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
