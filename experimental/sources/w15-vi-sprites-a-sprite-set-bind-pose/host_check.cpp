// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 14];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "set_bind_pose.cpp"
#include <cstring>
static int calls; static void *m[4], *q[4], *v[4]; static float s[4];
extern "C" void Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(void* mat, const void* rot, float scalar, const void* vec) {
    m[calls] = mat; q[calls] = const_cast<void*>(rot); v[calls] = const_cast<void*>(vec); s[calls] = scalar; ++calls;
}
int main() {
    alignas(8) unsigned char sprite[256] = {};
    unsigned* vec = reinterpret_cast<unsigned*>(sprite + 64);
    vec[0] = 3; vec[2] = arena_base + 0x200;
    for (int k = 0; k < 3; ++k) *reinterpret_cast<float*>(arena + 0x200 + 256 * k + 140) = 1.5f * (k + 1);
    if (SetBindPose__9VIHSprite(sprite) != 0 || calls != 3) return 1;
    for (int k = 0; k < 3; ++k) {
        unsigned char* e = arena + 0x200 + 256 * k;
        if (m[k] != e + 48 || q[k] != e + 124 || v[k] != e + 144 || s[k] != 1.5f * (k + 1)) return 2 + k;
    }
    calls = 0; vec[0] = 0;
    if (SetBindPose__9VIHSprite(sprite) != 0 || calls) return 6;
    vec[0] = static_cast<unsigned>(-2);
    if (SetBindPose__9VIHSprite(sprite) != 0 || calls) return 7;
    return 0;
}
