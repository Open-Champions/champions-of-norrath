// Host contract for msg_load__8SkeletonPc; not an EE input. The base load and norrath_skeleton_init_type
// are stubs that record order; the init stub also sees the fields stored before it ran.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string order; static int base_count, init_flag; static unsigned seen_208;
static unsigned char* g_obj;
extern "C" int msg_load__8CreaturePc(void*, char*) { order += "base;"; return base_count; }
extern "C" void norrath_skeleton_init_type(void* s, int f) {
    order += "init;"; init_flag = f; std::memcpy(&seen_208, static_cast<unsigned char*>(s) + 208, 4); if (s != g_obj) order += "BADSELF;";
}
#include "msg_load.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char obj[0x600], in[0x100];
    g_obj = obj;
    for (int n : {0, 4, 12, 40}) {
        for (short st : {0, 9, 10, 11, -10}) {
            std::memset(obj, 0x33, sizeof obj);
            for (unsigned i = 0; i < sizeof in; ++i) in[i] = static_cast<unsigned char>(i * 3 + 7);
            std::memcpy(obj + 572, &st, 2);
            unsigned ref1124 = 0x33333333u; std::memcpy(&ref1124, obj + 1124, 4); if (st == 10) ref1124 = 0;
            unsigned char ref[0x600]; std::memcpy(ref, obj, sizeof ref);
            std::memcpy(ref + 968, in + n, 4); std::memcpy(ref + 976, in + n + 4, 4);
            std::memcpy(ref + 1120, in + n + 8, 4); std::memcpy(ref + 208, in + n + 12, 4);
            std::memcpy(ref + 1124, &ref1124, 4);
            order.clear(); base_count = n; init_flag = -1;
            const int r = msg_load__8SkeletonPc(obj, reinterpret_cast<char*>(in));
            unsigned want208; std::memcpy(&want208, in + n + 12, 4);
            if (r != n + 16 || order != "base;init;" || init_flag != 1 || seen_208 != want208) bad = 1;
            if (std::memcmp(obj, ref, sizeof obj)) bad = 1;
        }
    }
    return bad;
}
