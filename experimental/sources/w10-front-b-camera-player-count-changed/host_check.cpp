#include "player_count_changed.cpp"
#include <cstdio>
#include <cstring>

int main() {
    unsigned char a[256], b[256];
    for (int i = 0; i < 256; ++i) a[i] = b[i] = static_cast<unsigned char>(i * 7 + 3);
    playerCountChanged__6Camera(a);
    const float one = 1.0f;
    std::memcpy(b + 200, &one, 4);
    int bad = std::memcmp(a, b, 256) != 0;
    std::printf("%s\n", bad ? "FAIL" : "ok: only the word at +200 changes, to 1.0f");
    return bad;
}
