// Observed draw__8CreatureP12_modelHeaderP8_textureT1 at 0x00109808, 2716 bytes.
// Arguments: the object, a model, a texture and a second model (the shadow model,
// null when absent; the mangled name's T1 repeats the first parameter's type
// counting the object as parameter 0). Offsets are from the object address.
// Struct-by-value Point3 parameters are passed by address to a caller-made copy
// in this image; the declarations below take pointers for that reason.
//  1. pos = (x, y, z) of +16 with z replaced by the float at +60. When the float
//     at +228 is nonzero, pos.x += icos(h) * f228 and pos.y += isin(h) * f228 with
//     h the halfword at +56.
//     matMakeTransYawScale(matrix, &pos, (short)(h + 16384), float at +224). The draw
//     point is center = (pos.x, pos.y, pos.z + float(+32) * 0.5).
//  2. Bit 0x100 of the word at +12: lightSetAmbientColor to {255, 255, 255, 0}.
//     Bit 0x02000000: lightSetColorScale(0.6f bits 0x3f199999, same, same).
//     Then runStandardGlow__8Creature(object).
//  3. When the word at +140 or the word at +144 is nonzero: for each of
//     modelGetNmShadowPoints(model) points, modelGetShadowPos fills a point and a
//     float and P_FaceSprite draws a sprite (colour halfwords +140..+146, texture
//     haloTexture2, the low 16 bits of frameNm shifted left 6, 3) at that point.
//  4. When the word at +148 or the word at +152 is nonzero, lightSetAmbientOffset is
//     called with the four halfwords at +148.
//  5. Floats at +860, +868 and +872 (clamped to 1.0) drive two ambient offset calls
//     and a number of floating haloTexture1 sprites at the object position.
//  6. One of the 14 words at +896 + 4 * k (k = 1, 2, 5, 6, 8..11, 13, and 7 taking
//     precedence) that is nonzero selects colours, a scale and a mode for
//     fxRunCreatureGlow; the choice below follows the image's table.
//  7. The first nonzero of the 16 words at +896 counts down once per call (not while
//     paused, and never entry 12) and its four floats at +108 become the ambient
//     offset.
//  8. Each object on the list at +888 gets its virtual function in vtable entry
//     3 (+24) called.
//  9. Without bit 0x200000 of the doubleword at +160, modelDrawShadow draws the
//     shadow. With bit 0x40000000, the tag strings "light1" and "light2" are looked
//     up and parsed with sscanf into setDirLight.
// 10. modelDraw draws the model; lightResetOptionalParams runs, bit 0x100 of the
//     word at +12 is cleared, and the result of modelDraw is returned.
// Inferred: the 14-entry table is a set of status effects with timers at +896 and
// the glow colours chosen per effect; the image repeats the whole selection 14
// times with no effect on the result, which is done once here. Field and
// function names come from the image symbols only.
// Unknown: field meanings, the units of the float inputs, and the callees' effects.
#ifdef NORRATH_W8_HOST
extern void* norrath_w8_word_to_pointer(unsigned word);
extern unsigned norrath_w8_pointer_to_word(const void* pointer);
extern void (*norrath_w8_word_to_function(unsigned word))(void*);
#define NORRATH_W8_POINTER(word) norrath_w8_word_to_pointer(word)
#define NORRATH_W8_WORD(pointer) norrath_w8_pointer_to_word(pointer)
#define NORRATH_W8_FUNCTION(word) norrath_w8_word_to_function(word)
#else
#define NORRATH_W8_POINTER(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_W8_WORD(pointer) static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer))
#define NORRATH_W8_FUNCTION(word) reinterpret_cast<void (*)(void*)>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct Point3 {
    float x;
    float y;
    float z;
};

template <class T>
static inline T& field(void* object, unsigned offset) {
    return *reinterpret_cast<T*>(static_cast<char*>(object) + offset);
}

static float float_bits(unsigned bits) {
    return __builtin_bit_cast(float, bits);
}

extern "C" {
extern int frameNm;
extern void* haloTexture1;
extern void* haloTexture2;

float icos__Fi(int angle);
float isin__Fi(int angle);
void matMakeTransYawScale__FP8Matrix34G6Point3sf(void* matrix, Point3* position, short yaw, float scale);
void lightSetAmbientColor__FPs(short* color);
void lightSetColorScale__Ffff(float a, float b, float c);
void runStandardGlow__8Creature(void* object);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3* position, short yaw, int index, Point3* out_point, float* out_extra,
    void* matrix33, float scale);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3* position, float a, float b, int c, int d, int e, int f,
                                               void* texture, short g, int h);
void lightSetAmbientOffset__FPs(short* offsets);
void fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
    void* object, void* model, void* animation, Point3* position, short yaw, void* fx_state, float a, float b, float c,
    short* colors_a, short* colors_b, int mode, float scale, int value);
int gameIsPaused__Fv();
void __16ListSafeIteratorP8ListHeadi(void* iterator, void* head, int mask);
void next__16ListSafeIterator(void* iterator);
void _$_16ListSafeIterator(void* iterator, int flags);
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(void* model, int a, void* matrix, void* animation,
                                                                       char b);
char* findTagString__10GameObjectPci(void* object, const char* tag, int zero);
int sscanf(const char* text, const char* format, ...);
void setDirLight__Fsssssfb(short a, short b, short c, short d, short e, float f, char g);
void* modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int a, Point3* position, void* matrix, void* animation, unsigned long long flags,
    void* record, int b);
void lightResetOptionalParams__Fv();
}

static short flash_color[4] = {255, 255, 255, 0};
static short default_glow_colors[4] = {0, 0, 128, 64};
static const char* light_names[2] = {"light1", "light2"};

extern "C" void* draw__8CreatureP12_modelHeaderP8_textureT1(void* self, void* model, void* texture, void* shadow_model) {
    Point3 position = field<Point3>(self, 16);
    position.z = field<float>(self, 60);
    if (field<float>(self, 228) != 0.0f) {
        position.x = position.x + icos__Fi(field<short>(self, 56)) * field<float>(self, 228);
        position.y = position.y + isin__Fi(field<short>(self, 56)) * field<float>(self, 228);
    }
    unsigned char matrix[48];
    matMakeTransYawScale__FP8Matrix34G6Point3sf(matrix, &position,
                                                static_cast<short>(field<unsigned short>(self, 56) + 16384),
                                                field<float>(self, 224));
    Point3 center;
    center.x = position.x;
    center.y = position.y;
    center.z = position.z + field<float>(self, 32) * 0.5f;

    if (field<unsigned>(self, 12) & 0x100u) lightSetAmbientColor__FPs(flash_color);
    if (field<unsigned>(self, 12) & 0x02000000u) {
        const float scale = float_bits(0x3f199999u);
        lightSetColorScale__Ffff(scale, scale, scale);
    }
    runStandardGlow__8Creature(self);

    void* const animation = static_cast<char*>(self) + 288;
    if ((field<unsigned long long>(self, 136) >> 32) != 0ull || field<unsigned>(self, 144) != 0u) {
        const int count = modelGetNmShadowPoints__FP12_modelHeader(model);
        for (int index = 0; index < count; ++index) {
            Point3 spot_source = field<Point3>(self, 16);
            Point3 spot;
            float spot_extra;
            modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
                model, animation, &spot_source, field<short>(self, 56), index, &spot, &spot_extra, nullptr,
                field<float>(self, 224));
            const float wobble = isin__Fi(frameNm << 10) * 250.0f;
            P_FaceSprite__FG6Point3ffiiiiP8_texturesi(
                &spot, float_bits(0xc14db22du), spot_extra + wobble, field<short>(self, 140), field<short>(self, 142),
                field<short>(self, 144), field<short>(self, 146), haloTexture2,
                static_cast<short>(static_cast<unsigned short>(frameNm) << 6), 3);
        }
    }

    if ((field<unsigned long long>(self, 144) >> 32) != 0ull || field<unsigned>(self, 152) != 0u) {
        lightSetAmbientOffset__FPs(&field<short>(self, 148));
    }
    float amount = field<float>(self, 860);
    if (0.0f < amount) {
        if (1.0f < amount) amount = 1.0f;
        const int level = static_cast<int>(amount * 100.0f);
        short offsets[4] = {static_cast<short>(-level), static_cast<short>(-level), static_cast<short>(level),
                            static_cast<short>(level >> 1)};
        lightSetAmbientOffset__FPs(offsets);
    }
    amount = field<float>(self, 868);
    if (0.0f < amount) {
        if (1.0f < amount) amount = 1.0f;
        const int level = static_cast<int>(amount * 100.0f);
        short offsets[4] = {static_cast<short>(-level), static_cast<short>(level >> 1), static_cast<short>(-level),
                            static_cast<short>(level >> 1)};
        lightSetAmbientOffset__FPs(offsets);
    }
    amount = field<float>(self, 872);
    if (0.0f < amount) {
        if (1.0f < amount) amount = 1.0f;
        int sprites = static_cast<int>(amount * 3.0f);
        if (sprites > 0) {
            do {
                Point3 sprite_position = field<Point3>(self, 16);
                --sprites;
                sprite_position.z = sprite_position.z + field<float>(self, 32);
                const float wobble = isin__Fi(frameNm << 10) * 250.0f;
                P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&sprite_position, 10.0f, wobble + float_bits(0x463b8000u), 0,
                                                          200, 0, 70, haloTexture1, 0, 0);
            } while (sprites != 0);
        }
    }

    short glow_colors_a[4] = {default_glow_colors[0], default_glow_colors[1], default_glow_colors[2],
                              default_glow_colors[3]};
    short glow_colors_b[4] = {0, 0, 0, 0};
    float glow_scale = 1.0f;
    int effect = 0;
    int glow_value = 0;
    if (field<int>(self, 924) != 0) {
        glow_colors_a[0] = 0; glow_colors_a[1] = 128; glow_colors_a[2] = 0; glow_colors_a[3] = 64;
        glow_colors_b[0] = 53; glow_colors_b[1] = 154; glow_colors_b[2] = 53; glow_colors_b[3] = 22;
        glow_value = -2200;
        effect = 7;
    } else {
        if (field<int>(self, 900) != 0) {
            glow_colors_a[0] = 0; glow_colors_a[1] = 0; glow_colors_a[2] = 128; glow_colors_a[3] = 64;
            glow_colors_b[0] = 86; glow_colors_b[1] = 126; glow_colors_b[2] = 250; glow_colors_b[3] = 35;
            effect = 1;
        }
        if (field<int>(self, 904) != 0) {
            glow_colors_a[0] = 128; glow_colors_a[1] = 0; glow_colors_a[2] = 128; glow_colors_a[3] = 64;
            glow_colors_b[0] = 250; glow_colors_b[1] = 126; glow_colors_b[2] = 86; glow_colors_b[3] = 35;
            effect = 2;
        }
        if (field<int>(self, 916) != 0) {
            glow_colors_a[0] = 0; glow_colors_a[1] = 0; glow_colors_a[2] = 79; glow_colors_a[3] = 40;
            glow_colors_b[0] = 53; glow_colors_b[1] = 78; glow_colors_b[2] = 154; glow_colors_b[3] = 22;
            effect = 5;
        }
        if (field<int>(self, 920) != 0) {
            glow_scale = float_bits(0x3e4ccccc);
            glow_colors_a[0] = 125; glow_colors_a[1] = 100; glow_colors_a[2] = 50; glow_colors_a[3] = 64;
            glow_colors_b[0] = 187; glow_colors_b[1] = 150; glow_colors_b[2] = 75; glow_colors_b[3] = 22;
            effect = 6;
        }
        if (field<int>(self, 928) != 0) {
            glow_scale = float_bits(0x3e4ccccc);
            glow_colors_a[0] = 125; glow_colors_a[1] = 50; glow_colors_a[2] = 50; glow_colors_a[3] = 64;
            glow_colors_b[0] = 250; glow_colors_b[1] = 75; glow_colors_b[2] = 75; glow_colors_b[3] = 22;
            effect = 8;
        }
        if (field<int>(self, 932) != 0) {
            glow_scale = float_bits(0x3e4ccccc);
            glow_colors_a[0] = 50; glow_colors_a[1] = 125; glow_colors_a[2] = 50; glow_colors_a[3] = 64;
            glow_colors_b[0] = 75; glow_colors_b[1] = 250; glow_colors_b[2] = 75; glow_colors_b[3] = 22;
            effect = 9;
        }
        if (field<int>(self, 936) != 0) {
            glow_scale = float_bits(0x3e4ccccc);
            glow_colors_a[0] = 50; glow_colors_a[1] = 50; glow_colors_a[2] = 125; glow_colors_a[3] = 64;
            glow_colors_b[0] = 75; glow_colors_b[1] = 75; glow_colors_b[2] = 250; glow_colors_b[3] = 22;
            effect = 10;
        }
        if (field<int>(self, 940) != 0) {
            glow_scale = float_bits(0x3e4ccccc);
            glow_colors_a[0] = 125; glow_colors_a[1] = 50; glow_colors_a[2] = 125; glow_colors_a[3] = 64;
            glow_colors_b[0] = 250; glow_colors_b[1] = 75; glow_colors_b[2] = 250; glow_colors_b[3] = 22;
            effect = 11;
        }
        if (field<int>(self, 948) != 0) {
            glow_scale = float_bits(0x3e4ccccc);
            glow_colors_a[0] = 83; glow_colors_a[1] = 33; glow_colors_a[2] = 83; glow_colors_a[3] = 48;
            glow_colors_b[0] = 187; glow_colors_b[1] = 56; glow_colors_b[2] = 187; glow_colors_b[3] = 22;
            effect = 13;
        }
    }
    int index = 0;
    if (effect != 0) {
        const int limit = static_cast<int>(glow_scale * 60.0f);
        const int elapsed = frameNm - field<int>(self, 252);
        int mode;
        if (limit < elapsed) {
            mode = limit < field<int>(self, 896 + 4 * effect) ? 1 : 2;
        } else {
            mode = 0;
        }
        Point3 glow_position = field<Point3>(self, 16);
        fxRunCreatureGlow__FP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffPsT9ifi(
            self, model, animation, &glow_position, field<short>(self, 56), static_cast<char*>(self) + 824, 0.0f, 1.0f,
            0.0f, glow_colors_a, glow_colors_b, mode, glow_scale, glow_value);
    }

    for (; index < 16; ++index) {
        if (field<int>(self, 896 + 4 * index) != 0) {
            if (gameIsPaused__Fv() == 0 && index != 12) field<int>(self, 896 + 4 * index) = field<int>(self, 896 + 4 * index) - 1;
            short ambient[4];
            for (int part = 0; part < 4; ++part) ambient[part] = static_cast<short>(static_cast<int>(field<float>(self, 108 + 4 * part)));
            lightSetAmbientOffset__FPs(ambient);
            break;
        }
    }

    unsigned char iterator[32];
    __16ListSafeIteratorP8ListHeadi(iterator, static_cast<char*>(self) + 888, 0);
    for (unsigned node = *reinterpret_cast<unsigned*>(iterator + 4); node != 0u; node = *reinterpret_cast<unsigned*>(iterator + 4)) {
        void* const object = NORRATH_W8_POINTER(*reinterpret_cast<unsigned*>(static_cast<char*>(NORRATH_W8_POINTER(node)) + 12));
        unsigned char* const entry = static_cast<unsigned char*>(NORRATH_W8_POINTER(field<unsigned>(object, 20))) + 24;
        void (*const function)(void*) = NORRATH_W8_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4));
        function(static_cast<char*>(object) + *reinterpret_cast<short*>(entry));
        next__16ListSafeIterator(iterator);
    }
    _$_16ListSafeIterator(iterator, 2);

    void* shadow_record = nullptr;
    if ((field<unsigned long long>(self, 160) & 0x200000ull) == 0ull) {
        shadow_record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
            shadow_model != nullptr ? shadow_model : model, 1, matrix, animation, 0);
    }
    if ((field<unsigned long long>(self, 160) & 0x40000000ull) != 0ull) {
        int slot_flag = 1;
        for (int slot = 0; slot < 2; ++slot) {
            char* const text = findTagString__10GameObjectPci(self, light_names[slot], 0);
            int values[5];
            float strength;
            if (text != nullptr) {
                values[0] = values[1] = values[2] = values[3] = values[4] = 0;
                strength = 0.0f;
                sscanf(text, "%d,%d,%d,%d,%d,%f", &values[0], &values[1], &values[2], &values[3], &values[4], &strength);
                setDirLight__Fsssssfb(static_cast<short>(field<unsigned short>(self, 798) + (static_cast<unsigned short>(values[0]) << 8)),
                                      static_cast<short>(static_cast<int>(static_cast<unsigned>(static_cast<unsigned short>(values[1])) << 24) >> 16),
                                      static_cast<short>(values[2]), static_cast<short>(values[3]),
                                      static_cast<short>(values[4]), strength, static_cast<char>(slot_flag));
            }
            slot_flag = 0;
        }
    }
    void* const result = modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 1, &center, matrix, animation, field<unsigned long long>(self, 552), shadow_record, 0);
    lightResetOptionalParams__Fv();
    field<unsigned>(self, 12) = field<unsigned>(self, 12) & 0xfffffeffu;
    return result;
}
