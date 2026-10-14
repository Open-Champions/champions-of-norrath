// Host check, not a plan input.
#include <cstdio>
#include "m_buf.cpp"
int main() {
    if (sizeof m_buf != 320) return 1;
    for (unsigned char b : m_buf) if (b) return 2;
    std::puts("m_buf check passed");
    return 0;
}
