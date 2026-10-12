// Observed msg_draw__10AutoObject at 0x0021ec10, 728 bytes.
// All offsets are into the object. Steps in order:
//  1. matMakeTransYaw builds the matrix from a copy of the 12 bytes at +16 and the
//     signed halfword at +56 (no offset added).
//  2. The animation pointer A is object+152 when the signed byte at +156 is nonzero,
//     else null.
//  3. When bit 8 (0x100) of the word at +12 is set, lightSetAmbientColor gets a fixed
//     four-halfword colour {255, 255, 255, 0} (the image's flashColor object, 8 bytes
//     at 0x004854a8).
//  4. modelSetShadowScale(2.0).
//  5. When bit 0 of the word at +208 is clear, modelDrawShadow(model +144, 1, matrix, A, 0)
//     supplies the draw record R (otherwise R is null).
//  6. When the byte at +41 or the byte at +40 equals 255, the byte modelClippingOn is
//     set to 1.
//  7. When bit 1 (value 2) of the word at +208 is set, lightSetAmbientColor gets
//     {128, 128, 128, 0} (8 bytes at 0x004854b0, copied to a local first), then
//     lightSetAmbientOnly.
//  8. modelDraw(model +144, texture +148, 1, copy of the +16 point, matrix, A, the 64-bit
//     value at +200, R, 0).
//  9. When the signed byte at +108 is nonzero: for tag index 0, 1, ... while
//     findTagString(object, "multiwak", index) returns a tag T, the names T + ".vif" and
//     T + ".tex" are formatted (sprintf "%s.vif" and "%s.tex"), looked up with
//     lumpFindResource(object+109, name) as model and texture, a shadow record is made
//     with modelDrawShadow only if bit 0 of +208 is clear (otherwise the previous R
//     value is reused), and modelDraw draws them with the same matrix and animation,
//     the 64-bit flags value -1, and that record.
// 10. modelResetOptionalParams. Then, when bit 8 of +12 is set or bit 1 of +208 is
//     set, lightResetOptionalParams, and bit 8 of +12 is cleared.
// The image keeps the colours and format texts in its shared data area; this source holds
// its own copies of them. Unknown: what the fields at +40/+41, +108, +200 and +208 mean.

extern "C" {
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
void lightSetAmbientColor__FPs(short* color);
void lightSetAmbientOnly__Fv();
void lightResetOptionalParams__Fv();
void modelSetShadowScale__Ff(float scale);
void modelResetOptionalParams__Fv();
void* modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
    void* model, int mode, void* matrix, void* animation, char flag);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last);
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
int format_text(char* buffer, const char* format, ...) asm("sprintf");
extern unsigned char modelClippingOn;
}

namespace {
typedef unsigned UWord __attribute__((may_alias));
typedef unsigned long long DWord __attribute__((may_alias));
typedef void* PointerWord __attribute__((may_alias));
unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
void* pointer_from_word(unsigned value) {
    return reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(value));
}
void copy_bytes(unsigned char* dest, const unsigned char* src, unsigned count) {
    for (unsigned i = 0; i < count; ++i) dest[i] = src[i];
}
short flashColor[4] = {255, 255, 255, 0};
}

extern "C" void msg_draw__10AutoObject(void* object_storage) {
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    float matrix[12];
    unsigned char first_point[12];
    copy_bytes(first_point, object + 16u, 12u);
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, first_point, *reinterpret_cast<short*>(object + 56));
    void* animation = nullptr;
    if (static_cast<signed char>(object[156]) != 0) animation = object + 152;
    if ((*reinterpret_cast<UWord*>(object + 12) & 0x100u) != 0) lightSetAmbientColor__FPs(flashColor);
    modelSetShadowScale__Ff(2.0f);
    void* record = nullptr;
    if ((*reinterpret_cast<UWord*>(object + 208) & 1u) == 0)
        record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(
            pointer_from_word(read_word(object + 144u)), 1, matrix, animation, 0);
    if (object[41] == 255 || object[40] == 255) modelClippingOn = 1;
    if ((*reinterpret_cast<UWord*>(object + 208) & 2u) != 0) {
        short ambient[4] = {128, 128, 128, 0};
        lightSetAmbientColor__FPs(ambient);
        lightSetAmbientOnly__Fv();
    }
    unsigned char second_point[12];
    copy_bytes(second_point, object + 16u, 12u);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        pointer_from_word(read_word(object + 144u)), pointer_from_word(read_word(object + 148u)), 1, second_point,
        matrix, animation, *reinterpret_cast<DWord*>(object + 200), record, 0);
    if (static_cast<signed char>(object[108]) != 0) {
        for (int index = 0;; ++index) {
            char* tag = findTagString__10GameObjectPci(object, "multiwak", index);
            if (tag == nullptr) break;
            char model_name[128];
            char texture_name[128];
            format_text(model_name, "%s.vif", tag);
            format_text(texture_name, "%s.tex", tag);
            void* model = lumpFindResource__FPcT0(reinterpret_cast<const char*>(object + 109), model_name);
            void* texture = lumpFindResource__FPcT0(reinterpret_cast<const char*>(object + 109), texture_name);
            if ((*reinterpret_cast<UWord*>(object + 208) & 1u) == 0)
                record = modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec(model, 1, matrix, animation, 0);
            unsigned char part_point[12];
            copy_bytes(part_point, object + 16u, 12u);
            modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
                model, texture, 1, part_point, matrix, animation, 0xffffffffffffffffull, record, 0);
        }
    }
    modelResetOptionalParams__Fv();
    if ((*reinterpret_cast<UWord*>(object + 12) & 0x100u) != 0 || (*reinterpret_cast<UWord*>(object + 208) & 2u) != 0) {
        lightResetOptionalParams__Fv();
        *reinterpret_cast<UWord*>(object + 12) &= 0xfffffeffu;
    }
}
