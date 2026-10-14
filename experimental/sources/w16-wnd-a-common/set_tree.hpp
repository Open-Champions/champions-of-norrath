// Host checks only; not a build input. Builders and validators for the window-set tree layout
// used by the set routines: a root index at set+12, 20-byte elements from the base word at set+8
// (+0 colour, +4 parent, +8 left, +12 right, +16 key), index 0 the nil element.
#pragma once
#include "host_world.hpp"
#include <vector>

namespace hw {
constexpr unsigned SET = BASE + 0x100;
constexpr unsigned ELEM = BASE + 0x1000;
inline unsigned E(int index, int field) { return ELEM + static_cast<unsigned>(index) * 20 + static_cast<unsigned>(field); }
inline unsigned& colour(int i) { return w(E(i, 0)); }
inline unsigned& parent(int i) { return w(E(i, 4)); }
inline unsigned& left(int i) { return w(E(i, 8)); }
inline unsigned& right(int i) { return w(E(i, 12)); }
inline unsigned& key(int i) { return w(E(i, 16)); }
inline unsigned& root() { return w(SET + 12); }

inline void reset_set() {
    clear();
    w(SET + 8) = ELEM;
}
// Plain unbalanced search tree; element i holds keys[i-1]. Colours are all 0.
inline void build_plain(const std::vector<unsigned>& keys) {
    reset_set();
    for (size_t n = 0; n < keys.size(); ++n) {
        int i = static_cast<int>(n) + 1;
        key(i) = keys[n];
        if (root() == 0) { root() = static_cast<unsigned>(i); continue; }
        int at = static_cast<int>(root());
        for (;;) {
            unsigned& link = keys[n] < key(at) ? left(at) : right(at);
            if (link == 0) { link = static_cast<unsigned>(i); parent(i) = static_cast<unsigned>(at); break; }
            at = static_cast<int>(link);
        }
    }
}
inline void inorder(int i, std::vector<int>& out) {
    if (i == 0) return;
    inorder(static_cast<int>(left(i)), out);
    out.push_back(i);
    inorder(static_cast<int>(right(i)), out);
}
// Returns the black height, or -1 on a violated invariant (colours are 1 = red, 0 = black).
inline int validate(int i, unsigned lo, unsigned hi, bool has_lo, bool has_hi, bool red_parent) {
    if (i == 0) return 1;
    if ((has_lo && key(i) <= lo) || (has_hi && key(i) >= hi)) return -1;
    if (colour(i) == 1 && red_parent) return -1;
    if (left(i) && parent(static_cast<int>(left(i))) != static_cast<unsigned>(i)) return -1;
    if (right(i) && parent(static_cast<int>(right(i))) != static_cast<unsigned>(i)) return -1;
    int a = validate(static_cast<int>(left(i)), lo, key(i), has_lo, true, colour(i) == 1);
    int b = validate(static_cast<int>(right(i)), key(i), hi, true, has_hi, colour(i) == 1);
    if (a < 0 || b < 0 || a != b) return -1;
    return a + (colour(i) == 0 ? 1 : 0);
}
inline bool valid_red_black() {
    if (root() == 0) return true;
    if (colour(static_cast<int>(root())) != 0 || parent(static_cast<int>(root())) != 0) return false;
    return validate(static_cast<int>(root()), 0, 0, false, false, false) >= 0;
}
}  // namespace hw
