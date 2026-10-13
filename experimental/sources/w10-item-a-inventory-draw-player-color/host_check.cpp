// Host check for inventoryDrawPlayerColor__Fiii. Not an EE plan input.
#include "draw_player_color.cpp"
#include <string>
static std::string ev;
extern "C" {
void* lumpFindResource__FPcT0(char* lump, char* name) { ev += std::string(lump) + "/" + name + ";"; return reinterpret_cast<void*>(0x77); }
void textureDrawColor__FP8_textureiiiUi(void* t, int x, int y, int a, unsigned c) {
    ev += "D" + std::to_string(t == reinterpret_cast<void*>(0x77)) + "," + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(a) + "," + std::to_string(c) + ";";
}
}
int main() {
    inventoryDrawPlayerColor__Fiii(0, 0, 1);
    if (ev != "shop.lmp/invtagred.tex;D1,131,21,1,2155905152;") return 1;
    ev.clear(); inventoryDrawPlayerColor__Fiii(3, 1, 0);
    if (ev != "inventry.lmp/invtagblue.tex;D1,451,21,1,2155905152;") return 2;
    ev.clear(); inventoryDrawPlayerColor__Fiii(1, 2, 7);
    if (ev != "shop.lmp/invtaggreen.tex;D1,771,21,1,2155905152;") return 3;
    ev.clear(); inventoryDrawPlayerColor__Fiii(2, 0, 0);
    if (ev != "inventry.lmp/invtagyellow.tex;D1,131,21,1,2155905152;") return 4;
    ev.clear(); inventoryDrawPlayerColor__Fiii(4, 0, 0); inventoryDrawPlayerColor__Fiii(-1, 0, 1);
    if (!ev.empty()) return 5;
    return 0;
}
