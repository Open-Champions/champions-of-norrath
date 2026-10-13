#include <initializer_list>
#include "init.cpp"
#include <string.h>

static float seenArg[3];
static float returnLength;
extern "C" float NormalizeMagn__7VIVect3(float* vector) {
    memcpy(seenArg, vector, 12);
    for (int i = 0; i < 3; ++i) vector[i] = vector[i] * 0.5f;
    return returnLength;
}

int main() {
    struct Case { float length; } cases[] = {{2.0f}, {0.0f}, {-4.0f}, {0.125f}};
    for (const Case& c : cases) {
        returnLength = c.length;
        float start[3] = {1.0f, 2.0f, 3.0f}, end[3] = {5.0f, -1.0f, 9.0f};
        float ray[20];
        memset(ray, 0x5a, sizeof ray);
        if (Init__8VIEnvRayRC7VIVect3T1Ui(ray, start, end, 0xdeadbeefu) != 0) return 1;
        if (memcmp(ray, start, 12) != 0 || memcmp(ray + 3, end, 12) != 0) return 2;
        if (seenArg[0] != 4.0f || seenArg[1] != -3.0f || seenArg[2] != 6.0f) return 3;
        if (ray[6] != 2.0f || ray[7] != -1.5f || ray[8] != 3.0f) return 4;
        if (ray[9] != c.length) return 5;
        if (ray[10] != (c.length == 0.0f ? 0.0f : 1.0f / c.length)) return 6;
        unsigned int w[3]; memcpy(w, ray + 11, 12);
        if (w[0] != 0xdeadbeefu || w[1] != 0 || w[2] != 1) return 7;
        if (ray[14] != 1.0f) return 8;
        unsigned char tail[20 * 4 - 60]; memcpy(tail, ray + 15, sizeof tail);
        for (unsigned char b : tail) if (b != 0x5a) return 9;
    }
    return 0;
}
