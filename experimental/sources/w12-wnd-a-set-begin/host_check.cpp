// Host check only; not a build input. Builds search trees from random keys with an independent
// builder and compares the result with the lowest key's element.
#define NORRATH_HOST_ARENA 1
#include "begin.cpp"
#include "../w16-wnd-a-common/set_tree.hpp"
#include <algorithm>

int main() {
    hw::Rng rng(11);
    hw::reset_set();
    hw::check(Begin__Ct5VISet1ZP5VIWnd(hw::at(hw::SET)) == 0, "empty");
    for (int round = 0; round < 200; ++round) {
        std::vector<unsigned> keys;
        const int count = 1 + static_cast<int>(rng.next() % 40);
        for (int n = 0; n < count; ++n) {
            unsigned k;
            do k = rng.next() % 1000; while (std::find(keys.begin(), keys.end(), k) != keys.end());
            keys.push_back(k);
        }
        hw::build_plain(keys);
        const int lowest = static_cast<int>(std::min_element(keys.begin(), keys.end()) - keys.begin()) + 1;
        hw::check(Begin__Ct5VISet1ZP5VIWnd(hw::at(hw::SET)) == lowest, "lowest", round, lowest);
    }
    // A single element and a root with only a right branch.
    hw::build_plain({5});
    hw::check(Begin__Ct5VISet1ZP5VIWnd(hw::at(hw::SET)) == 1, "single");
    hw::build_plain({5, 6, 7});
    hw::check(Begin__Ct5VISet1ZP5VIWnd(hw::at(hw::SET)) == 1, "right chain");
    return hw::finish();
}
