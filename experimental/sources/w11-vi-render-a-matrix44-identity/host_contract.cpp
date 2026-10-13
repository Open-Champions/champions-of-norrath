#include "identity.cpp"

int main() {
    float guard[24];
    for (int i = 0; i < 24; ++i) guard[i] = 7.0f + i;
    Identity__10VIMatrix44(guard + 4);
    for (int i = 0; i < 24; ++i) {
        float expect = 7.0f + i;
        if (i >= 4 && i < 20) {
            int k = i - 4;
            expect = (k % 5 == 0) ? 1.0f : 0.0f;
        }
        if (guard[i] != expect) return 1 + i;
    }
    return 0;
}
