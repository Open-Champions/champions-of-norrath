// Host check only; not a build input.
#include "sample_rate.cpp"
int main() {
    int o[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    if (SampleRate__C6VIWave(o) != 8) return 1;
    o[8] = 44100;
    if (SampleRate__C6VIWave(o) != 44100) return 2;
    return 0;
}
