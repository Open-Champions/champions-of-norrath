// Host contract for FindIopFunction__FUi. Not a build-plan input.
//   c++ -std=c++17 -o check host_contract.cpp && ./check
// The send buffer is filled the way SetIopFunction__FUii fills it (code, count, arguments, running command
// count in word 0) and the routine must return the address after the count word of the first matching
// command, null for an absent code, an empty buffer or a non-positive count, and never look beyond the
// commands that the count names.
#include <cstdio>
#include <cstring>
extern "C" { int norrath_overlay_sSend_11edb00[128]; }
#include "find_iop_function.cpp"
static int* put(int* at, int code, int n) {
    at[0] = code; at[1] = n;
    for (int i = 0; i < n; ++i) at[2 + i] = 100 * code + i;
    return at + 2 + n;
}
int main() {
    int bad = 0;
    int* s = norrath_overlay_sSend_11edb00;
    std::memset(s, 0, 128 * sizeof(int));
    if (FindIopFunction__FUi(4) != nullptr) { std::puts("empty"); ++bad; }
    int* at = s + 1;
    at = put(at, 9, 3); at = put(at, 4, 2); at = put(at, 7, 0); at = put(at, 4, 5);
    s[0] = 4;
    if (FindIopFunction__FUi(9) != s + 3) { std::puts("first"); ++bad; }
    if (FindIopFunction__FUi(4) != s + 1 + 5 + 2) { std::puts("second (first match wins)"); ++bad; }
    if (FindIopFunction__FUi(7) != s + 1 + 5 + 4 + 2) { std::puts("zero-length command"); ++bad; }
    if (FindIopFunction__FUi(8) != nullptr) { std::puts("absent"); ++bad; }
    s[0] = 3;
    if (FindIopFunction__FUi(4)[0] != 400 || FindIopFunction__FUi(7) == nullptr) { std::puts("count 3"); ++bad; }
    s[0] = 2;
    if (FindIopFunction__FUi(7) != nullptr) { std::puts("count limits the walk"); ++bad; }
    s[0] = 0;
    if (FindIopFunction__FUi(9) != nullptr) { std::puts("count 0"); ++bad; }
    s[0] = -3;
    if (FindIopFunction__FUi(9) != nullptr) { std::puts("negative count"); ++bad; }
    return bad;
}
