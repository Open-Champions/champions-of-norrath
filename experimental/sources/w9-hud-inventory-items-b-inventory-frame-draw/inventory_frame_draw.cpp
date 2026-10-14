// Observed inventoryFrameDraw__Fv at 0x001ad870, 1152 bytes.
// The per-frame draw of the inventory screens. `inventory` (0x00427288) is an
// array of 116-byte screen records; the word at +52 of a record is a player index
// (call it its player) and the word at +76 is a mode value of which only the
// value 2 is tested. animState (0x007cc5c8; the name is also used by other file-local objects) is an array of 264-byte
// AnimationState records indexed by player. Steps:
//  1. inventoryLump zero: call inventoryLoad__Fv and stop.
//  2. inventoryDrawDelay (0x00484830) positive: decrement it and stop.
//  3. lightSetSceneLights__FG6Point3iiiiiiiii with the vector (-0.5, -0.5, 0.5) and
//     the integers 101, 107, 78, 0, 38, 40, 48, 0, 128.
//  4. localNmPlayersAlive (0x00484f48) equal to 1: set the view with
//     worldSetViewMatricies__FP6Point3P8Matrix34 (position (0, -108, 0), identity
//     matrix rotated by matRot__FP8Matrix34is with axis 0 by 16384, then axis 2 by
//     0), then inventoryDrawPlayer__FiiP14AnimationStatei(player of record 0, 1,
//     &animState[player], 0), then textureDrawColor__FP8_textureiiiUi on invHeader
//     at (0,0,0), marble at (0,105,0) and stage at (319,0,0), all with color
//     0x80808080, then inventoryDraw__Fi(0), and stop.
//  5. Otherwise (any other count):
//     a. If record 1's mode is 2: set the same view, call
//        inventoryDrawPlayerColor__Fiii(player of record 0, 0, 0),
//        inventoryDrawPlayer(player of record 0, 1, &animState[...], 0) and draw
//        stage at (319,0,0).
//        Otherwise: inventoryDrawPlayerColor(player of record 0, 0, 0) and
//        inventoryDrawPlayerColor(player of record 1, 1, 0), then draw invHeader
//        and marble at (320,0,0) and (320,105,0).
//     b. If record 0's mode is 2: set the same view, call inventoryDrawPlayer(
//        player of record 1, 2, &animState[...], 1) and
//        textureDrawSuperColor__FP8_textureiiiUiiiiicc(stage, 0, 1, 0, color, 0, 0,
//        640, 512, 1, 0).
//        Otherwise draw invHeader at (0,0,0) and marble at (0,105,0).
//     c. If neither record's mode is 2: draw bevel at (314,0,6).
//     d. Set the same view again, inventoryDraw__Fi(1), then inventoryDraw__Fi(0)
//        if the byte interfaceOn (0x00483f45) is nonzero.
// What the texture globals contain (invHeader, marble, stage, bevel are word
// pointers in the small-data area), what each trailing integer of the draw calls
// means and the meaning of the mode values are not established. The callees and
// objects are not defined here.
struct Point3 { float x, y, z; };
struct Matrix34 { float m[12]; };

extern "C" {
extern void* inventoryLump;
extern int inventoryDrawDelay;
extern int localNmPlayersAlive;
extern signed char interfaceOn;
extern unsigned char inventory[] __attribute__((aligned(4)));
extern unsigned char animState[];
extern void* invHeader;
extern void* marble;
extern void* stage;
extern void* bevel;
int inventoryLoad__Fv();
void lightSetSceneLights__FG6Point3iiiiiiiii(Point3, int, int, int, int, int, int, int, int, int);
void matIdent__FP8Matrix34(Matrix34*);
void matRot__FP8Matrix34is(Matrix34*, int, short);
void worldSetViewMatricies__FP6Point3P8Matrix34(Point3*, Matrix34*);
void inventoryDrawPlayer__FiiP14AnimationStatei(int, int, void*, int);
void inventoryDrawPlayerColor__Fiii(int, int, int);
void textureDrawColor__FP8_textureiiiUi(void*, int, int, int, unsigned);
void textureDrawSuperColor__FP8_textureiiiUiiiiicc(void*, int, int, int, unsigned, int, int, int, int, char, char);
void inventoryDraw__Fi(int);
}

static const unsigned kColor = 0x80808080u;

static int recordWord(int record, int offset) {
    return *reinterpret_cast<int*>(inventory + record * 116 + offset);
}

static inline __attribute__((always_inline)) void setView() {
    Point3 position = {0.0f, -108.0f, 0.0f};
    Matrix34 matrix;
    matIdent__FP8Matrix34(&matrix);
    matRot__FP8Matrix34is(&matrix, 0, 16384);
    matRot__FP8Matrix34is(&matrix, 2, 0);
    worldSetViewMatricies__FP6Point3P8Matrix34(&position, &matrix);
}

extern "C" void inventoryFrameDraw__Fv() {
    if (inventoryLump == nullptr) {
        inventoryLoad__Fv();
        return;
    }
    if (inventoryDrawDelay > 0) {
        inventoryDrawDelay = inventoryDrawDelay - 1;
        return;
    }
    Point3 lightDirection = {-0.5f, -0.5f, 0.5f};
    lightSetSceneLights__FG6Point3iiiiiiiii(lightDirection, 101, 107, 78, 0, 38, 40, 48, 0, 128);
    if (localNmPlayersAlive == 1) {
        setView();
        int p = recordWord(0, 52);
        inventoryDrawPlayer__FiiP14AnimationStatei(p, 1, animState + p * 264, 0);
        textureDrawColor__FP8_textureiiiUi(invHeader, 0, 0, 0, kColor);
        textureDrawColor__FP8_textureiiiUi(marble, 0, 105, 0, kColor);
        textureDrawColor__FP8_textureiiiUi(stage, 319, 0, 0, kColor);
        inventoryDraw__Fi(0);
        return;
    }
    if (recordWord(1, 76) == 2) {
        setView();
        inventoryDrawPlayerColor__Fiii(recordWord(0, 52), 0, 0);
        int p = recordWord(0, 52);
        inventoryDrawPlayer__FiiP14AnimationStatei(p, 1, animState + p * 264, 0);
        textureDrawColor__FP8_textureiiiUi(stage, 319, 0, 0, kColor);
    } else {
        inventoryDrawPlayerColor__Fiii(recordWord(0, 52), 0, 0);
        inventoryDrawPlayerColor__Fiii(recordWord(1, 52), 1, 0);
        textureDrawColor__FP8_textureiiiUi(invHeader, 320, 0, 0, kColor);
        textureDrawColor__FP8_textureiiiUi(marble, 320, 105, 0, kColor);
    }
    if (recordWord(0, 76) == 2) {
        setView();
        int p = recordWord(1, 52);
        inventoryDrawPlayer__FiiP14AnimationStatei(p, 2, animState + p * 264, 1);
        textureDrawSuperColor__FP8_textureiiiUiiiiicc(stage, 0, 1, 0, kColor, 0, 0, 640, 512, 1, 0);
    } else {
        textureDrawColor__FP8_textureiiiUi(invHeader, 0, 0, 0, kColor);
        textureDrawColor__FP8_textureiiiUi(marble, 0, 105, 0, kColor);
    }
    if (recordWord(0, 76) != 2 && recordWord(1, 76) != 2) {
        textureDrawColor__FP8_textureiiiUi(bevel, 314, 0, 6, kColor);
    }
    setView();
    inventoryDraw__Fi(1);
    if (interfaceOn != 0) inventoryDraw__Fi(0);
}
