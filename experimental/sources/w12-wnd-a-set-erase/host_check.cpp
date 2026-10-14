// Host check only; not a build input. Builds random red-black trees with reference helpers,
// erases random elements through the authored routine (reference helpers play the fix-up and
// free roles) and validates red-black properties, parent links, remaining keys and the returned
// successor (the in-order next element, taken before the erase).
#define NORRATH_HOST_ARENA 1
#include "erase.cpp"
#include "../w16-wnd-a-common/set_tree.hpp"
#include <algorithm>

static std::vector<int> freed;
static int fixups;
extern "C" int Next__Ct5VISet1ZP5VIWndi(const void*, int index) {
    std::vector<int> order;
    hw::inorder(static_cast<int>(hw::root()), order);
    for (size_t n = 0; n < order.size(); ++n)
        if (order[n] == index) return n + 1 < order.size() ? order[n + 1] : 0;
    return -99;
}
extern "C" void EraseFixup__t5VISet1ZP5VIWndi(void*, int x) { ++fixups; hw::delete_fixup(x); }
extern "C" void FreeNode__t5VISet1ZP5VIWndi(void*, int index) { freed.push_back(index); }

int main() {
    hw::Rng rng(15);
    for (int round = 0; round < 400; ++round) {
        hw::reset_set();
        hw::next_free = 1;
        const int count = 1 + static_cast<int>(rng.next() % 50);
        std::vector<unsigned> keys;
        for (int n = 0; n < count; ++n) {
            unsigned k;
            do k = rng.next() % 500; while (std::find(keys.begin(), keys.end(), k) != keys.end());
            keys.push_back(k);
            hw::rb_insert(k);
        }
        int live = count;
        while (live > 0) {
            std::vector<int> before;
            hw::inorder(static_cast<int>(hw::root()), before);
            const int victim = before[rng.next() % before.size()];
            size_t position = std::find(before.begin(), before.end(), victim) - before.begin();
            const int want = position + 1 < before.size() ? before[position + 1] : 0;
            freed.clear();
            hw::colour(0) = 0;
            const int got = Erase__t5VISet1ZP5VIWndi(hw::at(hw::SET), victim);
            hw::check(got == want, "successor", round, live);
            hw::check(freed.size() == 1 && freed[0] == victim, "freed", round, live);
            hw::check(hw::valid_red_black(), "red-black", round, live);
            std::vector<int> after;
            hw::inorder(static_cast<int>(hw::root()), after);
            std::vector<unsigned> keys_before, keys_after;
            for (int i : before) if (i != victim) keys_before.push_back(hw::key(i));
            for (int i : after) keys_after.push_back(hw::key(i));
            // Keys travel with their elements, except that the victim's own key is gone.
            hw::check(after.size() == before.size() - 1, "count", round, live);
            hw::check(std::is_sorted(keys_after.begin(), keys_after.end()), "sorted", round, live);
            std::vector<unsigned> expected;
            for (size_t n = 0; n < before.size(); ++n) if (before[n] != victim) expected.push_back(hw::key(before[n]));
            hw::check(expected == keys_after, "keys", round, live);
            --live;
            if (hw::failures) return hw::finish();
        }
        hw::check(hw::root() == 0, "empty at end", round);
    }
    hw::check(fixups > 0, "fix-up reached");
    return hw::finish();
}
