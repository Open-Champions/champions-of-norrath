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

// Reference red-black helpers (standard textbook algorithms, written independently of the
// authored routines). Colours are 1 = red, 0 = black.
inline void rotate_left(int x) {
    const int y = static_cast<int>(right(x));
    right(x) = left(y);
    if (left(y)) parent(static_cast<int>(left(y))) = static_cast<unsigned>(x);
    parent(y) = parent(x);
    if (parent(x) == 0) root() = static_cast<unsigned>(y);
    else if (static_cast<int>(left(static_cast<int>(parent(x)))) == x) left(static_cast<int>(parent(x))) = static_cast<unsigned>(y);
    else right(static_cast<int>(parent(x))) = static_cast<unsigned>(y);
    left(y) = static_cast<unsigned>(x);
    parent(x) = static_cast<unsigned>(y);
}
inline void rotate_right(int x) {
    const int y = static_cast<int>(left(x));
    left(x) = right(y);
    if (right(y)) parent(static_cast<int>(right(y))) = static_cast<unsigned>(x);
    parent(y) = parent(x);
    if (parent(x) == 0) root() = static_cast<unsigned>(y);
    else if (static_cast<int>(right(static_cast<int>(parent(x)))) == x) right(static_cast<int>(parent(x))) = static_cast<unsigned>(y);
    else left(static_cast<int>(parent(x))) = static_cast<unsigned>(y);
    right(y) = static_cast<unsigned>(x);
    parent(x) = static_cast<unsigned>(y);
}
inline void insert_fixup(int z) {
    while (parent(z) && colour(static_cast<int>(parent(z))) == 1) {
        int p = static_cast<int>(parent(z));
        int g = static_cast<int>(parent(p));
        if (p == static_cast<int>(left(g))) {
            const int u = static_cast<int>(right(g));
            if (colour(u) == 1) { colour(p) = 0; colour(u) = 0; colour(g) = 1; z = g; }
            else {
                if (z == static_cast<int>(right(p))) { z = p; rotate_left(z); p = static_cast<int>(parent(z)); g = static_cast<int>(parent(p)); }
                colour(p) = 0; colour(g) = 1; rotate_right(g);
            }
        } else {
            const int u = static_cast<int>(left(g));
            if (colour(u) == 1) { colour(p) = 0; colour(u) = 0; colour(g) = 1; z = g; }
            else {
                if (z == static_cast<int>(left(p))) { z = p; rotate_right(z); p = static_cast<int>(parent(z)); g = static_cast<int>(parent(p)); }
                colour(p) = 0; colour(g) = 1; rotate_left(g);
            }
        }
    }
    colour(static_cast<int>(root())) = 0;
}
// Next free element index for hand-built trees.
inline int next_free = 1;
inline int rb_insert(unsigned k) {
    const int z = next_free++;
    key(z) = k;
    int y = 0, x = static_cast<int>(root());
    while (x) { y = x; x = k < key(x) ? static_cast<int>(left(x)) : static_cast<int>(right(x)); }
    parent(z) = static_cast<unsigned>(y);
    if (y == 0) root() = static_cast<unsigned>(z);
    else if (k < key(y)) left(y) = static_cast<unsigned>(z);
    else right(y) = static_cast<unsigned>(z);
    colour(z) = 1;
    insert_fixup(z);
    return z;
}
inline void delete_fixup(int x) {
    while (x != static_cast<int>(root()) && colour(x) == 0) {
        int p = static_cast<int>(parent(x));
        if (x == static_cast<int>(left(p))) {
            int s = static_cast<int>(right(p));
            if (colour(s) == 1) { colour(s) = 0; colour(p) = 1; rotate_left(p); s = static_cast<int>(right(p)); }
            if (colour(static_cast<int>(left(s))) == 0 && colour(static_cast<int>(right(s))) == 0) { colour(s) = 1; x = p; }
            else {
                if (colour(static_cast<int>(right(s))) == 0) { colour(static_cast<int>(left(s))) = 0; colour(s) = 1; rotate_right(s); s = static_cast<int>(right(p)); }
                colour(s) = colour(p); colour(p) = 0; colour(static_cast<int>(right(s))) = 0; rotate_left(p);
                x = static_cast<int>(root());
            }
        } else {
            int s = static_cast<int>(left(p));
            if (colour(s) == 1) { colour(s) = 0; colour(p) = 1; rotate_right(p); s = static_cast<int>(left(p)); }
            if (colour(static_cast<int>(right(s))) == 0 && colour(static_cast<int>(left(s))) == 0) { colour(s) = 1; x = p; }
            else {
                if (colour(static_cast<int>(left(s))) == 0) { colour(static_cast<int>(right(s))) = 0; colour(s) = 1; rotate_left(s); s = static_cast<int>(left(p)); }
                colour(s) = colour(p); colour(p) = 0; colour(static_cast<int>(left(s))) = 0; rotate_right(p);
                x = static_cast<int>(root());
            }
        }
    }
    colour(x) = 0;
}
}  // namespace hw
