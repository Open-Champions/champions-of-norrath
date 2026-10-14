// Observed: ParseMaterial__10VIESFParseP10VIMaterial at 0x01171d58 is 968 bytes. Arguments: parse (a0;
// callback pointer word at +4 with its argument word at +8, raster word at +12, dictionary word at
// +32, object file word at +36) and material (a1, the material to fill). In order it:
//   1. builds two locals, a 4-byte color (0, 0, 0, 255) and a VIMatrix33 (constructor
//      __10VIMatrix33), and calls the callback at parse+4 with parse+8 when the callback is non-zero;
//   2. reads a signed layer count (Read__9VIObjFileRi) and calls Init__10VIMateriali(material,
//      count); a negative result returns -1 (the matrix is destroyed with _$_10VIMatrix33(matrix, 2)
//      on every exit);
//   3. when ObjectVersion__9VIObjFile(file) is 2 or more: an unsigned flag word; bit 0 calls
//      SetTessellate__10VIMateriali(material, 1);
//   4. when the object version is 3 or more: four bytes (Read__9VIObjFileRUc) into the color, then
//      SetEmissiveColor__10VIMaterialRC9VIColor32(material, &color);
//   5. for each layer below the count (read again every iteration): an unsigned flag word; bit 0
//      selects SetLayerFillType__10VIMateriali18VIMaterialFillType(material, layer, bit0) and bit 1
//      SetLayerUVSet__10VIMaterialii(material, layer, bit1); an unsigned texture id; when
//      non-zero FindTyped__12VIDictionaryUi14VIResourceTypeRi(dict, id, 1, &handle) (a zero result
//      returns -1) and SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, layer,
//      handle, raster, dict); a signed wrap mode (SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode)
//      and a signed blend mode (SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode); four color
//      bytes (SetLayerColor__10VIMaterialiRC9VIColor32); nine floats read into the matrix in order
//      and given to SetLayerUVTransform__10VIMaterialiRC10VIMatrix33; one float for
//      SetLayerTextureLODBias__10VIMaterialif which, for object version 0, is read and then replaced
//      by the constant 0.8 (the image builds it as 0x3f4ccccc); and two floats for
//      SetLayerUVRate__10VIMaterialiRC7VIVect2. No call result in step 5 other than the lookup is
//      checked;
//   6. returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: this fills a material from the file: per-layer fill type, UV set, texture (resource type 1),
// wrap and blend modes, a color, a UV transform, a texture LOD bias (default 0.8 for the oldest file
// version) and a UV scroll rate; bit 0 of the layer flags selects the second fill type and bit 1 the
// second UV set; the callback is probably a progress hook. Unknown: the enumerators of the fill,
// wrap and blend modes, the meaning of material flag bit 0 (tessellate is the callee name), and
// the callback signature beyond one pointer argument.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRUc(void* file, unsigned char* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadEnd__9VIObjFile(void* file);
int ObjectVersion__9VIObjFile(void* file);
int FindTyped__12VIDictionaryUi14VIResourceTypeRi(void* dictionary, unsigned id, int type, int* handle);
void __10VIMatrix33(void* matrix);
void _$_10VIMatrix33(void* matrix, int flags);
int Init__10VIMateriali(void* material, int count);
void SetTessellate__10VIMateriali(void* material, int enabled);
void SetEmissiveColor__10VIMaterialRC9VIColor32(void* material, const unsigned char* color);
void SetLayerFillType__10VIMateriali18VIMaterialFillType(void* material, int layer, int type);
void SetLayerUVSet__10VIMaterialii(void* material, int layer, int set);
void SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(void* material, int layer, int handle, void* raster, void* dictionary);
void SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode(void* material, int layer, int mode);
void SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(void* material, int layer, int mode);
void SetLayerColor__10VIMaterialiRC9VIColor32(void* material, int layer, const unsigned char* color);
void SetLayerUVTransform__10VIMaterialiRC10VIMatrix33(void* material, int layer, const float* matrix);
void SetLayerTextureLODBias__10VIMaterialif(void* material, int layer, float bias);
void SetLayerUVRate__10VIMaterialiRC7VIVect2(void* material, int layer, const float* rate);

int ParseMaterial__10VIESFParseP10VIMaterial(unsigned char* parse, void* material) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    unsigned char color[4] = {0, 0, 0, 255};
    float matrix[9];
    float rate[2] = {0.0f, 0.0f};
    int count;
    unsigned flags;
    __10VIMatrix33(matrix);
    if (words[1] != 0) reinterpret_cast<void (*)(void*)>(NORRATH_WORD_ADDRESS(words[1]))(NORRATH_WORD_ADDRESS(words[2]));
    Read__9VIObjFileRi(file, &count);
    if (Init__10VIMateriali(material, count) < 0) {
        _$_10VIMatrix33(matrix, 2);
        return -1;
    }
    if (ObjectVersion__9VIObjFile(file) >= 2) {
        Read__9VIObjFileRUi(file, &flags);
        if (flags & 1) SetTessellate__10VIMateriali(material, 1);
    }
    if (ObjectVersion__9VIObjFile(file) >= 3) {
        Read__9VIObjFileRUc(file, &color[0]);
        Read__9VIObjFileRUc(file, &color[1]);
        Read__9VIObjFileRUc(file, &color[2]);
        Read__9VIObjFileRUc(file, &color[3]);
        SetEmissiveColor__10VIMaterialRC9VIColor32(material, color);
    }
    for (int layer = 0; layer < count; ++layer) {
        unsigned texture;
        int handle;
        int wrap;
        int blend;
        float bias;
        Read__9VIObjFileRUi(file, &flags);
        SetLayerFillType__10VIMateriali18VIMaterialFillType(material, layer, (flags & 1) != 0 ? 1 : 0);
        SetLayerUVSet__10VIMaterialii(material, layer, (flags & 2) != 0 ? 1 : 0);
        Read__9VIObjFileRUi(file, &texture);
        if (texture != 0) {
            if (FindTyped__12VIDictionaryUi14VIResourceTypeRi(dictionary, texture, 1, &handle) == 0) {
                _$_10VIMatrix33(matrix, 2);
                return -1;
            }
            SetLayerTexture__10VIMaterialiiR8VIRasterR12VIDictionary(material, layer, handle, NORRATH_WORD_ADDRESS(words[3]), dictionary);
        }
        Read__9VIObjFileRi(file, &wrap);
        SetLayerWrapMode__10VIMateriali18VIMaterialWrapMode(material, layer, wrap);
        Read__9VIObjFileRi(file, &blend);
        SetLayerBlendMode__10VIMateriali16VIAlphaBlendMode(material, layer, blend);
        Read__9VIObjFileRUc(file, &color[0]);
        Read__9VIObjFileRUc(file, &color[1]);
        Read__9VIObjFileRUc(file, &color[2]);
        Read__9VIObjFileRUc(file, &color[3]);
        SetLayerColor__10VIMaterialiRC9VIColor32(material, layer, color);
        for (int k = 0; k < 9; ++k) Read__9VIObjFileRf(file, &matrix[k]);
        SetLayerUVTransform__10VIMaterialiRC10VIMatrix33(material, layer, matrix);
        if (ObjectVersion__9VIObjFile(file) == 0) {
            Read__9VIObjFileRf(file, &bias);
            bias = 0.8f;
        } else {
            Read__9VIObjFileRf(file, &bias);
        }
        SetLayerTextureLODBias__10VIMaterialif(material, layer, bias);
        Read__9VIObjFileRf(file, &rate[0]);
        Read__9VIObjFileRf(file, &rate[1]);
        SetLayerUVRate__10VIMaterialiRC7VIVect2(material, layer, rate);
    }
    if (ReadEnd__9VIObjFile(file) < 0) {
        _$_10VIMatrix33(matrix, 2);
        return -1;
    }
    _$_10VIMatrix33(matrix, 2);
    return 0;
}
}
