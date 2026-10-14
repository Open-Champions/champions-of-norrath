// Observed lgKbdSetRepeatRate at 0x002e76c0, 124 bytes, from the keyboard library the image
// links in. The library state is the object GLibData (0x008ebdd8). Two unsigned arguments.
//   Either argument zero: the word at offset 96 is cleared and 0 is returned.
//   Otherwise: the second argument is raised to at least 20 and cut to at most 2000, then stored
//   zero-extended in the 64-bit word at offset 112; the first argument is raised to at least 100
//   and cut to at most 3000 and stored in the word at offset 120; the word at offset 96 is set
//   to 1; 0 is returned.
// lgKbMInit starts those three words at 1, 100 and 500, so 96 reads as an enable flag and 112/120
// as two time values; which is the delay and which the repeat period is not established, and the
// units are unknown. Nothing was executed on the EE.
extern "C" {
extern unsigned char GLibData[];

int lgKbdSetRepeatRate(unsigned first, unsigned second) {
    if (first == 0 || second == 0) {
        *reinterpret_cast<int*>(GLibData + 96) = 0;
        return 0;
    }
    if (second < 20) second = 20;
    if (first < 100) first = 100;
    if (second > 2000) second = 2000;
    if (first > 3000) first = 3000;
    *reinterpret_cast<int*>(GLibData + 120) = static_cast<int>(first);
    *reinterpret_cast<int*>(GLibData + 96) = 1;
    *reinterpret_cast<unsigned long long*>(GLibData + 112) = second;
    return 0;
}
}
