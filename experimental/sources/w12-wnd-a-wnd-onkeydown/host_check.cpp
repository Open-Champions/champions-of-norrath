// Host check only; not a build input. The class table is laid out as in the target (8-byte
// entries: halfword adjustment, halfword index, function word), so the parent's OnKeyDown entry
// is the one at byte offset 40. Checks the McErr forward, the parent call with its adjustment, the
// no-parent return, and that neighbouring entries are not used.
#define NORRATH_HOST_ARENA 1
#include "onkeydown.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

unsigned char sWndMcErr[0x210];
static std::string trace;
static void parent_key(void* self, int key) { trace += "parent:" + std::to_string(hw::guest(self) - hw::BASE) + "," + std::to_string(key) + ";"; }
static void wrong_key(void*, int) { trace += "WRONG;"; }
extern "C" void OnKeyDown__10VIWndMcErri(void* self, int key) {
    trace += std::string("mcerr:") + (self == sWndMcErr ? "E" : "?") + "," + std::to_string(key) + ";";
}

int main() {
    const unsigned child = hw::BASE + 0x100, parent = hw::BASE + 0x400, table = hw::BASE + 0x800;
    hw::clear();
    for (int slot = 0; slot < 8; ++slot) hw::put_entry(table + 8 * slot, 3, static_cast<short>(slot), reinterpret_cast<void*>(wrong_key));
    hw::put_entry(table + 40, 24, 5, reinterpret_cast<void*>(parent_key));
    hw::w(parent + 32) = table;
    hw::w(child + 20) = parent;

    trace.clear();
    OnKeyDown__5VIWndi(hw::at(child), 65);
    hw::check(trace == "parent:" + std::to_string(parent - hw::BASE + 24) + ",65;", "parent call");

    trace.clear();
    OnKeyDown__5VIWndi(hw::at(child), -3);
    hw::check(trace == "parent:" + std::to_string(parent - hw::BASE + 24) + ",-3;", "negative key");

    hw::put_entry(table + 40, -8, 5, reinterpret_cast<void*>(parent_key));
    trace.clear();
    OnKeyDown__5VIWndi(hw::at(child), 1);
    hw::check(trace == "parent:" + std::to_string(parent - hw::BASE - 8) + ",1;", "negative adjustment");

    std::memset(sWndMcErr, 0, sizeof sWndMcErr);
    std::memcpy(sWndMcErr + 20, &parent, 4);
    trace.clear();
    OnKeyDown__5VIWndi(sWndMcErr, 77);
    hw::check(trace == "mcerr:E,77;", "mcerr");

    hw::w(child + 20) = 0;
    trace.clear();
    OnKeyDown__5VIWndi(hw::at(child), 9);
    hw::check(trace.empty(), "no parent");
    return hw::finish();
}
