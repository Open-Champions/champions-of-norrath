#include <cstdio>
#include "get_inst_type.cpp"
int main() {
    unsigned char object[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    unsigned char scene[8] = {0};
    int failures = 0;
    if (GetInstType__C20VIEffectVolumeSpriteR7VIScene(object, scene) != 1) { std::fprintf(stderr, "fail: wrong constant\n"); ++failures; }
    if (GetInstType__C20VIEffectVolumeSpriteR7VIScene(nullptr, nullptr) != 1) { std::fprintf(stderr, "fail: arguments are used\n"); ++failures; }
    for (unsigned char b : object) if (b != 0xff) ++failures;
    return failures ? 1 : 0;
}
