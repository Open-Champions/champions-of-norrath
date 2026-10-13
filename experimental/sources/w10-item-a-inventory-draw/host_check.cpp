// Host check for inventoryDraw__Fi with recording callees. Not an EE plan input.
#include "inventory_draw.cpp"
#include <string>
unsigned char player[3 * 23984];
unsigned char inventory[464] __attribute__((aligned(4)));
static std::string ev;
static int flip = -1;
static void w32(unsigned char* p, int v) { *reinterpret_cast<int*>(p) = v; }
extern "C" {
void displayGold__FP12playerStructP12screenStruct(unsigned char* r, unsigned char*) { ev += "G" + std::to_string((r - player) / 23984); }
void displayWeight__FP12playerStructP12screenStruct(unsigned char* r, unsigned char* s) {
    ev += "W" + std::to_string((r - player) / 23984);
    if (flip >= 0) w32(s + 76, flip);
}
void doHelpfulMessage__FiiP12screenStructiiPQ24item11description(int a, int b, unsigned char* s, int c, int d, void* e) {
    ev += "H" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string((s - inventory) / 116) + "," + std::to_string(c) + "," + std::to_string(d) + "," + std::to_string(e == nullptr);
}
}
int main() {
    w32(inventory + 116 + 52, 2); w32(inventory + 116 + 76, 0);
    w32(inventory + 232 + 52, 1); w32(inventory + 232 + 76, 2);
    inventoryDraw__Fi(1);
    if (ev != "G2W2") return 1;                 // mode 0: gold and weight, then stop
    ev.clear();
    inventoryDraw__Fi(2);
    if (ev != "H2,1,2,-2,1,1") return 2;        // mode 2: helpful message only
    ev.clear(); flip = 2;
    inventoryDraw__Fi(1);
    if (ev != "G2W2H1,2,1,-2,1,1") return 3;    // mode becomes 2 during the draw
    return 0;
}
