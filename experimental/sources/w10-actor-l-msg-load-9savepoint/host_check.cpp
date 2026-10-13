// Host contract for msg_load__9SavepointPc; not an EE input. Callees log in order. Words that
// hold addresses are offsets from a host base. Both branches of the word at +128 are covered,
// as are signed byte values, the 64-bit mask and the returned count of 6.
#define W10_ACTOR_L_HOST 1
#include <cstring>
#include <initializer_list>
#include <string>
unsigned char* w10_actor_l_host_base;
unsigned w10_actor_l_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
static std::string calls; static unsigned char* g_o; static unsigned long long mask_to_return; static int ret_hdr = 0;
extern "C" void lightActivate__FP5Light(void* l) { calls += "light" + std::to_string(static_cast<unsigned char*>(l) - g_o) + ";"; }
extern "C" unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* m, int w) {
    calls += "chunks(" + std::to_string(static_cast<unsigned char*>(m) - w10_actor_l_host_base) + "," + std::to_string(w) + ");"; return mask_to_return; }
extern "C" char* lumpFindResource__FPcT0(char* a, char* b) { calls += std::string("find(") + a + "," + b + ");"; return reinterpret_cast<char*>(0x88); }
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* s, void* h, int c) {
    calls += "anim(" + std::to_string(static_cast<unsigned char*>(s) - g_o) + "," + std::to_string(reinterpret_cast<unsigned long>(h)) + "," + std::to_string(c) + ");"; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls += "run" + std::to_string(o == g_o) + ";"; }
#include "msg_load.cpp"
alignas(16) static unsigned char arena[0x1000], ref[0x1000], buf[16];
int main() {
    w10_actor_l_host_base = arena; unsigned char* const o = arena + 0x100; g_o = o; int bad = 0;
    for (int w : {0, 1, -1, 0x12345678}) for (int b4 : {0, 5, -3}) for (int b5 : {0, 9, -128}) {
        std::memset(arena, 0x61, sizeof arena); calls.clear(); mask_to_return = 0x1122334455667788ull;
        unsigned grp = 0x800, model = 0xa00; std::memcpy(o + 36, &grp, 4); std::memcpy(arena + grp + 12, &model, 4);
        std::memcpy(ref, arena, sizeof arena);
        std::memset(buf, 0x7e, sizeof buf); std::memcpy(buf, &w, 4); buf[4] = static_cast<unsigned char>(b4); buf[5] = static_cast<unsigned char>(b5);
        const int r = msg_load__9SavepointPc(o, reinterpret_cast<char*>(buf));
        std::memcpy(ref + 0x100 + 128, &w, 4);
        std::string want;
        if (w) {
            want = "light136;chunks(2560,1);find(savepnt.lmp,float.anm);anim(76,136,3);run1;";
            ref[0x100 + 196] = 1; ref[0x100 + 80] = 0; std::memcpy(ref + 0x100 + 120, &mask_to_return, 8);
        } else want = "run1;";
        ref[0x100 + 197] = w ? 1 : static_cast<unsigned char>(b4); ref[0x100 + 198] = static_cast<unsigned char>(b5);
        if (r != 6 || calls != want || std::memcmp(arena, ref, sizeof arena)) bad = 1;
        for (int i = 6; i < 16; ++i) if (buf[i] != 0x7e) bad = 1;
    }
    return bad;
}
