// Host contract for msg_draw__8GutChunk; not an EE input. Tries blood level, network mode and the low bits of the
// word at +584 against the rule in msg_draw.cpp. The authored function was also compared with the original in a
// private R5900 run. Returns non-zero when wrong.
#include "msg_draw.cpp"
#include <cstring>
#include <initializer_list>
int g_BloodLevel;
signed char netNetworkMode;
static int drawn;
static void* drawnObject;
extern "C" void msg_draw__6PhProp(void* s) { ++drawn; drawnObject = s; }
int main() {
    alignas(16) static unsigned char object[1024];
    int bad = 0;
    for (int blood : {0, 1, -1}) for (int net : {0, 1, -1, 0x7f}) for (unsigned w : {0u, 1u, 2u, 3u, 0xfffffffeu, 0xffffffffu}) {
        g_BloodLevel = blood; netNetworkMode = static_cast<signed char>(net);
        std::memset(object, 0x5a, sizeof object); std::memcpy(object + 584, &w, 4);
        drawn = 0; drawnObject = nullptr;
        msg_draw__8GutChunk(object);
        const bool want = !(blood == 0 && net != 0) && (w & 1) == 0;
        if (drawn != (want ? 1 : 0) || (want && drawnObject != object)) bad = 1;
    }
    return bad;
}
