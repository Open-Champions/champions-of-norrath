// Host check only; not a build input. Walks random search trees from the lowest element with
// Next and compares the visited order with the sorted keys; also checks the end value and a
// single-step case with a deep right subtree.
#define NORRATH_HOST_ARENA 1
#include "next.cpp"
#include "../w16-wnd-a-common/set_tree.hpp"
#include <algorithm>

int main() {
    hw::Rng rng(12);
    for (int round = 0; round < 300; ++round) {
        std::vector<unsigned> keys;
        const int count = 1 + static_cast<int>(rng.next() % 40);
        for (int n = 0; n < count; ++n) {
            unsigned k;
            do k = rng.next() % 1000; while (std::find(keys.begin(), keys.end(), k) != keys.end());
            keys.push_back(k);
        }
        hw::build_plain(keys);
        std::vector<int> order;
        hw::inorder(static_cast<int>(hw::root()), order);
        for (size_t n = 0; n < order.size(); ++n) {
            const int got = Next__Ct5VISet1ZP5VIWndi(hw::at(hw::SET), order[n]);
            const int want = n + 1 < order.size() ? order[n + 1] : 0;
            hw::check(got == want, "successor", round, static_cast<long>(n));
        }
    }
    return hw::finish();
}
