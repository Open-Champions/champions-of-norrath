#include "init.cpp"
#include <string.h>

static int order[16]; static int order_n;
static int results[3];
static void* seen_alloc; static void* seen_clipper; static int seen_clip_n; static void* seen_vol; static const void* seen_box;
static float cos_arg; static float cos_ret = 0.25f; static int cos_n;
extern "C" int Allocate__14VIQueryTriList(void* o) { seen_alloc = o; order[order_n++] = 1; return results[0]; }
extern "C" int Init__9VIClipperi(void* c, int n) { seen_clipper = c; seen_clip_n = n; order[order_n++] = 2; return results[1]; }
extern "C" int Volume__14VIQueryTriListRC6VIBBox(void* o, const void* b) { seen_vol = o; seen_box = b; order[order_n++] = 3; return results[2]; }
extern "C" float cosf(float v) { cos_arg = v; ++cos_n; order[order_n++] = 4; return cos_ret; }

int main() {
    unsigned char object[100], expect[100];
    float vec[3] = {1.5f, -2.5f, 3.25f};
    int box;
    memset(object, 0x44, sizeof object);
    memcpy(expect, object, sizeof expect);
    int r = Init__14VIQueryTriListiRC6VIBBoxRC7VIVect3f(object, 77, &box, vec, 60.0f);
    if (r != 0) return 1;
    int ex[] = {1, 2, 3, 4};
    if (order_n != 4) return 2;
    for (int i = 0; i < 4; ++i) if (order[i] != ex[i]) return 3;
    if (seen_alloc != object || seen_clipper != object + 12 || seen_clip_n != 9 || seen_vol != object || seen_box != &box) return 4;
    float k = __builtin_bit_cast(float, 0x3c8efa34u);
    if (cos_arg != 60.0f * k) return 5;
    int count = 77; float zero = 0; (void)zero;
    memcpy(expect + 4, &count, 4);
    memset(expect, 0, 4);
    memcpy(expect + 28, vec, 12);
    memcpy(expect + 40, &cos_ret, 4);
    if (memcmp(object, expect, sizeof object) != 0) return 6;
    for (int fail = 0; fail < 3; ++fail) {
        memset(results, 0, sizeof results); results[fail] = -2;
        order_n = 0; cos_n = 0;
        memset(object, 0x44, sizeof object);
        if (Init__14VIQueryTriListiRC6VIBBoxRC7VIVect3f(object, 5, &box, vec, 1.0f) != -1) return 10 + fail;
        if (order_n != fail + 1 || cos_n != 0) return 20 + fail;
        int w4; memcpy(&w4, object + 4, 4);
        unsigned int w0; memcpy(&w0, object, 4);
        if (w4 != 5 || w0 != 0 || object[28] != 0x44) return 30 + fail;
    }
    return 0;
}
