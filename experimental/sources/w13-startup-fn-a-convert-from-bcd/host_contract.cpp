// Host contract for convertfrombcd. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// frombcd is scripted with the arithmetic the image's routine performs; the contract records which byte
// offsets are converted, in what order, and that offsets 0 and 4 and everything past 7 stay untouched.
#include <cstdio>
#include <vector>
static std::vector<int> seen;
extern "C" unsigned char frombcd(unsigned char v) { seen.push_back(v); return static_cast<unsigned char>(v - (v >> 4) * 6); }
#include "convert_from_bcd.cpp"
int main() {
    int bad = 0;
    unsigned char c[10] = {0xa0, 11, 22, 33, 0xb4, 44, 55, 66, 0xc1, 0xc2};
    seen.clear();
    convertfrombcd(c);
    const int order[6] = {66, 55, 44, 33, 22, 11};
    if (seen.size() != 6) { std::printf("calls %zu\n", seen.size()); return 1; }
    for (int i = 0; i < 6; ++i) if (seen[i] != order[i]) { std::printf("order %d: %d\n", i, seen[i]); ++bad; }
    const int idx[6] = {7, 6, 5, 3, 2, 1};
    for (int i = 0; i < 6; ++i) if (c[idx[i]] != frombcd(order[i])) { std::printf("store %d\n", idx[i]); ++bad; }
    if (c[0] != 0xa0 || c[4] != 0xb4 || c[8] != 0xc1 || c[9] != 0xc2) { std::puts("untouched bytes changed"); ++bad; }
    return bad;
}
