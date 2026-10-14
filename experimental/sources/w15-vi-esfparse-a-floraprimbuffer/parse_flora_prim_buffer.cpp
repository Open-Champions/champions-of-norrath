// Observed: ParseFloraPrimBuffer__10VIESFParse at 0x01170648 is 1,540 bytes. Argument: parse (a0;
// creation argument word at +0, raster word at +12, dictionary word at +32, object file word at +36).
// In order it:
//   1. ReadBegin__9VIObjFileRUsT1: negative, or a tag other than 0x1230, returns -1;
//   2. reads an unsigned id (Read__9VIObjFileRUi) and then six signed words: a kind, a, a group count,
//      c, and two packing exponents p1 and p2 (Read__9VIObjFileRi);
//   3. when the id is non-zero and Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type,
//      &handle) succeeds: a type other than 11 returns -1, otherwise ReadEnd__9VIObjFile is called
//      (its result ignored) and the found handle is returned without parsing anything else;
//   4. handle = CreatePrimBuffer__8VIRasteri(raster, parse[0]) (negative returns -1); buffer =
//      PrimBuffer__8VIRasteri(raster, handle); a non-zero id is registered with
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, buffer, id, 11, handle) (negative
//      returns -1);
//   5. Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(buffer, raster, kind, a, group count, c)
//      (negative returns -1), Lock__12VIPrimBuffer(buffer) (negative returns -1) and
//      SetPacking__12VIPrimBufferiii(buffer, p1, p2, 7) (result ignored);
//   6. scale1 = 1.0f / powf(2.0f, (float)p1) and scale2 = 1.0f / powf(2.0f, (float)p2);
//   7. for each group (the count is read again after every group): a vertex count and a material
//      number (two Read__9VIObjFileRi); when the material differs from the previous one (starting at
//      -1) EndMaterial__12VIPrimBuffer is called unless the previous was -1, then
//      BeginMaterial__12VIPrimBufferi(buffer, material); then Begin__12VIPrimBufferi(buffer, count)
//      and, per vertex, by kind: 7: five floats (x, y, z, u, v), UV0__12VIPrimBufferR7VIVect2 then
//      Vertex__12VIPrimBufferR7VIVect3; 8: the word at buffer+8 selects, when it is 8, five 16-bit
//      values (Read__9VIObjFileRs) given as shorts to UV0__12VIPrimBufferR10VIVect2S16 and
//      Vertex__12VIPrimBufferR10VIVect3S16, and when it is 7 the same five values converted to
//      floats, x y z times scale1 and u v times scale2, given to the float UV0 and Vertex; any
//      other value returns -1; any other kind returns -1; End__12VIPrimBuffer ends each group;
//   8. afterwards EndMaterial when a is non-zero, then CalcBVolumes__12VIPrimBufferiP7VIVect3(buffer,
//      0, 0), Unlock__12VIPrimBuffer(buffer) (each negative returns -1), and ReadEnd (negative
//      returns -1); the result is the primitive buffer handle.
// Inferred: this builds a vertex buffer for flora objects (resource type 11 in the dictionary) with
// per-vertex UV, material runs, and optional 16-bit fixed-point vertices scaled by two exponents.
// Unknown: the names of the kind enumerators 7 and 8, of a and c, the meaning of the constant 7 passed
// to SetPacking, and the second ReadBegin output (not used).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRs(void* file, short* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
float powf(float base, float exponent);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(void* dictionary, void* resource, unsigned id, int type, int handle);
int CreatePrimBuffer__8VIRasteri(void* raster, int argument);
void* PrimBuffer__8VIRasteri(void* raster, int handle);
int Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(void* buffer, void* raster, int kind, int a, int group_count, int c);
int Lock__12VIPrimBuffer(void* buffer);
int Unlock__12VIPrimBuffer(void* buffer);
int SetPacking__12VIPrimBufferiii(void* buffer, int first, int second, int third);
int BeginMaterial__12VIPrimBufferi(void* buffer, int material);
int EndMaterial__12VIPrimBuffer(void* buffer);
int Begin__12VIPrimBufferi(void* buffer, int count);
int End__12VIPrimBuffer(void* buffer);
int UV0__12VIPrimBufferR7VIVect2(void* buffer, const float* uv);
int UV0__12VIPrimBufferR10VIVect2S16(void* buffer, const short* uv);
int Vertex__12VIPrimBufferR7VIVect3(void* buffer, const float* vertex);
int Vertex__12VIPrimBufferR10VIVect3S16(void* buffer, const short* vertex);
int CalcBVolumes__12VIPrimBufferiP7VIVect3(void* buffer, int count, void* table);

int ParseFloraPrimBuffer__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* raster = NORRATH_WORD_ADDRESS(words[3]);
    unsigned short tag;
    unsigned short second;
    unsigned id = 0;
    int kind;
    int a;
    int group_count;
    int c;
    int packing_first;
    int packing_second;
    int handle;
    int type;
    int previous_material = -1;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &second) < 0) return -1;
    if (tag != 0x1230) return -1;
    Read__9VIObjFileRUi(file, &id);
    Read__9VIObjFileRi(file, &kind);
    Read__9VIObjFileRi(file, &a);
    Read__9VIObjFileRi(file, &group_count);
    Read__9VIObjFileRi(file, &c);
    Read__9VIObjFileRi(file, &packing_first);
    Read__9VIObjFileRi(file, &packing_second);
    if (id != 0) {
        if (Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, id, &type, &handle) != 0) {
            if (type != 11) return -1;
            ReadEnd__9VIObjFile(file);
            return handle;
        }
    }
    handle = CreatePrimBuffer__8VIRasteri(raster, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    void* buffer = PrimBuffer__8VIRasteri(raster, handle);
    if (id != 0) {
        if (Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dictionary, buffer, id, 11, handle) < 0) return -1;
    }
    if (Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(buffer, raster, kind, a, group_count, c) < 0) return -1;
    if (Lock__12VIPrimBuffer(buffer) < 0) return -1;
    SetPacking__12VIPrimBufferiii(buffer, packing_first, packing_second, 7);
    float scale_first = 1.0f / powf(2.0f, static_cast<float>(packing_first));
    float scale_second = 1.0f / powf(2.0f, static_cast<float>(packing_second));
    for (int index = 0; index < group_count; ++index) {
        int vertex_count;
        int material;
        Read__9VIObjFileRi(file, &vertex_count);
        Read__9VIObjFileRi(file, &material);
        if (previous_material != material) {
            if (previous_material != -1) EndMaterial__12VIPrimBuffer(buffer);
            BeginMaterial__12VIPrimBufferi(buffer, material);
            previous_material = material;
        }
        if (kind == 7) {
            float position[3] = {0.0f, 0.0f, 0.0f};
            float uv[2] = {0.0f, 0.0f};
            Begin__12VIPrimBufferi(buffer, vertex_count);
            for (int i = 0; i < vertex_count; ++i) {
                Read__9VIObjFileRf(file, &position[0]);
                Read__9VIObjFileRf(file, &position[1]);
                Read__9VIObjFileRf(file, &position[2]);
                Read__9VIObjFileRf(file, &uv[0]);
                Read__9VIObjFileRf(file, &uv[1]);
                UV0__12VIPrimBufferR7VIVect2(buffer, uv);
                Vertex__12VIPrimBufferR7VIVect3(buffer, position);
            }
        } else if (kind == 8) {
            int buffer_kind = *reinterpret_cast<int*>(static_cast<unsigned char*>(buffer) + 8);
            short position[3] = {0, 0, 0};
            short uv[2] = {0, 0};
            if (buffer_kind == 7) {
                float position_f[3] = {0.0f, 0.0f, 0.0f};
                float uv_f[2] = {0.0f, 0.0f};
                Begin__12VIPrimBufferi(buffer, vertex_count);
                for (int i = 0; i < vertex_count; ++i) {
                    Read__9VIObjFileRs(file, &position[0]);
                    Read__9VIObjFileRs(file, &position[1]);
                    Read__9VIObjFileRs(file, &position[2]);
                    Read__9VIObjFileRs(file, &uv[0]);
                    Read__9VIObjFileRs(file, &uv[1]);
                    position_f[0] = static_cast<float>(position[0]) * scale_first;
                    position_f[1] = static_cast<float>(position[1]) * scale_first;
                    position_f[2] = static_cast<float>(position[2]) * scale_first;
                    uv_f[0] = static_cast<float>(uv[0]) * scale_second;
                    uv_f[1] = static_cast<float>(uv[1]) * scale_second;
                    UV0__12VIPrimBufferR7VIVect2(buffer, uv_f);
                    Vertex__12VIPrimBufferR7VIVect3(buffer, position_f);
                }
            } else if (buffer_kind == 8) {
                Begin__12VIPrimBufferi(buffer, vertex_count);
                for (int i = 0; i < vertex_count; ++i) {
                    Read__9VIObjFileRs(file, &position[0]);
                    Read__9VIObjFileRs(file, &position[1]);
                    Read__9VIObjFileRs(file, &position[2]);
                    Read__9VIObjFileRs(file, &uv[0]);
                    Read__9VIObjFileRs(file, &uv[1]);
                    UV0__12VIPrimBufferR10VIVect2S16(buffer, uv);
                    Vertex__12VIPrimBufferR10VIVect3S16(buffer, position);
                }
            } else {
                return -1;
            }
        } else {
            return -1;
        }
        End__12VIPrimBuffer(buffer);
    }
    if (a != 0) EndMaterial__12VIPrimBuffer(buffer);
    if (CalcBVolumes__12VIPrimBufferiP7VIVect3(buffer, 0, 0) < 0) return -1;
    if (Unlock__12VIPrimBuffer(buffer) < 0) return -1;
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return handle;
}
}
