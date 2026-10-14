// Host check only; not a build input.
#include "set_anim_sound_channels.cpp"
#include <cstring>
#include <initializer_list>
int main() {
    alignas(8) static unsigned char s[4096], e[4096];
    for (int kind : {0, 1, 2, -1}) {
        std::memset(s, 0x5a, sizeof s); std::memset(e, 0x5a, sizeof e);
        std::memcpy(s + 404, &kind, 4); std::memcpy(e + 404, &kind, 4);
        if (SetAnimSoundChannels__9VICSprite(s) != 0) return 1;
        auto w = [&](int off, int x) { std::memcpy(e + off, &x, 4); };
        for (int k = 0; k <= 18; ++k) w(472 + 52 * k, kind == 1 ? 1 : 0);
        for (int k = 0; k <= 24; ++k) w(1616 + 52 * k, 0);
        for (int off : {3072, 3124, 3176, 3228, 3384, 3436, 3540, 3592, 3644, 3748, 3800}) w(off, 0);
        if (std::memcmp(s, e, sizeof s)) return 2 + kind;
    }
    return 0;
}
