// Host check only; not a build input. Looks up every present key and a set of absent ones in
// random search trees and compares with the element that holds the key, or the End result (0).
#define NORRATH_HOST_ARENA 1
#include "find.cpp"
#include "../w16-wnd-a-common/set_tree.hpp"
#include <algorithm>

extern "C" int End__Ct5VISet1ZP5VIWnd(const void*) { return 0; }

int main() {
    hw::Rng rng(13);
    unsigned key_cell = 5;
    hw::reset_set();
    hw::check(Find__Ct5VISet1ZP5VIWndRCP5VIWnd(hw::at(hw::SET), &key_cell) == 0, "empty");
    for (int round = 0; round < 200; ++round) {
        std::vector<unsigned> keys;
        const int count = 1 + static_cast<int>(rng.next() % 40);
        for (int n = 0; n < count; ++n) {
            unsigned k;
            do k = rng.next() % 1000; while (std::find(keys.begin(), keys.end(), k) != keys.end());
            keys.push_back(k);
        }
        hw::build_plain(keys);
        for (size_t n = 0; n < keys.size(); ++n) {
            key_cell = keys[n];
            hw::check(Find__Ct5VISet1ZP5VIWndRCP5VIWnd(hw::at(hw::SET), &key_cell) == static_cast<int>(n) + 1, "present", round, static_cast<long>(n));
        }
        for (int probe = 0; probe < 20; ++probe) {
            key_cell = rng.next() % 1200;
            const auto it = std::find(keys.begin(), keys.end(), key_cell);
            const int want = it == keys.end() ? 0 : static_cast<int>(it - keys.begin()) + 1;
            hw::check(Find__Ct5VISet1ZP5VIWndRCP5VIWnd(hw::at(hw::SET), &key_cell) == want, "probe", round, key_cell);
        }
        // Keys above 2^31 must compare unsigned.
    }
    hw::build_plain({0x80000000u, 5, 0xffffffffu, 0x7fffffffu});
    for (unsigned n = 0; n < 4; ++n) {
        key_cell = hw::key(static_cast<int>(n) + 1);
        hw::check(Find__Ct5VISet1ZP5VIWndRCP5VIWnd(hw::at(hw::SET), &key_cell) == static_cast<int>(n) + 1, "unsigned", n);
    }
    return hw::finish();
}
