// Observed drawAutoMap__Fv at 0x001cabd0, 2328 bytes (a local symbol; called from the frame code).
// Early outs, each ending the function: when netNetworkMode is zero, validLocalPlayer < 0 or the object word at
// +21872 of that 23984-byte player record is null; the word at +108 or +128 of world is zero.
// Button step, when gameIsPaused() is zero, cutMode is zero and nmPlayers > 0 (the count is read once): for each
// player p, skipped when (the +21872 object word is null, or its word at +12 has bit 8 set) and netNetworkMode
// was zero. Otherwise, with b = automapButton[p] and r = the 88-byte netLocalPad record p: if r.word12 & b,
// mapButtonWasDown[p] = 1. If r.word8 & b: the float at r+32 is compared as a double against 0.9 and then -0.9; above
// 0.9 scaleScale = (float)((double)scaleScale * 0.99009900990099) and below -0.9 it is multiplied by 1.01
// (the file doubles 0x3feccccccccccccc, 0x3fefaee41e6a7499, 0xbfeccccccccccccc and 0x3ff028f5c28f5c28), and each
// clears mapButtonWasDown[p]; then scaleScale above 2.0 becomes 2.0f and below 0.5 becomes 0.5f. After that, if
// mapButtonWasDown[p] is set and r.word8 & b is clear, gameAutoMapMode = (gameAutoMapMode + 1) % 3, except that
// with four players a result of 1 becomes 2, and mapButtonWasDown[p] = 0.
// Later gates, each ending the function: MC_GetAsyncMode() == 3; engineTaskActive("deathTask") nonzero;
// MC_GetAsyncMode() == 7; the word at +52 of gameCam not equal to 1.
// Easing: with mode = gameAutoMapMode and k = 6 when nmPlayers >= 2, else 0, the targets are loc[2*mode + k] and
// loc[2*mode + k + 1] for mapX and mapY, scale[mode] for mapScale, scissor[2*mode + k] and [2*mode + k + 1] for
// mapScissorX and mapScissorY. For each value: when it differs from the target and the target minus the value
// has an absolute value below 0.005 (mapX, mapY), 0.002 (the others), the value becomes the target, else it
// moves a quarter of the way (value + (target - value) * 0.25). The thresholds are the file words 0x3ba3d70a (twice)
// and 0x3b03126e (three times).
// Then map::revealMap is called with a copy of the 12 bytes at object+16 of each player whose +21872 object word is
// non-null. Then, with t = gameCam->GetTargetNum(): t < 0 ends the function; the float at +172 of player t below
// zero ends it; a null +21872 object word of player t ends it (GetTargetNum is called again for each of these).
// A 2-float point is read from gameCam+24 and +28. If map::willDraw(world, mapScissorX) is nonzero, mapObjList
// is set to a local 200-entry list, a half-size r is 960.0f, or 1920.0f when map::isRevealed() is nonzero, and
// objectFindInBox is called with two corners built from the point (x - r, y - r, z - r) and (x + r, y + r,
// z + r) where z is the float at +24 of the target player's object, and the list. In a network game
// (netNetworkMode != 0) the list is then filtered to drop entries whose word at +12 has bit 0x40 set,
// the netGetLocalObject() of each player's object word is appended, and the list is copied back and terminated
// with a null entry.
// Finally map::draw is called with world, mapX, mapY, mapScale * scaleScale, mapScissorX, mapScissorY,
// cursorSize[mode], iconSize[mode], a copy of the point, drawCharacterCallBack__FiP6Point3PsPiN23, and the
// result of gameCam->GetYaw().
// The soft double operations are called through the helpers below, which stand for the original routines at the
// named addresses: fptodp (0x002fb320), dpcmp (0x002fba48), dpmul (0x002fb680) and dptofp (0x002fbc20).
// Static tables (local symbols in the original read-only data): loc (0x0046cc90, 12 floats), scale
// (0x0046ccc0, 3), iconSize (0x0046ccd0, 3), cursorSize (0x0046cce0, 3) and scissor (0x0046ccf0, 12).
// Inferred: word12 of the pad record is a pressed bit set and word8 a held bit set, from how they are used.
// Unknown: the meaning of the pad record beyond those fields, what the player +21872 object is, what
// the +12 word flag bits mean, and the argument roles of map::draw beyond their positions.
struct Point2 {
    float x, y;
};
struct Point3 {
    float x, y, z;
};
struct AutoMapPadRecord {
    unsigned char before[8];
    unsigned held;
    unsigned pressed;
    unsigned char between[16];
    float stick;
    unsigned char after[48];
};
struct AutoMapWorld {
    unsigned char before[108];
    int flagA;
    unsigned char between[16];
    void* record;
};

extern "C" signed char netNetworkMode;
extern "C" int validLocalPlayer;
extern "C" unsigned char player[];
extern "C" AutoMapWorld* world;
extern "C" signed char cutMode;
extern "C" int nmPlayers;
extern "C" int mapButtonWasDown[];
extern "C" unsigned automapButton[];
extern "C" AutoMapPadRecord netLocalPad[];
extern "C" float scaleScale;
extern "C" int gameAutoMapMode;
extern "C" float mapX;
extern "C" float mapY;
extern "C" float mapScale;
extern "C" float mapScissorX;
extern "C" float mapScissorY;
extern "C" void* gameCam;
extern "C" void** mapObjList;

extern "C" int gameIsPaused__Fv();
extern "C" int MC_GetAsyncMode__Fv();
extern "C" int engineTaskActive__FPc(const char* name);
extern "C" int GetTargetNum__6Camera(void* camera);
extern "C" int GetYaw__6Camera(void* camera);
extern "C" void revealMap__3mapG6Point3(const void* point);
extern "C" int willDraw__3mapP12_worldHeaderf(AutoMapWorld* header, float value);
extern "C" int isRevealed__3mapv();
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** list, int count);
extern "C" void* netGetLocalObject__FP8Creature(void* creature);
extern "C" int drawCharacterCallBack__FiP6Point3PsPiN23(int, void*, short*, int*, int*, int*);
extern "C" void draw__3mapP12_worldHeaderfffffffG6Point2PFiP6Point3PsPiPiPi_is(
    AutoMapWorld* header, float a, float b, float c, float d, float e, float f, float g, const void* point,
    int (*callback)(int, void*, short*, int*, int*, int*), int yaw);

extern "C" unsigned long long norrath_entity_fptodp(float value);
extern "C" int norrath_softdouble_compare_2fba48(unsigned long long left, unsigned long long right);
extern "C" unsigned long long norrath_softdouble_multiply_2fb680(unsigned long long left, unsigned long long right);
extern "C" float norrath_softdouble_to_float_2fbc20(unsigned long long value);

static const float loc[12] = {0.0f, 0.0f, 200.0f, -120.0f, 180.0f, -120.0f, 0.0f, 0.0f, 0.0f, -120.0f, 0.0f, -140.0f};
static const float scale[3] = {0.9f, 0x1.333332p-1f, 0.0f};
static const float iconSize[3] = {16.0f, 10.0f, 10.0f};
static const float cursorSize[3] = {6.0f, 5.0f, 5.0f};
static const float scissor[12] = {320.0f, 224.0f, 120.0f, 100.0f, 0.0f, 0.0f, 320.0f, 224.0f, 120.0f, 100.0f, 0.0f, 0.0f};

static inline unsigned char* playerObject(int index) {
    return *(unsigned char**)(player + index * 23984 + 21872);
}

static void ease(float& value, float target, float limit) {
    float current = value;
    if (current == target) return;
    float delta = target - current;
    float magnitude = delta < 0.0f ? -delta : delta;
    if (magnitude < limit)
        value = target;
    else
        value = current + delta * 0.25f;
}

extern "C" void drawAutoMap__Fv() {
    if (netNetworkMode == 0) {
        if (validLocalPlayer < 0) return;
        if (playerObject(validLocalPlayer) == 0) return;
    }
    if (world->flagA == 0) return;
    if (world->record == 0) return;

    if (!gameIsPaused__Fv() && cutMode == 0 && nmPlayers > 0) {
        int count = nmPlayers;
        int network = netNetworkMode;
        for (int p = 0; p < count; ++p) {
            unsigned char* object = playerObject(p);
            if ((object == 0 || (*(unsigned*)(object + 12) & 8)) && network == 0) continue;
            unsigned button = automapButton[p];
            AutoMapPadRecord& record = netLocalPad[p];
            if (record.pressed & button) mapButtonWasDown[p] = 1;
            if (record.held & button) {
                if (norrath_softdouble_compare_2fba48(norrath_entity_fptodp(record.stick), 0x3feccccccccccccc) > 0) {
                    scaleScale = norrath_softdouble_to_float_2fbc20(norrath_softdouble_multiply_2fb680(
                        norrath_entity_fptodp(scaleScale), 0x3fefaee41e6a7499));
                    mapButtonWasDown[p] = 0;
                }
                if (norrath_softdouble_compare_2fba48(norrath_entity_fptodp(record.stick), 0xbfeccccccccccccc) < 0) {
                    scaleScale = norrath_softdouble_to_float_2fbc20(norrath_softdouble_multiply_2fb680(
                        norrath_entity_fptodp(scaleScale), 0x3ff028f5c28f5c28));
                    mapButtonWasDown[p] = 0;
                }
                if (norrath_softdouble_compare_2fba48(norrath_entity_fptodp(scaleScale), 0x4000000000000000) > 0)
                    scaleScale = 2.0f;
                if (norrath_softdouble_compare_2fba48(norrath_entity_fptodp(scaleScale), 0x3fe0000000000000) < 0)
                    scaleScale = 0.5f;
            }
            if (mapButtonWasDown[p] == 0) continue;
            if (record.held & automapButton[p]) continue;
            int next = (gameAutoMapMode + 1) % 3;
            if (count != 4)
                gameAutoMapMode = next;
            else
                gameAutoMapMode = next == 1 ? 2 : next;
            mapButtonWasDown[p] = 0;
        }
    }

    if (MC_GetAsyncMode__Fv() == 3) return;
    if (engineTaskActive__FPc("deathTask")) return;
    if (MC_GetAsyncMode__Fv() == 7) return;
    if (*(int*)((unsigned char*)gameCam + 52) != 1) return;

    int mode = gameAutoMapMode;
    int count = nmPlayers;
    int offset = mode * 2 + (count < 2 ? 0 : 6);
    ease(mapX, loc[offset], 0x1.47ae14p-8f);
    ease(mapY, loc[offset + 1], 0x1.47ae14p-8f);
    ease(mapScale, scale[mode], 0x1.0624dcp-9f);
    ease(mapScissorX, scissor[offset], 0x1.0624dcp-9f);
    ease(mapScissorY, scissor[offset + 1], 0x1.0624dcp-9f);

    for (int p = 0; p < count; ++p) {
        unsigned char* object = playerObject(p);
        if (object) {
            Point3 position = *(Point3*)(object + 16);
            revealMap__3mapG6Point3(&position);
            count = nmPlayers;
        }
    }

    if (GetTargetNum__6Camera(gameCam) < 0) return;
    if (*(float*)(player + GetTargetNum__6Camera(gameCam) * 23984 + 172) < 0.0f) return;
    if (playerObject(GetTargetNum__6Camera(gameCam)) == 0) return;

    unsigned char* camera = (unsigned char*)gameCam;
    Point2 point;
    point.x = *(float*)(camera + 24);
    point.y = *(float*)(camera + 28);
    void* list[200];
    if (willDraw__3mapP12_worldHeaderf(world, mapScissorX)) {
        mapObjList = list;
        float half = isRevealed__3mapv() ? 1920.0f : 960.0f;
        Point3 low, high;
        float x = point.x, y = point.y;
        low.x = x - half;
        low.y = y - half;
        low.z = *(float*)(playerObject(GetTargetNum__6Camera(gameCam)) + 24) - half;
        x = point.x;
        y = point.y;
        high.x = x + half;
        high.y = y + half;
        high.z = *(float*)(playerObject(GetTargetNum__6Camera(gameCam)) + 24) + half;
        objectFindInBox__FG6Point3T0PP10GameObjecti(&low, &high, list, 200);
        if (netNetworkMode != 0) {
            void* kept[200];
            int used = 0;
            if (list[0]) {
                for (int i = 0; list[i]; ++i) {
                    if (!(*(unsigned*)((unsigned char*)list[i] + 12) & 0x40)) kept[used++] = list[i];
                }
            }
            int players = nmPlayers;
            for (int p = 0; p < players; ++p) {
                unsigned char* object = playerObject(p);
                if (object) {
                    ++used;
                    players = nmPlayers;
                    kept[used - 1] = netGetLocalObject__FP8Creature(object);
                }
            }
            for (int i = 0; i < used; ++i) list[i] = kept[i];
            list[used] = 0;
        }
    }
    Point2 drawPoint = point;
    draw__3mapP12_worldHeaderfffffffG6Point2PFiP6Point3PsPiPiPi_is(
        world, mapX, mapY, mapScale * scaleScale, mapScissorX, mapScissorY, cursorSize[mode], iconSize[mode],
        &drawPoint, drawCharacterCallBack__FiP6Point3PsPiN23, GetYaw__6Camera(gameCam));
}
