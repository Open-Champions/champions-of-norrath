#include "begin.cpp"

int main() {
    float object[6] = {7, 7, 0.25f, 7, 7, 7};
    Begin__7VIFaderff(object, 1.0f, 0.5f);
    if (object[2] != 0.25f || object[3] != 1.0f || object[4] != (1.0f - 0.25f) / 0.5f) return 1;
    if (object[0] != 7 || object[1] != 7 || object[5] != 7) return 2;
    Begin__7VIFaderff(object, 0.0f, 4.0f);
    if (object[3] != 0.0f || object[4] != -0.0625f || object[2] != 0.25f) return 3;
    Begin__7VIFaderff(object, 0.75f, 0.0f);
    if (object[2] != 0.75f || object[3] != 0.75f || object[4] != 0.0f) return 4;
    if (object[0] != 7 || object[1] != 7 || object[5] != 7) return 5;
    return 0;
}
