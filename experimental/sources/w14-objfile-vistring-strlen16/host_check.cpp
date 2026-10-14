// Host check only; not a build input.
#include "strlen16.cpp"
int main() {
    const unsigned short a[] = {0};
    const unsigned short b[] = {0x41, 0};
    const unsigned short c[] = {0x41, 0x100, 0xffff, 0x7f, 0, 0x55};
    const unsigned short d[] = {0x100, 0, 0};
    if (strlen16__8VIStringPCUs(a) != 0 || strlen16__8VIStringPCUs(b) != 1) return 1;
    if (strlen16__8VIStringPCUs(c) != 4 || strlen16__8VIStringPCUs(d) != 1) return 2;
    return 0;
}
