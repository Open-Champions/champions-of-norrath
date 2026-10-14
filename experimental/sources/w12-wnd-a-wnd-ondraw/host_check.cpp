// Host check only; not a build input. Class tables use the target layout (8-byte entries), so the
// window's own draw is the entry at byte offset 24 and each child's draw the entry at 16. The set
// routines are scripted: Begin/Next walk a list of element indices, End always returns 0. Checks the
// exact call order (self, then each element whose visibility word is nonzero, End re-evaluated each pass),
// the 20-byte element stride, the child pointer at element+16, the McErr branch and an empty set.
#define NORRATH_HOST_ARENA 1
#include "ondraw.cpp"
#include "../w16-wnd-a-common/host_world.hpp"
#include <vector>

unsigned char sWndMcErr[0x210];
static std::string trace;
static std::vector<int> script;
static size_t cursor;
static std::string name_of(void* p) {
    if (p == sWndMcErr) return "E";
    return std::to_string(hw::guest(p) - hw::BASE);
}
static void self_draw(void* self) { trace += "self:" + name_of(self) + ";"; }
static void child_draw(void* self) { trace += "child:" + name_of(self) + ";"; }
static void wrong(void*) { trace += "WRONG;"; }
extern "C" void OnDrawSelf__10VIWndMcErr(void* self) { trace += "mcerr:" + name_of(self) + ";"; }
extern "C" int Begin__Ct5VISet1ZP5VIWnd(void* set) {
    trace += "B" + name_of(set) + ";";
    cursor = 0;
    return script.empty() ? 0 : script[0];
}
extern "C" int End__Ct5VISet1ZP5VIWnd(void*) { trace += "E;"; return 0; }
extern "C" int Next__Ct5VISet1ZP5VIWndi(void* set, int it) {
    trace += "N" + name_of(set) + "," + std::to_string(it) + ";";
    return ++cursor < script.size() ? script[cursor] : 0;
}

int main() {
    const unsigned window = hw::BASE + 0x100, table = hw::BASE + 0x800, elements = hw::BASE + 0x2000;
    const unsigned child_table = hw::BASE + 0x900;
    hw::clear();
    for (int slot = 0; slot < 8; ++slot) {
        hw::put_entry(table + 8 * slot, 1, static_cast<short>(slot), reinterpret_cast<void*>(wrong));
        hw::put_entry(child_table + 8 * slot, 1, static_cast<short>(slot), reinterpret_cast<void*>(wrong));
    }
    hw::put_entry(table + 24, 16, 3, reinterpret_cast<void*>(self_draw));
    hw::put_entry(child_table + 16, 8, 2, reinterpret_cast<void*>(child_draw));
    hw::w(window + 32) = table;
    hw::w(window + 8) = elements;
    const unsigned children[3] = {hw::BASE + 0x3000, hw::BASE + 0x3200, hw::BASE + 0x3400};
    const unsigned visible[3] = {1, 0, 0x80000000u};
    for (int n = 0; n < 3; ++n) {
        hw::w(children[n] + 32) = child_table;
        hw::w(children[n] + 28) = visible[n];
        hw::w(elements + (n + 1) * 20 + 16) = children[n];
        hw::w(elements + (n + 1) * 20 + 12) = 0xdead;  // other element words are not child pointers
    }
    auto num = [](unsigned v) { return std::to_string(v); };

    script = {1, 2, 3};
    trace.clear();
    OnDraw__5VIWnd(hw::at(window));
    const std::string expected = "self:" + num(window - hw::BASE + 16) + ";B" + num(window - hw::BASE) + ";E;child:" +
                                 num(children[0] - hw::BASE + 8) + ";N" + num(window - hw::BASE) + ",1;E;N" +
                                 num(window - hw::BASE) + ",2;E;child:" + num(children[2] - hw::BASE + 8) + ";N" +
                                 num(window - hw::BASE) + ",3;E;";
    hw::check(trace == expected, "full walk");
    if (trace != expected) std::printf("%s\n%s\n", trace.c_str(), expected.c_str());

    script = {3};
    trace.clear();
    OnDraw__5VIWnd(hw::at(window));
    hw::check(trace == "self:" + num(window - hw::BASE + 16) + ";B" + num(window - hw::BASE) + ";E;child:" +
                           num(children[2] - hw::BASE + 8) + ";N" + num(window - hw::BASE) + ",3;E;", "single");

    script.clear();
    trace.clear();
    OnDraw__5VIWnd(hw::at(window));
    hw::check(trace == "self:" + num(window - hw::BASE + 16) + ";B" + num(window - hw::BASE) + ";E;", "empty");

    // The McErr global draws through its own routine instead of the table entry.
    std::memset(sWndMcErr, 0, sizeof sWndMcErr);
    script.clear();
    trace.clear();
    OnDraw__5VIWnd(sWndMcErr);
    hw::check(trace == "mcerr:E;BE;E;", "mcerr");
    return hw::finish();
}
