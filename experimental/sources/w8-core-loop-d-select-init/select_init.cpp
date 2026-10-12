// Observed init__Fv at 0x0027d600, 2016 bytes (a local symbol), in the module that holds
// getHairAndSkin__Fi, initPreSelect__Fv, initNamePad__Fv and initLightInfo__Fv. No argument,
// no result. Steps, in image order:
//  1. playerSelectCount (signed byte) becomes 1 if it was 0.
//  2. npcLumpName (a word holding a string address) is "select2.lmp" when
//     strcmp(levelName + 0x51, "kelsel") is 0, otherwise "select.lmp".
//  3. For i = 0..9: object = objectFindObject__FPc(targets[i].name) (the first word of each 636-byte
//     targets record) is stored in targetObjs[i]. If scriptGetGameState__FPc("needPlayers") is nonzero,
//     lumpFindResource__FPcT0(npcLumpName, "<name>_intro1.anm") is looked up and a nonzero result is
//     added with animAdd__FP14AnimationStateP15AnimationHeaderi(object + 288, header, 3).
//     yaws[i] = (short)(-16384 - unsigned16 at object+56). With a = (icos, isin, 0) of the signed
//     16-bit angle at object+56, normalized with the EE rsqrt.s and scaled by 90.0, hpos[i] = object
//     position (+16) + a. b = normalize(position - hpos[i]); c = normalize(cross(b, up)) with
//     up = (0, 0, 1); lightMats[i] = {c, b, up} as nine singles. For i > 0,
//     hvecs[i-1] = normalize(hpos[i] - hpos[i-1]). getHairAndSkin__Fi(i) ends the pass.
//  4. hvecs[9] = normalize(hpos[0] - hpos[9]).
//  5. If scriptGetGameState__FPc("needPlayers") is nonzero: for j = 0..9 changeHairStyle__Fii(j, 0),
//     changeHairColor__Fii(j, 0), changeBeard__Fii(j, 0), changeSkin__Fii(j, 0). Otherwise
//     objectRemoveFromGrid__FP10GameObject is called for the ten targetObjs, then for each of the
//     first nmPlayers (if positive) players k: names "finalpos<k+1>" (objectFindObject), "<n>_bodyv1.vif",
//     "<n>_bodyv<m+1>.tex" and "<n>_intro1.anm" are built with sprintf, where n is
//     targets[playerSelectInfo[k].byte32].name and m the byte at +35 of that record; nameTags[k] =
//     "name=<n>" and pnums[k] = "pnum=<k>"; dummyList[k] = {nameTags[k], pnums[k], 0};
//     blockAlloc__Fi(1144); a nonzero block gets the NPC constructor
//     __3NPCG6Point3sPcN33ifPPc(block, position of finalpos, finalpos yaw, npcLumpName, vif, tex,
//     anm, 0, 60.0f, &dummyList[k]); then flags (64-bit at +160) |= 0x10000000 and
//     objectAddToRunList__FP10GameObject(block). nmPlayers is re-read each pass.
//  6. If strcmp(levelName + 0x51, "kelsel2") is 0: hermiteTime = 0, playerSelectInfo[start].byte32 = 0
//     with start = playerSelectStartPlayer, headerAlpha = hermiteTime, hermDeltaScale = 1.0,
//     needEnforce = playerSelectEnforce, curPlayer = start, flareDelta = hermiteTime,
//     bodyAlpha = hermiteTime, requestToExit = 0, startExitFade = 0, playerSelectSuccessful = 0,
//     initPreSelect__Fv(), initNamePad__Fv(), initLightInfo__Fv(), engineDisable30fps = 1.
// Observed: the image has two local symbols named flareDelta (0x486cb4 and 0x486d44); this routine
// writes the second.
// Unknown: what the screen is called beyond its helper names, the meaning of the two bytes of
// playerSelectInfo that are used, the NPC constructor's parameters other than their types, and the
// buffer sizes (text is 256 bytes here; the image reserves the stack room without naming it).
// The trailing z products of the first vector are kept as written because z is zero there.
extern "C" {
struct Point3 { float x, y, z; };
struct SelectTarget { const char* name; unsigned char rest[632]; };
struct SelectInfo { unsigned char head[32]; signed char byte32; unsigned char gap[2]; signed char byte35; unsigned char tail[5]; };
struct SelectDummy { const char* tag; const char* number; int zero; };
#if defined(__mips__)
static_assert(sizeof(SelectTarget) == 636, "targets stride");
static_assert(sizeof(SelectInfo) == 41, "playerSelectInfo stride");
static_assert(sizeof(SelectDummy) == 12, "dummyList stride");
#endif
extern signed char playerSelectCount, playerSelectStartPlayer, playerSelectEnforce, startExitFade, playerSelectSuccessful;
extern const char* npcLumpName;
extern char levelName[];
extern SelectTarget targets[];
extern unsigned char* targetObjs[];
extern short yaws[];
extern Point3 hpos[];
extern Point3 hvecs[];
extern float lightMats[][9];
extern SelectInfo playerSelectInfo[];
extern char nameTags[][32];
extern char pnums[][8];
extern SelectDummy dummyList[];
extern int nmPlayers, needEnforce, curPlayer, requestToExit, engineDisable30fps;
extern float hermiteTime, headerAlpha, hermDeltaScale, flareDelta, bodyAlpha;

int strcmp(const char*, const char*);
int sprintf(char*, const char*, ...);
unsigned char* objectFindObject__FPc(char*);
int scriptGetGameState__FPc(const char*);
void* lumpFindResource__FPcT0(const char*, char*);
void animAdd__FP14AnimationStateP15AnimationHeaderi(unsigned char*, void*, int);
float icos__Fi(int);
float isin__Fi(int);
void getHairAndSkin__Fi(int);
void changeHairStyle__Fii(int, int);
void changeHairColor__Fii(int, int);
void changeBeard__Fii(int, int);
void changeSkin__Fii(int, int);
void objectRemoveFromGrid__FP10GameObject(unsigned char*);
void* blockAlloc__Fi(int);
void __3NPCG6Point3sPcN33ifPPc(void* self, Point3 position, short yaw, const char* lump, char* vif, char* tex,
                               char* anm, int zero, float scale, SelectDummy* dummy);
void objectAddToRunList__FP10GameObject(void*);
void initPreSelect__Fv();
void initNamePad__Fv();
void initLightInfo__Fv();
}

// The EE reciprocal square root: fd = fs / sqrt(ft).
#if defined(NORRATH_HOST_CONTRACT)
extern float norrath_host_rsqrt(float one, float value);
static inline float reciprocalRoot(float one, float value) { return norrath_host_rsqrt(one, value); }
#else
static inline float reciprocalRoot(float one, float value) {
    float result;
    __asm__("rsqrt.s %0,%1,%2" : "=f"(result) : "f"(one), "f"(value));
    return result;
}
#endif

static inline Point3 normalized(float one, float x, float y, float z) {
    float scale = reciprocalRoot(one, x * x + y * y + z * z);
    Point3 out = {x * scale, y * scale, z * scale};
    return out;
}

extern "C" void init__Fv() {
    const Point3 up = {0.0f, 0.0f, 1.0f};
    const float one = 1.0f;
    if (playerSelectCount == 0) playerSelectCount = 1;
    npcLumpName = strcmp(levelName + 0x51, "kelsel") == 0 ? "select2.lmp" : "select.lmp";
    for (int i = 0; i < 10; ++i) {
        unsigned char* object = objectFindObject__FPc(const_cast<char*>(targets[i].name));
        targetObjs[i] = object;
        if (scriptGetGameState__FPc("needPlayers") != 0) {
            char text[256];
            sprintf(text, "%s_intro1.anm", targets[i].name);
            void* header = lumpFindResource__FPcT0(npcLumpName, text);
            if (header != 0) animAdd__FP14AnimationStateP15AnimationHeaderi(object + 288, header, 3);
        }
        yaws[i] = static_cast<short>(-16384 - *reinterpret_cast<unsigned short*>(object + 56));
        float angleX = icos__Fi(*reinterpret_cast<short*>(object + 56));
        float angleY = isin__Fi(*reinterpret_cast<short*>(object + 56));
        float scaleA = reciprocalRoot(one, angleX * angleX + angleY * angleY);
        float ax = angleX * scaleA * 90.0f;
        float ay = angleY * scaleA * 90.0f;
        float az = 0.0f * scaleA * 90.0f;
        const float* position = reinterpret_cast<float*>(object + 16);
        float px = position[0] + ax;
        float py = position[1] + ay;
        float pz = position[2] + az;
        hpos[i].x = px;
        hpos[i].y = py;
        hpos[i].z = pz;
        Point3 b = normalized(one, position[0] - px, position[1] - py, position[2] - pz);
        Point3 cross = {b.y * up.z - b.z * up.y, b.z * up.x - b.x * up.z, b.x * up.y - b.y * up.x};
        Point3 c = normalized(one, cross.x, cross.y, cross.z);
        lightMats[i][0] = c.x;
        lightMats[i][1] = c.y;
        lightMats[i][2] = c.z;
        lightMats[i][3] = b.x;
        lightMats[i][4] = b.y;
        lightMats[i][5] = b.z;
        lightMats[i][6] = up.x;
        lightMats[i][7] = up.y;
        lightMats[i][8] = up.z;
        if (i != 0) {
            Point3 d = normalized(one, px - hpos[i - 1].x, py - hpos[i - 1].y, pz - hpos[i - 1].z);
            hvecs[i - 1].z = d.z;
            hvecs[i - 1].x = d.x;
            hvecs[i - 1].y = d.y;
        }
        getHairAndSkin__Fi(i);
    }
    Point3 wrap = normalized(one, hpos[0].x - hpos[9].x, hpos[0].y - hpos[9].y, hpos[0].z - hpos[9].z);
    hvecs[9].z = wrap.z;
    hvecs[9].x = wrap.x;
    hvecs[9].y = wrap.y;
    if (scriptGetGameState__FPc("needPlayers") != 0) {
        for (int j = 0; j < 10; ++j) {
            changeHairStyle__Fii(j, 0);
            changeHairColor__Fii(j, 0);
            changeBeard__Fii(j, 0);
            changeSkin__Fii(j, 0);
        }
    } else {
        for (int j = 0; j < 10; ++j) objectRemoveFromGrid__FP10GameObject(targetObjs[j]);
        for (int k = 0; k < nmPlayers; ) {
            char finalName[256], vif[256], tex[256], anm[256];
            int next = k + 1;
            sprintf(finalName, "finalpos%d", next);
            unsigned char* finalObject = objectFindObject__FPc(finalName);
            const SelectInfo& info = playerSelectInfo[k];
            sprintf(vif, "%s_bodyv1.vif", targets[info.byte32].name);
            sprintf(tex, "%s_bodyv%d.tex", targets[info.byte32].name, info.byte35 + 1);
            sprintf(anm, "%s_intro1.anm", targets[info.byte32].name);
            sprintf(nameTags[k], "name=%s", targets[info.byte32].name);
            sprintf(pnums[k], "pnum=%d", k);
            SelectDummy* dummy = &dummyList[k];
            dummy->tag = nameTags[k];
            dummy->number = pnums[k];
            dummy->zero = 0;
            unsigned char* block = static_cast<unsigned char*>(blockAlloc__Fi(1144));
            if (block != 0) {
                Point3 where = *reinterpret_cast<Point3*>(finalObject + 16);
                __3NPCG6Point3sPcN33ifPPc(block, where, *reinterpret_cast<short*>(finalObject + 56), npcLumpName, vif,
                                          tex, anm, 0, 60.0f, dummy);
            }
            unsigned long long flags = *reinterpret_cast<unsigned long long*>(block + 160);
            *reinterpret_cast<unsigned long long*>(block + 160) = flags | 0x10000000ull;
            objectAddToRunList__FP10GameObject(block);
            k = next;
        }
    }
    if (strcmp(levelName + 0x51, "kelsel2") == 0) {
        hermiteTime = 0.0f;
        signed char start = playerSelectStartPlayer;
        signed char enforce = playerSelectEnforce;
        playerSelectInfo[start].byte32 = 0;
        headerAlpha = hermiteTime;
        hermDeltaScale = 1.0f;
        needEnforce = enforce;
        curPlayer = start;
        flareDelta = hermiteTime;
        bodyAlpha = hermiteTime;
        requestToExit = 0;
        startExitFade = 0;
        playerSelectSuccessful = 0;
        initPreSelect__Fv();
        initNamePad__Fv();
        initLightInfo__Fv();
        engineDisable30fps = 1;
    }
}
