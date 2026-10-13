// Host check only; not a build input. Defines NORRATH_HOST_CONTRACT so the 32-bit parent and table
// words translate to host objects through a tag table. Checks the McErr tail call, forwarding to a
// parent's table entry (with its adjustment and key), and the no-parent return.
#define NORRATH_HOST_CONTRACT 1
#include "onkeyup.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char sWndMcErr[0x210];
static unsigned char parent_object[128];
static VtEntry parent_table[8];
static std::string trace;
static const unsigned TAG_PARENT = 0x1000, TAG_TABLE = 0x2000;

extern "C" char* norrath_host_translate(unsigned address) {
    switch (address) {
    case TAG_PARENT: return reinterpret_cast<char*>(parent_object);
    case TAG_TABLE: return reinterpret_cast<char*>(parent_table);
    default: return nullptr;
    }
}
extern "C" void OnKeyUp__10VIWndMcErri(void* self, int key) {
    trace += std::string("mcerr:") + (self == sWndMcErr ? "E" : "?") + "," + std::to_string(key) + ";";
}
static void parent_key(void* self, int key) {
    trace += "parent:" + std::to_string(static_cast<unsigned char*>(self) - parent_object) + "," + std::to_string(key) + ";";
}

int main() {
    bool ok = true;
    static unsigned char child[128];
    parent_table[4] = VtEntry{24, 4, parent_key};
    parent_table[3] = VtEntry{99, 3, nullptr};
    parent_table[5] = VtEntry{98, 5, nullptr};
    std::memcpy(parent_object + 32, &TAG_TABLE, 4);

    // McErr object: the parent word is ignored.
    std::memset(sWndMcErr, 0, sizeof sWndMcErr);
    std::memcpy(sWndMcErr + 20, &TAG_PARENT, 4);
    trace.clear();
    OnKeyUp__5VIWndi(sWndMcErr, 77);
    ok = ok && trace == "mcerr:E,77;";

    // Ordinary window with a parent.
    std::memset(child, 0, sizeof child);
    std::memcpy(child + 20, &TAG_PARENT, 4);
    trace.clear();
    OnKeyUp__5VIWndi(child, -5);
    ok = ok && trace == "parent:24,-5;";

    // No parent.
    std::memset(child + 20, 0, 4);
    trace.clear();
    OnKeyUp__5VIWndi(child, 3);
    ok = ok && trace.empty();
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
