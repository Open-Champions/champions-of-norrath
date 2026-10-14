// Host check only; not a build input. The text pointer word at +424 translates through the arena;
// checks the halfword zeroed through it (and only that halfword) and the SetCaretOffset(this, 0) call.
#define NORRATH_HOST_ARENA 1
#include "cleartext.cpp"
#include "../w16-wnd-a-common/host_world.hpp"

static std::string trace;
extern "C" void SetCaretOffset__9VIWndEditi(void* self, int offset) {
    trace += (self == hw::at(hw::BASE) ? "self:" : "other:") + std::to_string(offset) + ";";
}

int main() {
    const unsigned object = hw::BASE, text = hw::BASE + 0x800;
    for (unsigned fill : {0xffffu, 0x1234u, 0u}) {
        hw::clear();
        for (int i = 0; i < 8; ++i) std::memcpy(hw::at(text) + 2 * i, &fill, 2);
        hw::w(object + 424) = text;
        trace.clear();
        ClearText__9VIWndEdit(hw::at(object));
        unsigned short got[8];
        std::memcpy(got, hw::at(text), sizeof got);
        hw::check(got[0] == 0, "first halfword");
        for (int i = 1; i < 8; ++i) hw::check(got[i] == fill, "rest untouched", i);
        hw::check(hw::w(object + 424) == text, "pointer word kept");
        hw::check(trace == "self:0;", "caret call");
    }
    return hw::finish();
}
