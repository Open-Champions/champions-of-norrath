// Host check only; not a build input. Compares size and a few table positions with the values read from the image.
#include "key_map.cpp"
#include <cstdio>

int main() {
    bool ok = sizeof sKeyMap == 66 && sKeyMap[0] == 0x41 && sKeyMap[5] == 0x31 && sKeyMap[10] == 0x05 &&
              sKeyMap[11] == 0x46 && sKeyMap[32] == 0x3f && sKeyMap[63] == 0x0a && sKeyMap[64] == 0 && sKeyMap[65] == 0;
    unsigned sum = 0;
    for (unsigned char v : sKeyMap) sum += v;
    ok = ok && sum == 3305;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
