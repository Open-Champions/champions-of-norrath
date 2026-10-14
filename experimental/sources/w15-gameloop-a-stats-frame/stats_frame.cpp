#include "../w9-animation-object-c-common/mem.hpp"
// Observed statsFrame__Fv at 0x001bc948, 496 bytes, 160-byte frame. One frame of the character stats screen:
//  1. If lumpQuery("inventry.lmp") is zero it calls inventoryLoad__Fv and returns.
//  2. If the word statsDrawDelay (0x00484a34) is above zero it is decreased by one and the function returns.
//  3. lightSetSceneLights is called with a direction (-0.5, -0.5, 0.5) passed as a pointer to a local copy and the
//     integers 101, 107, 78, 0, 38, 40, 48, 0, 128.
//  4. idx is the word at statsScreen + 52 (statsScreen is the 232-byte object at 0x004281a8) and entry is the 23984-byte
//     record player[idx]. When the signed byte at entry + 241 is -1 the function returns.
//  5. Otherwise a view is set up: position (0, -108, 0) and a matrix that is the identity rotated by matRot(0, 16384)
//     and matRot(2, 0), given to worldSetViewMatricies. inventoryDrawPlayer(slot, 1, &animState[slot], 0) draws the
//     player whose slot is the signed byte at entry + 241 (animState is the 40-byte-stride array at 0x007d0440).
//  6. textureDrawColor(marble, 0, i * (halfword at marble + 2), 0, 0x80808080) for i = 0, 1, 2, marble re-read each
//     time, then textureDrawColor(stage, 320, 0, 0, 0x80808080).
//  7. The word at statsScreen + 52 is replaced with the signed byte at player[idx] + 241 read again.
// Inferred: the screen shows one player over a tiled background; the byte at +241 is a player slot index (-1 for none)
// and the final store keeps the shown slot. Names of the stack arguments of the light call are not claimed.
// Unknown: the layouts of statsScreen beyond +52, of the player record beyond +241, and the light call's parameters.
extern "C" {
extern unsigned char statsScreen[];
extern unsigned char player[];
extern int statsDrawDelay;
extern unsigned marble;
extern unsigned stage;
extern unsigned char animState[] __asm__("norrath_stats_screen_view_7d0440");   // the image has four objects named animState

void* lumpQuery__FPc(const char* name);
void inventoryLoad__Fv();
void lightSetSceneLights__FG6Point3iiiiiiiii(const void* direction, int a, int b, int c, int d, int e, int f, int g, int h, int i);
void matIdent__FP8Matrix34(void* matrix);
void matRot__FP8Matrix34is(void* matrix, int axis, short angle);
void worldSetViewMatricies__FP6Point3P8Matrix34(const void* position, void* matrix);
void inventoryDrawPlayer__FiiP14AnimationStatei(int slot, int a, void* animation, int b);
void textureDrawColor__FP8_textureiiiUi(void* texture, int x, int y, int z, unsigned color);

void statsFrame__Fv() {
    using namespace norrath_aoc;
    if (lumpQuery__FPc("inventry.lmp") == nullptr) {
        inventoryLoad__Fv();
        return;
    }
    if (statsDrawDelay > 0) {
        --statsDrawDelay;
        return;
    }
    const float direction[3] = {-0.5f, -0.5f, 0.5f};
    lightSetSceneLights__FG6Point3iiiiiiiii(direction, 101, 107, 78, 0, 38, 40, 48, 0, 128);
    const int index = static_cast<int>(load_word(statsScreen + 52));
    unsigned char* entry = player + 23984 * index;
    if (load_signed_byte(entry + 241) == -1) return;
    const float position[3] = {0.0f, -108.0f, 0.0f};
    float matrix[12];
    matIdent__FP8Matrix34(matrix);
    matRot__FP8Matrix34is(matrix, 0, 16384);
    matRot__FP8Matrix34is(matrix, 2, 0);
    worldSetViewMatricies__FP6Point3P8Matrix34(position, matrix);
    const int slot = load_signed_byte(entry + 241);
    inventoryDrawPlayer__FiiP14AnimationStatei(slot, 1, animState + 40 * slot, 0);
    for (int i = 0; i < 3; ++i) {
        unsigned char* texture = load_pointer(reinterpret_cast<unsigned char*>(&marble));
        textureDrawColor__FP8_textureiiiUi(texture, 0, i * load_signed_half(texture + 2), 0, 0x80808080u);
    }
    textureDrawColor__FP8_textureiiiUi(load_pointer(reinterpret_cast<unsigned char*>(&stage)), 320, 0, 0, 0x80808080u);
    store_word(statsScreen + 52, static_cast<unsigned>(load_signed_byte(player + 23984 * index + 241)));
}
}
