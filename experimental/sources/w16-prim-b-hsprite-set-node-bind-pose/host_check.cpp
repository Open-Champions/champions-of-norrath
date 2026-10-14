// Host check only; not a build input. Element addresses are arena offsets (NORRATH_HOST_ARENA).
#define NORRATH_HOST_ARENA 1
#include "set_node_bind_pose.cpp"
#include <initializer_list>
#include <string.h>

alignas(16) static unsigned char arena[0x4000];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }

static int g_step;
static int g_order[8];
static float *g_m0, *g_m1;
static unsigned char* g_expect_dest;
static bool g_bad;
static void note(int id) { if (g_step < 8) g_order[g_step] = id; ++g_step; }

extern "C" void Transform__10VIMatrix44RC6VIQuatfRC7VIVect3(float* m, const float* q, float s, const float* t) {
    note(1);
    g_m0 = m;
    for (int i = 0; i < 16; ++i) if (m[i] != 0.0f) g_bad = true;   // freshly zeroed local
    for (int i = 0; i < 16; ++i) m[i] = 100.0f + i + q[0] + s + t[2];
}
extern "C" void Invert__10VIMatrix44RC10VIMatrix44(float* r, const float* src) {
    note(2);
    if (src != g_m0) g_bad = true;
    for (int i = 0; i < 16; ++i) r[i] = -src[i];
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(float* r, const float* a, const float* b) {
    note(3);
    g_m1 = r;
    if (a != g_m0) g_bad = true;
    for (int i = 0; i < 16; ++i) r[i] = a[i] * 2.0f + b[i];
}
static float s_conv_in[16];
extern "C" void ConvertTo__C10VIMatrix44R6VIQuatRfR7VIVect3(const float* m, float* q, float* s, float* t) {
    note(4);
    if (m != g_m1) g_bad = true;
    memcpy(s_conv_in, m, 64);
    q[0] = m[0]; q[1] = m[1]; q[2] = m[2]; q[3] = m[3];
    *s = m[4];
    t[0] = m[5]; t[1] = m[6]; t[2] = m[7];
}
extern "C" void _$_10VIMatrix44(float* m, int flags) {
    note(g_step >= 2 && g_order[g_step - 1] != 5 && g_order[g_step - 1] != 6 ? 5 : 6);
    if (g_m1 && g_order[g_step - 1] == 5 && m != g_m1) g_bad = true;
    if (g_m1 && g_order[g_step - 1] == 6 && m != g_m0) g_bad = true;
    if (flags != 2) g_bad = true;
}

int main() {
    for (int parent : {-1, 0, 2}) {
        for (int node : {1, 3}) {
            memset(arena, 0x33, sizeof arena);
            g_step = 0; g_bad = false; g_m0 = g_m1 = 0;
            unsigned char* sprite = arena + 0x3000;
            unsigned elements = 0x400;
            memcpy(sprite + 72, &elements, 4);
            unsigned char* e = arena + elements + node * 256;
            memcpy(e, &parent, 4);
            alignas(16) float q[4] = {0.5f, -1.5f, 2.5f, 0.25f};
            float t[3] = {7.0f, 8.0f, 9.0f};
            float s = 1.75f;
            unsigned char before[0x4000];
            memcpy(before, arena, sizeof before);
            if (SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3(sprite, node, reinterpret_cast<unsigned char*>(q), s, t) != 0) return 1;
            if (g_bad) return 2;
            if (memcmp(e + 124, q, 16) != 0 || memcmp(e + 140, &s, 4) != 0 || memcmp(e + 144, t, 12) != 0) return 3;
            float tm[16];
            for (int i = 0; i < 16; ++i) tm[i] = 100.0f + i + 0.5f + 1.75f + 9.0f;
            float inv[16];
            for (int i = 0; i < 16; ++i) inv[i] = -tm[i];
            if (memcmp(e + 160, inv, 64) != 0) return 4;
            int want_order[5] = {1, 2, 3, 4, 5};
            if (parent == -1) {
                if (g_step != 4 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 5 || g_order[3] != 6) return 5;
                if (memcmp(e + 224, q, 16) != 0 || memcmp(e + 240, &s, 4) != 0 || memcmp(e + 244, t, 12) != 0) return 6;
            } else {
                if (g_step != 6) return 7;
                int order[6] = {1, 2, 3, 4, 5, 6};
                for (int i = 0; i < 6; ++i) if (g_order[i] != order[i]) return 8;
                float prod[16];
                const float* pinv = reinterpret_cast<const float*>(arena + elements + parent * 256 + 160);
                for (int i = 0; i < 16; ++i) prod[i] = tm[i] * 2.0f + pinv[i];
                if (memcmp(s_conv_in, prod, 64) != 0) return 9;
                if (memcmp(e + 224, prod, 16) != 0 || memcmp(e + 240, prod + 4, 4) != 0 || memcmp(e + 244, prod + 5, 12) != 0) return 10;
                (void)want_order;
            }
        }
    }
    return 0;
}
