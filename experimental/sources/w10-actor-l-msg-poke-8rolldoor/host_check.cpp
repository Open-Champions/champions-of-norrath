// Host contract for msg_poke__8RollDoori; not an EE input. Callees log in order; the flag word
// loses only bit 20; nothing else changes on the negative path.
#include <cstring>
#include <initializer_list>
#include <string>
static std::string calls; static void* sound_self; static float sound_rate;
extern "C" void objectAddToRunList__FP10GameObject(void*) { calls += "add;"; }
extern "C" char* lumpFindResource__FPcT0(char* lump, char* name) { calls += std::string("find(") + lump + "," + name + ");"; return reinterpret_cast<char*>(0x77); }
extern "C" int soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* h, void* o, float r) {
    calls += "play(" + std::to_string(reinterpret_cast<unsigned long>(h)) + ");"; sound_self = o; sound_rate = r; return 0;
}
#include "msg_poke.cpp"
alignas(16) static unsigned char o[128], ref[128];
int main() {
    int bad = 0;
    for (short h : {short(0), short(5), short(-1), short(-32768), short(32767)}) for (int arg : {0, 3}) {
        std::memset(o, 0x44, sizeof o); calls.clear(); sound_self = nullptr; sound_rate = 0;
        std::memcpy(o + 76, &h, 2); const unsigned fl = 0xffffffffu; std::memcpy(o + 12, &fl, 4);
        std::memcpy(ref, o, sizeof o);
        msg_poke__8RollDoori(o, arg);
        const unsigned fl2 = 0xffefffffu; std::memcpy(ref + 12, &fl2, 4);
        if (h >= 0) {
            const short one = 1; std::memcpy(ref + 90, &one, 2);
            if (calls != "add;find(fx.lmp,door_stn_open.anm);play(119);" || sound_self != o || sound_rate != 1.0f) bad = 1;
        } else if (!calls.empty()) bad = 1;
        if (std::memcmp(o, ref, sizeof o)) bad = 1;
    }
    return bad;
}
