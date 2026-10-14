#include "calc_member_transform.cpp"
#include <initializer_list>
#include <string.h>

static int calls;
static void* got_matrix;
static const void* got_in;
static float got_scalar;
static void* got_out;
extern "C" int Transform__10VIMatrix44RC7VIVect3fT1(void* m, const void* in, float s, void* out) {
    ++calls;
    got_matrix = m;
    got_in = in;
    got_scalar = s;
    got_out = out;
    return 1234;
}

int main() {
    alignas(16) unsigned char obj[512];
    unsigned char matrix[64];
    for (int index : {0, 1, 3}) {
        memset(obj, 0x31, sizeof obj);
        float s = 2.5f + index;
        memcpy(obj + index * 32 + 84, &s, 4);
        calls = 0;
        if (CalcMemberTransform__13VIGroupSpriteiR10VIMatrix44(obj, index, matrix) != 0) return 1;
        if (calls != 1 || got_matrix != matrix) return 2;
        if (got_in != obj + index * 32 + 72 || got_out != obj + index * 32 + 88) return 3;
        if (got_scalar != s) return 4;
    }
    return 0;
}
