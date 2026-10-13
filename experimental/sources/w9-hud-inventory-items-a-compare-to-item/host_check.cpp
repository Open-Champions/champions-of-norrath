// Host contract for compareToItem. Not an EE plan input.
#include "compare_to_item.cpp"
#include <cstring>
struct Desc { unsigned w[18]; };
static int cmp(const Desc& a, const Desc& b) { return compareToItem__Q24item11descriptionPQ24item11description(&a, &b); }
int main() {
    Desc a, b;
    std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b);
    if (cmp(a, b) != 0) return 1;                  // right empty
    a.w[0] = b.w[0] = 7; a.w[2] = b.w[2] = 3; a.w[3] = b.w[3] = 9; a.w[8] = b.w[8] = 11;
    if (cmp(a, b) != 1) return 2;
    b.w[8] = 12; if (cmp(a, b) != 0) return 3;     // +32 differs
    b.w[8] = 11; b.w[3] = 8; if (cmp(a, b) != 0) return 4;   // high half of +8
    b.w[3] = 9; b.w[2] = 4; if (cmp(a, b) != 0) return 5;    // low half of +8
    b.w[2] = 3; a.w[0] = 6; if (cmp(a, b) != 0) return 6;    // +0 differs
    a.w[0] = 7; a.w[4] = 1; b.w[4] = 2; if (cmp(a, b) != 1) return 7; // other words ignored
    a.w[0] = 0; b.w[0] = 0; if (cmp(a, b) != 0) return 8;
    return 0;
}
