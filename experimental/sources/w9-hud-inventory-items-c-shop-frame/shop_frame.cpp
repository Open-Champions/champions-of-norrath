// Observed shopFrame__Fv at 0x001b2248, 876 bytes: the per-frame task of the shop
// screen (shopKick removes it). Lump names are "inventry.lmp" (0x00466c00),
// "shop.lmp" (0x00466c10) and the resource "front.tex" (0x00466c20). `shop` is
// the 464-byte shop record; it holds two 116-byte sub-records, each with a signed
// word at +52 (a player number, negative when unused) and a word at +76 (a state;
// the value 2 is tested). 0x80808080 is the color word passed to every draw.
//  1. When lumpQuery("inventry.lmp") is zero: inventoryLoad() runs; if the lump is
//     still missing the frame ends; otherwise shopUnpackToGrid(0, 0, 1) runs and
//     the frame ends.
//  2. With nmPlayers == 1 or netNetworkMode != 0: shopRun(0), then when the lump is
//     present marble is drawn twice (x 0, y 111 + i * height, i = 0, 1, where
//     height is the halfword at marble+2; mode 0).
//  3. Otherwise, with nmPlayers >= 2 (netNetworkMode is 0): shopRun(1), shopRun(0);
//     for the player numbers at shop+52 and shop+168, when non-negative and the
//     float at record+172 of that player is <= 0.0, the word at shop+116*player+76
//     becomes 2. Then, when the lump is present, marble is drawn twice at x 0 as
//     above; if the word at shop+192 is 2, inventoryDrawPlayerColor(shop[52], 0, 1)
//     is called; otherwise inventoryDrawPlayerColor(shop[52], 0, 1) and
//     inventoryDrawPlayerColor(shop[168], 1, 1) are called, marble is drawn twice
//     at x 320 and bevel is drawn once at (314, 0) with mode 1.
//  4. Always: shopIntro becomes 0 when drawFrameNm - startFrame is exactly 20 (it
//     is stored back either way). Then, with nmPlayers == 1 or the word at
//     shop+192 equal to 2, front.tex from shop.lmp is drawn at (320, 0) with mode 1;
//     else when the word at shop+76 is 2 it is drawn with textureDrawSuperColor
//     (0, 0, 1, color, 0, 0, 640, 448, 1, 0). Otherwise nothing more.
// Unknown: what the sub-record fields hold and the meaning of the draw arguments
// beyond their values.
#include "../inventory-items-record/record.hpp"
extern "C" {
extern int nmPlayers;
extern signed char netNetworkMode;
extern int drawFrameNm;
extern int startFrame;
extern int shopIntro;
extern unsigned char shop[];
extern unsigned char* marble;
extern unsigned char* bevel;
int lumpQuery__FPc(char* name);
void inventoryLoad__Fv();
void shopUnpackToGrid__Fiii(int a, int b, int c);
void shopRun__Fi(int a);
void inventoryDrawPlayerColor__Fiii(int player, int a, int b);
char* lumpFindResource__FPcT0(char* lump, char* name);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int mode, unsigned color);
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void* texture, int a, int b, int c, unsigned color, int d, int e, int f,
                                                   int g, char h, char i);
}

namespace {
char inventry_name[] = "inventry.lmp";
char shop_name[] = "shop.lmp";
char front_name[] = "front.tex";
const unsigned color = 0x80808080u;
int& shop_word(unsigned offset) { return *reinterpret_cast<int*>(shop + offset); }
void mark_dead(int slot) {
    if (slot >= 0) {
        if (*reinterpret_cast<const float*>(inventory_items::player_record(slot) + 172) <= 0.0f) shop_word(116 * slot + 76) = 2;
    }
}
void draw_marble(int x) {
    for (int row = 0; row < 2; ++row) {
        textureDrawColor__FP8_textureiiiUi(marble, x, 111 + row * *reinterpret_cast<short*>(marble + 2), 0, color);
    }
}
}

extern "C" void shopFrame__Fv() {
    if (lumpQuery__FPc(inventry_name) == 0) {
        inventoryLoad__Fv();
        if (lumpQuery__FPc(inventry_name) != 0) shopUnpackToGrid__Fiii(0, 0, 1);
        return;
    }
    if (nmPlayers == 1 || netNetworkMode != 0) {
        shopRun__Fi(0);
        if (lumpQuery__FPc(inventry_name) != 0) draw_marble(0);
    } else if (nmPlayers >= 2) {
        shopRun__Fi(1);
        shopRun__Fi(0);
        mark_dead(shop_word(52));
        mark_dead(shop_word(168));
        if (lumpQuery__FPc(inventry_name) != 0) {
            draw_marble(0);
            if (shop_word(192) == 2) {
                inventoryDrawPlayerColor__Fiii(shop_word(52), 0, 1);
            } else {
                inventoryDrawPlayerColor__Fiii(shop_word(52), 0, 1);
                inventoryDrawPlayerColor__Fiii(shop_word(168), 1, 1);
                draw_marble(320);
                textureDrawColor__FP8_textureiiiUi(bevel, 314, 0, 1, color);
            }
        }
    }
    if (drawFrameNm - startFrame == 20) shopIntro = 0;
    if (nmPlayers == 1 || shop_word(192) == 2) {
        textureDrawColor__FP8_textureiiiUi(lumpFindResource__FPcT0(shop_name, front_name), 320, 0, 1, color);
    } else if (shop_word(76) == 2) {
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(lumpFindResource__FPcT0(shop_name, front_name), 0, 0, 1, color, 0, 0, 640, 448, 1, 0);
    }
}
