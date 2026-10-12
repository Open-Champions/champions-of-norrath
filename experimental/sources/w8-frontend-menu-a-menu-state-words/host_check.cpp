#include <cstdio>
#include "menu_state_words.cpp"
static_assert(sizeof(body) == 4 && sizeof(cursorTween) == 4 && sizeof(letterAtATime) == 4, "words");
static_assert(sizeof(fullLaunchOK) == 1 && sizeof(kickMenuList) == 8, "bytes");
int main() {
    if (body || cursorTween != 0.0f || fullLaunchOK || letterAtATime) return 1;
    for (unsigned char b : kickMenuList) if (b) return 2;
    fullLaunchOK = -1;
    if (fullLaunchOK >= 0) return 3;
    cursorTween = 0.5f;
    if (cursorTween != 0.5f) return 4;
    std::puts("menu state words check passed");
    return 0;
}
