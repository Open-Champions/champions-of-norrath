// Host contract for msg_load__4SoulPc; not an EE input. strcasecmp and the base routine are stubs.
#include <cstring>
#include <initializer_list>
static int cmp_result, ncmp, nbase, base_ret; static const char* cmp_a; static const char* cmp_b; static void *base_self, *base_buf;
extern "C" char levelName[200];
extern "C" unsigned char norrath_ctors_lava_wraith_view_485240[8];
extern "C" int strcasecmp(const char* a, const char* b) { ++ncmp; cmp_a = a; cmp_b = b; return cmp_result; }
extern "C" int msg_load__8CreaturePc(void* s, char* b) { ++nbase; base_self = s; base_buf = b; return base_ret; }
#include "msg_load.cpp"
int main() {
    int bad = 0;
    for (int c : {0, 1, -1, 99}) {
        for (int r : {0, 5, 40}) {
            cmp_result = c; base_ret = r; ncmp = nbase = 0;
            char obj[16], buf[16];
            int got = msg_load__4SoulPc(obj, buf);
            if (ncmp != 1 || cmp_a != levelName + 81 || cmp_b != reinterpret_cast<const char*>(norrath_ctors_lava_wraith_view_485240)) bad = 1;
            if (c == 0) { if (got != 0 || nbase != 0) bad = 1; }
            else if (got != r || nbase != 1 || base_self != obj || base_buf != buf) bad = 1;
        }
    }
    return bad;
}
