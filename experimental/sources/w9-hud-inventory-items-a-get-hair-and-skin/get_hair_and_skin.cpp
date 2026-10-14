// Observed getHairAndSkin__Fi at 0x0027ed50, 1120 bytes, in the pinned SLUS_205.65.
// (index). Fills one 636-byte record of the targets table at 0x00437038 with resources
// looked up in the lump named by the word npcLumpName; no value is returned. The record
// layout used here (offsets in bytes): +0 name text pointer, +17 signed byte compared with 4,
// +18 signed byte flag, +20 pointer to a second name text, +24 count of body textures,
// +25 count of hair styles, +26 variants of the last hair style, +27 beard count plus one,
// +28 hair model words [8], +60 beard model words [8], +92/+96/+100 three armor model words,
// +104 model for the second name, +108 hair textures [8][8], +364 beard textures [8][8],
// +620/+624/+628 three armor texture words, +632 texture for the second name.
// Steps, each looking a text up as lumpFindResource(npcLumpName, text) and stopping at the
// first miss:
//  1. +24 = 0, then while "<name>_bodyv<+24 + 1>.tex" exists, +24 is incremented.
//  2. For k = 0.. : "<name>_hair<k+1>a.vif" is stored at hair model [k]; on a miss stop.
//     Otherwise "<name>_hair<k+1><letter>.tex" for letter a, b, ... is stored at hair
//     textures [k][j] until a miss (the miss is stored too). If j is zero stop, else +26 = j
//     and the next k. Afterwards +25 = k, plus one when the +18 flag is set.
//  3. The same for beards ("_beard<k+1>a.vif", "_beard<k+1><letter>.tex") into the beard
//     words, with no counter written while looping.
//  4. +27 = k + 1 of the last beard attempt. getVifAndTex(name, ...) then fills the three
//     armor model and texture pairs, from "cloth1body11", "cloth1leggings11",
//     "cloth1boots11" when the +17 byte is 4, otherwise "lethbody11", "lethleggings11",
//     "lethboots11".
//  5. "<+20 text>.vif" is stored at +104 and "<+20 text>.tex" at +632.
// The stack text buffer's size is not named by the image; 128 bytes here.
// Unknown: what the +17 value 4 stands for, and the bound of the loops (the image checks none).
// Not executed on hardware; callees and objects are defined elsewhere.
extern "C" {
struct Target {
    const char* name;
    unsigned char gap4[13];
    signed char kind;
    signed char flag;
    unsigned char gap19;
    const char* modelName;
    signed char bodyTextureCount;
    signed char hairCount;
    signed char hairVariants;
    signed char beardCountPlusOne;
    void* hairModel[8];
    void* beardModel[8];
    void* armorModel[3];
    void* modelVif;
    void* hairTexture[8][8];
    void* beardTexture[8][8];
    void* armorTexture[3];
    void* modelTex;
};
extern Target targets[];
extern const char* npcLumpName;
int sprintf(char*, const char*, ...);
void* lumpFindResource__FPcT0(const char*, char*);
int getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(const char*, const char*, void**, void**);

void getHairAndSkin__Fi(int index) {
    char text[128];
    Target& t = targets[index];
    t.bodyTextureCount = 0;
    for (;;) {
        sprintf(text, "%s_bodyv%d.tex", t.name, t.bodyTextureCount + 1);
        if (!lumpFindResource__FPcT0(npcLumpName, text))
            break;
        t.bodyTextureCount = t.bodyTextureCount + 1;
    }
    int k = 0;
    for (;;) {
        sprintf(text, "%s_hair%da.vif", t.name, k + 1);
        t.hairModel[k] = lumpFindResource__FPcT0(npcLumpName, text);
        if (!t.hairModel[k])
            break;
        int j = 0;
        for (;;) {
            sprintf(text, "%s_hair%d%c.tex", t.name, k + 1, 97 + j);
            t.hairTexture[k][j] = lumpFindResource__FPcT0(npcLumpName, text);
            if (!t.hairTexture[k][j])
                break;
            ++j;
        }
        if (j == 0)
            break;
        t.hairVariants = j;
        ++k;
    }
    t.hairCount = k;
    if (t.flag != 0)
        t.hairCount = k + 1;
    int b = 0;
    int next;
    for (;;) {
        next = b + 1;
        sprintf(text, "%s_beard%da.vif", t.name, next);
        t.beardModel[b] = lumpFindResource__FPcT0(npcLumpName, text);
        if (!t.beardModel[b])
            break;
        int j = 0;
        for (;;) {
            sprintf(text, "%s_beard%d%c.tex", t.name, next, 97 + j);
            t.beardTexture[b][j] = lumpFindResource__FPcT0(npcLumpName, text);
            if (!t.beardTexture[b][j])
                break;
            ++j;
        }
        if (j == 0)
            break;
        b = next;
    }
    t.beardCountPlusOne = next;
    if (t.kind == 4) {
        getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(t.name, "cloth1body11", &t.armorModel[0], &t.armorTexture[0]);
        getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(t.name, "cloth1leggings11", &t.armorModel[1], &t.armorTexture[1]);
        getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(t.name, "cloth1boots11", &t.armorModel[2], &t.armorTexture[2]);
    } else {
        getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(t.name, "lethbody11", &t.armorModel[0], &t.armorTexture[0]);
        getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(t.name, "lethleggings11", &t.armorModel[1], &t.armorTexture[1]);
        getVifAndTex__FPcT0PP12_modelHeaderPP8_texture(t.name, "lethboots11", &t.armorModel[2], &t.armorTexture[2]);
    }
    sprintf(text, "%s.vif", t.modelName);
    t.modelVif = lumpFindResource__FPcT0(npcLumpName, text);
    sprintf(text, "%s.tex", t.modelName);
    t.modelTex = lumpFindResource__FPcT0(npcLumpName, text);
}
}
