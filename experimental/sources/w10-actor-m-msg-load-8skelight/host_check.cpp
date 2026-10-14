// Host contract for msg_load__8SkelightPc; not an EE input. Stubs record order; setParameters records
// worldDifficulty as seen at call time.
#define W10_ACTOR_M_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
unsigned char* w10_actor_m_host_base;
unsigned w10_actor_m_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
extern "C" float worldDifficulty;
float worldDifficulty;
static std::string order; static int base_count; static float seen_wd; static float args5[5]; static int flag_arg; static void* g_obj;
extern "C" int msg_load__8CreaturePc(void*, char*) { order += "base;"; return base_count; }
extern "C" void norrath_skelight_init_weapon(void* s) { order += "weapon;"; if (s != g_obj) order += "BAD;"; }
extern "C" void setParameters__8Creaturefffffi(void* s, float a, float b, float c, float d, float e, int f) {
    order += "params;"; seen_wd = worldDifficulty; args5[0] = a; args5[1] = b; args5[2] = c; args5[3] = d; args5[4] = e; flag_arg = f; if (s != g_obj) order += "BAD;";
}
#include "msg_load.cpp"
int main() {
    int bad = 0;
    alignas(16) static unsigned char obj[0x600], in[0x100];
    g_obj = obj;
    for (int n : {0, 4, 12, 40}) for (unsigned char flag : {0, 1, 0x80, 0xff, 2}) for (int level : {0, 5, -7, 123456}) for (float wd : {0.0f, 1.0f, 2.5f}) {
        std::memset(obj, 0x33, sizeof obj);
        for (unsigned i = 0; i < sizeof in; ++i) in[i] = static_cast<unsigned char>(i * 3 + 7);
        std::memcpy(in + n + 4, &level, 4); in[n + 16] = flag;
        unsigned long long f160 = 0x1122334400000000ull; std::memcpy(obj + 160, &f160, 8);
        unsigned char ref[0x600]; std::memcpy(ref, obj, sizeof ref);
        ref[85] = in[n]; std::memcpy(ref + 1104, in + n + 4, 4); std::memcpy(ref + 960, in + n + 8, 4); std::memcpy(ref + 208, in + n + 12, 4);
        if (flag != 0) { unsigned long long v = f160 | 0x400000ull; std::memcpy(ref + 160, &v, 8); }
        order.clear(); base_count = n; worldDifficulty = wd; seen_wd = -1.0f; flag_arg = -1;
        const int r = msg_load__8SkelightPc(obj, reinterpret_cast<char*>(in));
        if (r != n + 17 || order != "base;weapon;params;" || seen_wd != static_cast<float>(level) || worldDifficulty != wd) bad = 1;
        for (float f : args5) if (f != 1.0f) bad = 1;
        if (flag_arg != 0) bad = 1;
        if (std::memcmp(obj, ref, sizeof obj)) bad = 1;
    }
    return bad;
}
