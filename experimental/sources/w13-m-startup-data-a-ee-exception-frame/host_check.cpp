// Host check for the exception frame block. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
// The size and alignment are the observed ones; the restore routine reads offsets up to 0x228.
#include <cstdio>
#include <cstdint>
#include "ee_exception_frame.cpp"

int main() {
    int bad = 0;
    if (sizeof norrath_ee_exception_frame != 0x230) { std::puts("frame size"); ++bad; }
    if (reinterpret_cast<std::uintptr_t>(norrath_ee_exception_frame) % 16 != 0) { std::puts("frame alignment"); ++bad; }
    for (unsigned char byte : norrath_ee_exception_frame) if (byte != 0) { std::puts("frame not zero"); ++bad; break; }
    if (bad == 0) std::puts("exception frame passed");
    return bad;
}
