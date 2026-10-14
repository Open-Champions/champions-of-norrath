#include <cstdio>
#include <cstring>
extern "C" unsigned char sequence[76 * 3] = {};
extern "C" void soundMakeSequenceUnpausable__Fi(int);
int main() {
    std::memset(sequence, 0, sizeof sequence);
    soundMakeSequenceUnpausable__Fi(-1);
    soundMakeSequenceUnpausable__Fi(1);
    soundMakeSequenceUnpausable__Fi(0x8002);
    if (sequence[59] != 0 || sequence[76 + 59] != 0 || sequence[2 * 76 + 59] != 1) return 1;
    std::puts("unpausable contract passed");
    return 0;
}
