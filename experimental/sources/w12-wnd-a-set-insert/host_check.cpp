// Host check only; not a build input. Inserts random keys (with repeats) through the authored
// routine, using reference red-black helpers for the callee roles (node allocation, rotations),
// and validates the result: red-black properties, parent links, sorted unique keys, the returned
// index per key, an unchanged tree for repeats, and the -1 pass-through when allocation fails.
#define NORRATH_HOST_ARENA 1
#include "insert.cpp"
#include "../w16-wnd-a-common/set_tree.hpp"
#include <algorithm>
#include <map>

static int fail_after = -1;      // allocation fails when this many nodes exist
static int allocations;
static std::string rotations;
extern "C" int AddNode__t5VISet1ZP5VIWndii(void*, int parent, int side) {
    if (fail_after >= 0 && hw::next_free - 1 >= fail_after) return -1;
    const int z = hw::next_free++;
    ++allocations;
    hw::colour(z) = 1;
    hw::parent(z) = static_cast<unsigned>(parent);
    if (parent == 0) hw::root() = static_cast<unsigned>(z);
    else if (side) hw::left(parent) = static_cast<unsigned>(z);
    else hw::right(parent) = static_cast<unsigned>(z);
    return z;
}
extern "C" void LeftRotate__t5VISet1ZP5VIWndi(void*, int x) { rotations += "L"; hw::rotate_left(x); }
extern "C" void RightRotate__t5VISet1ZP5VIWndi(void*, int x) { rotations += "R"; hw::rotate_right(x); }

int main() {
    hw::Rng rng(14);
    for (int round = 0; round < 300; ++round) {
        hw::reset_set();
        hw::next_free = 1;
        std::map<unsigned, int> have;
        const int count = 1 + static_cast<int>(rng.next() % 60);
        const unsigned span = 1 + rng.next() % 90;
        const bool ascending = round % 3 == 1, descending = round % 3 == 2;
        for (int n = 0; n < count; ++n) {
            unsigned k = ascending ? static_cast<unsigned>(n) : descending ? static_cast<unsigned>(count - n) : rng.next() % span;
            if (round % 5 == 4) k |= 0x80000000u;
            const auto found = have.find(k);
            const int before = allocations;
            const int got = Insert__t5VISet1ZP5VIWndRCP5VIWnd(hw::at(hw::SET), &k);
            if (found != have.end()) {
                hw::check(got == found->second && allocations == before, "repeat", round, n);
            } else {
                hw::check(got > 0 && hw::key(got) == k && allocations == before + 1, "new", round, n);
                have[k] = got;
            }
            hw::check(hw::valid_red_black(), "red-black", round, n);
        }
        std::vector<int> order;
        hw::inorder(static_cast<int>(hw::root()), order);
        hw::check(order.size() == have.size(), "size", round);
        size_t i = 0;
        for (const auto& entry : have) {
            if (i < order.size()) hw::check(order[i] == entry.second && hw::key(order[i]) == entry.first, "order", round, static_cast<long>(i));
            ++i;
        }
    }
    // Allocation failure returns -1 and leaves the tree untouched.
    hw::reset_set();
    hw::next_free = 1;
    for (unsigned k : {10u, 20u, 30u}) Insert__t5VISet1ZP5VIWndRCP5VIWnd(hw::at(hw::SET), &k);
    fail_after = 3;
    unsigned k = 40;
    hw::check(Insert__t5VISet1ZP5VIWndRCP5VIWnd(hw::at(hw::SET), &k) == -1, "failure");
    hw::check(hw::valid_red_black(), "after failure");
    std::vector<int> order;
    hw::inorder(static_cast<int>(hw::root()), order);
    hw::check(order.size() == 3, "failure size");
    return hw::finish();
}
