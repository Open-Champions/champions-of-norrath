// Host contract for msg_draw__15FrostStormSpell; not an EE input. The object must be left untouched. The authored function was also
// compared with the original in a private R5900 run (6 scenarios, every original word covered).
#include "msg_draw.cpp"
int main() {
    unsigned char object[64];
    for (int i = 0; i < 64; ++i) object[i] = static_cast<unsigned char>(i * 5 + 3);
    msg_draw__15FrostStormSpell(object);
    for (int i = 0; i < 64; ++i) if (object[i] != static_cast<unsigned char>(i * 5 + 3)) return 1;
    return 0;
}
