// Observed: ParsePrimBuffer__10VIESFParse at 0x0116e5a8 is 3,372 bytes. Argument: parse (a0; creation
// argument word at +0, raster word at +12, current-object word at +28 whose +84 word is a count
// and +88 word a table, dictionary word at +32, object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &version): negative, or a tag other than 0x1200,
//      returns -1;
//   2. a version of 0 returns the result of ParsePrimBufferObjV0__10VIESFParse(parse) when it is not
//      negative, else -1 (nothing else is read here);
//   3. otherwise, when the version is 2 or more, an unsigned id (Read__9VIObjFileRUi; 0 when the
//      version is 1), then seven signed words (Read__9VIObjFileRi): kind, a, group count, c and
//      three packing words p1, p2, p3;
//   4. a non-zero id looked up with Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type,
//      &handle): when found, a type other than 11 returns -1, else ReadEnd__9VIObjFile (result
//      ignored) and the found handle is returned;
//   5. handle = CreatePrimBuffer__8VIRasteri(raster, parse[0]) (negative returns -1); buffer =
//      PrimBuffer__8VIRasteri(raster, handle); a non-zero id is registered with
//      Add__12VIDictionaryR10VIResourceUi14VIResourceTypei(dict, buffer, id, 11, handle) (negative
//      returns -1); Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(buffer, raster, kind, a,
//      group count, c) (negative returns -1); Lock__12VIPrimBuffer (negative returns -1);
//      SetPacking__12VIPrimBufferiii(buffer, p1, p2, p3) (result ignored);
//   6. scale1 = 1.0f / powf(2.0f, (float)p1), scale2 = 1.0f / powf(2.0f, (float)p2) and a constant
//      0x3c010204 (about 1/127.5) used for normals;
//   7. for each group (the count is read again after every group): a vertex count and a material
//      number (two Read__9VIObjFileRi); material changes call EndMaterial__12VIPrimBuffer (unless
//      the previous number was -1, the initial value) and BeginMaterial__12VIPrimBufferi; then
//      Begin__12VIPrimBufferi(buffer, count), the vertices and End__12VIPrimBuffer. By kind:
//        0: eight floats and four color bytes per vertex (position x y z, uv u v, normal x y z, then
//           Read__9VIObjFileRUc x4) given to UV0__12VIPrimBufferR7VIVect2,
//           Normal__12VIPrimBufferR7VIVect3, Color__12VIPrimBufferR9VIColor32 and
//           Vertex__12VIPrimBufferR7VIVect3;
//        2: the word at buffer+8 selects. 2: five 16-bit values (position, uv), three signed bytes
//           (normal, Read__9VIObjFileRSc) and four color bytes, given to the S16/S8 variants
//           (UV0 ...R10VIVect2S16, Normal ...R9VIVect3S8, Color, Vertex ...R10VIVect3S16); 0: the
//           same values converted to floats (position times scale1, uv times scale2, normal bytes
//           times the 1/127.5 constant, color bytes copied) and given to the float variants;
//           any other value returns -1;
//        4: as kind 2 but with one more 16-bit value per vertex given to
//           VertexGroup__12VIPrimBuffers before Vertex; buffer+8 equal to 4 selects the S16
//           variants, 6 the converted float variants, any other value returns -1;
//        any other kind returns -1;
//   8. after the groups EndMaterial when a is non-zero, then CalcBVolumes__12VIPrimBufferiP7VIVect3
//      (buffer, count, table) with the +84/+88 words of the current object (0 and 0 when the +28
//      word or the count is zero), Unlock__12VIPrimBuffer (each negative returns -1) and ReadEnd
//      (negative returns -1); the result is the primitive buffer handle.
// Inferred: this builds a vertex buffer (resource type 11) with per-vertex UV, normal and color for
// ordinary geometry; the 16-bit variants are fixed-point with two exponents. Unknown: the names of the
// kinds 0, 2, 4 and the buffer types at +8 (0, 2, 4, 6), of a and c, and the second ReadBegin
// output name (version is a guess from its use).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRs(void* file, short* value);
int Read__9VIObjFileRSc(void* file, signed char* value);
int Read__9VIObjFileRUc(void* file, unsigned char* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
float powf(float base, float exponent);
int ParsePrimBufferObjV0__10VIESFParse(void* parse);
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
int Normal__12VIPrimBufferR7VIVect3(void* buffer, const float* normal);
int Normal__12VIPrimBufferR9VIVect3S8(void* buffer, const signed char* normal);
int Color__12VIPrimBufferR9VIColor32(void* buffer, const unsigned char* color);
int VertexGroup__12VIPrimBuffers(void* buffer, short group);
int Vertex__12VIPrimBufferR7VIVect3(void* buffer, const float* vertex);
int Vertex__12VIPrimBufferR10VIVect3S16(void* buffer, const short* vertex);
int CalcBVolumes__12VIPrimBufferiP7VIVect3(void* buffer, int count, void* table);

int ParsePrimBuffer__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* dictionary = NORRATH_WORD_ADDRESS(words[8]);
    void* raster = NORRATH_WORD_ADDRESS(words[3]);
    unsigned short tag;
    unsigned short version;
    unsigned id = 0;
    int kind;
    int a;
    int group_count;
    int c;
    int packing_first;
    int packing_second;
    int packing_third;
    int handle;
    int type;
    int previous_material = -1;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &version) < 0) return -1;
    if (tag != 0x1200) return -1;
    if (version == 0) {
        int result = ParsePrimBufferObjV0__10VIESFParse(parse);
        return result >= 0 ? result : -1;
    }
    if (version >= 2) Read__9VIObjFileRUi(file, &id);
    Read__9VIObjFileRi(file, &kind);
    Read__9VIObjFileRi(file, &a);
    Read__9VIObjFileRi(file, &group_count);
    Read__9VIObjFileRi(file, &c);
    Read__9VIObjFileRi(file, &packing_first);
    Read__9VIObjFileRi(file, &packing_second);
    Read__9VIObjFileRi(file, &packing_third);
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
    SetPacking__12VIPrimBufferiii(buffer, packing_first, packing_second, packing_third);
    float scale_first = 1.0f / powf(2.0f, static_cast<float>(packing_first));
    float scale_second = 1.0f / powf(2.0f, static_cast<float>(packing_second));
    const float normal_scale = 0x1.020408p-7f;
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
        if (kind == 0) {
            float position[3] = {0.0f, 0.0f, 0.0f};
            float uv[2] = {0.0f, 0.0f};
            float normal[3] = {0.0f, 0.0f, 0.0f};
            unsigned char color[4] = {0, 0, 0, 255};
            Begin__12VIPrimBufferi(buffer, vertex_count);
            for (int i = 0; i < vertex_count; ++i) {
                Read__9VIObjFileRf(file, &position[0]);
                Read__9VIObjFileRf(file, &position[1]);
                Read__9VIObjFileRf(file, &position[2]);
                Read__9VIObjFileRf(file, &uv[0]);
                Read__9VIObjFileRf(file, &uv[1]);
                Read__9VIObjFileRf(file, &normal[0]);
                Read__9VIObjFileRf(file, &normal[1]);
                Read__9VIObjFileRf(file, &normal[2]);
                Read__9VIObjFileRUc(file, &color[0]);
                Read__9VIObjFileRUc(file, &color[1]);
                Read__9VIObjFileRUc(file, &color[2]);
                Read__9VIObjFileRUc(file, &color[3]);
                UV0__12VIPrimBufferR7VIVect2(buffer, uv);
                Normal__12VIPrimBufferR7VIVect3(buffer, normal);
                Color__12VIPrimBufferR9VIColor32(buffer, color);
                Vertex__12VIPrimBufferR7VIVect3(buffer, position);
            }
        } else if (kind == 2) {
            int buffer_kind = *reinterpret_cast<int*>(static_cast<unsigned char*>(buffer) + 8);
            short position[3] = {0, 0, 0};
            short uv[2] = {0, 0};
            signed char normal[3] = {0, 0, 0};
            unsigned char color[4] = {0, 0, 0, 255};
            if (buffer_kind == 2) {
                Begin__12VIPrimBufferi(buffer, vertex_count);
                for (int i = 0; i < vertex_count; ++i) {
                    Read__9VIObjFileRs(file, &position[0]);
                    Read__9VIObjFileRs(file, &position[1]);
                    Read__9VIObjFileRs(file, &position[2]);
                    Read__9VIObjFileRs(file, &uv[0]);
                    Read__9VIObjFileRs(file, &uv[1]);
                    Read__9VIObjFileRSc(file, &normal[0]);
                    Read__9VIObjFileRSc(file, &normal[1]);
                    Read__9VIObjFileRSc(file, &normal[2]);
                    Read__9VIObjFileRUc(file, &color[0]);
                    Read__9VIObjFileRUc(file, &color[1]);
                    Read__9VIObjFileRUc(file, &color[2]);
                    Read__9VIObjFileRUc(file, &color[3]);
                    UV0__12VIPrimBufferR10VIVect2S16(buffer, uv);
                    Normal__12VIPrimBufferR9VIVect3S8(buffer, normal);
                    Color__12VIPrimBufferR9VIColor32(buffer, color);
                    Vertex__12VIPrimBufferR10VIVect3S16(buffer, position);
                }
            } else if (buffer_kind == 0) {
                float position_f[3] = {0.0f, 0.0f, 0.0f};
                float uv_f[2] = {0.0f, 0.0f};
                float normal_f[3] = {0.0f, 0.0f, 0.0f};
                unsigned char color_out[4] = {0, 0, 0, 255};
                Begin__12VIPrimBufferi(buffer, vertex_count);
                for (int i = 0; i < vertex_count; ++i) {
                    Read__9VIObjFileRs(file, &position[0]);
                    Read__9VIObjFileRs(file, &position[1]);
                    Read__9VIObjFileRs(file, &position[2]);
                    Read__9VIObjFileRs(file, &uv[0]);
                    Read__9VIObjFileRs(file, &uv[1]);
                    Read__9VIObjFileRSc(file, &normal[0]);
                    Read__9VIObjFileRSc(file, &normal[1]);
                    Read__9VIObjFileRSc(file, &normal[2]);
                    Read__9VIObjFileRUc(file, &color[0]);
                    Read__9VIObjFileRUc(file, &color[1]);
                    Read__9VIObjFileRUc(file, &color[2]);
                    Read__9VIObjFileRUc(file, &color[3]);
                    position_f[0] = static_cast<float>(position[0]) * scale_first;
                    position_f[1] = static_cast<float>(position[1]) * scale_first;
                    position_f[2] = static_cast<float>(position[2]) * scale_first;
                    uv_f[0] = static_cast<float>(uv[0]) * scale_second;
                    uv_f[1] = static_cast<float>(uv[1]) * scale_second;
                    normal_f[0] = static_cast<float>(normal[0]) * normal_scale;
                    normal_f[1] = static_cast<float>(normal[1]) * normal_scale;
                    normal_f[2] = static_cast<float>(normal[2]) * normal_scale;
                    color_out[0] = color[0];
                    color_out[1] = color[1];
                    color_out[2] = color[2];
                    color_out[3] = color[3];
                    UV0__12VIPrimBufferR7VIVect2(buffer, uv_f);
                    Normal__12VIPrimBufferR7VIVect3(buffer, normal_f);
                    Color__12VIPrimBufferR9VIColor32(buffer, color_out);
                    Vertex__12VIPrimBufferR7VIVect3(buffer, position_f);
                }
            } else {
                return -1;
            }
        } else if (kind == 4) {
            int buffer_kind = *reinterpret_cast<int*>(static_cast<unsigned char*>(buffer) + 8);
            short position[3] = {0, 0, 0};
            short uv[2] = {0, 0};
            signed char normal[3] = {0, 0, 0};
            unsigned char color[4] = {0, 0, 0, 255};
            short vertex_group;
            if (buffer_kind == 4) {
                Begin__12VIPrimBufferi(buffer, vertex_count);
                for (int i = 0; i < vertex_count; ++i) {
                    Read__9VIObjFileRs(file, &position[0]);
                    Read__9VIObjFileRs(file, &position[1]);
                    Read__9VIObjFileRs(file, &position[2]);
                    Read__9VIObjFileRs(file, &uv[0]);
                    Read__9VIObjFileRs(file, &uv[1]);
                    Read__9VIObjFileRSc(file, &normal[0]);
                    Read__9VIObjFileRSc(file, &normal[1]);
                    Read__9VIObjFileRSc(file, &normal[2]);
                    Read__9VIObjFileRUc(file, &color[0]);
                    Read__9VIObjFileRUc(file, &color[1]);
                    Read__9VIObjFileRUc(file, &color[2]);
                    Read__9VIObjFileRUc(file, &color[3]);
                    Read__9VIObjFileRs(file, &vertex_group);
                    UV0__12VIPrimBufferR10VIVect2S16(buffer, uv);
                    Normal__12VIPrimBufferR9VIVect3S8(buffer, normal);
                    Color__12VIPrimBufferR9VIColor32(buffer, color);
                    VertexGroup__12VIPrimBuffers(buffer, vertex_group);
                    Vertex__12VIPrimBufferR10VIVect3S16(buffer, position);
                }
            } else if (buffer_kind == 6) {
                float position_f[3] = {0.0f, 0.0f, 0.0f};
                float uv_f[2] = {0.0f, 0.0f};
                float normal_f[3] = {0.0f, 0.0f, 0.0f};
                unsigned char color_out[4] = {0, 0, 0, 255};
                Begin__12VIPrimBufferi(buffer, vertex_count);
                for (int i = 0; i < vertex_count; ++i) {
                    Read__9VIObjFileRs(file, &position[0]);
                    Read__9VIObjFileRs(file, &position[1]);
                    Read__9VIObjFileRs(file, &position[2]);
                    Read__9VIObjFileRs(file, &uv[0]);
                    Read__9VIObjFileRs(file, &uv[1]);
                    Read__9VIObjFileRSc(file, &normal[0]);
                    Read__9VIObjFileRSc(file, &normal[1]);
                    Read__9VIObjFileRSc(file, &normal[2]);
                    Read__9VIObjFileRUc(file, &color[0]);
                    Read__9VIObjFileRUc(file, &color[1]);
                    Read__9VIObjFileRUc(file, &color[2]);
                    Read__9VIObjFileRUc(file, &color[3]);
                    Read__9VIObjFileRs(file, &vertex_group);
                    position_f[0] = static_cast<float>(position[0]) * scale_first;
                    position_f[1] = static_cast<float>(position[1]) * scale_first;
                    position_f[2] = static_cast<float>(position[2]) * scale_first;
                    uv_f[0] = static_cast<float>(uv[0]) * scale_second;
                    uv_f[1] = static_cast<float>(uv[1]) * scale_second;
                    normal_f[0] = static_cast<float>(normal[0]) * normal_scale;
                    normal_f[1] = static_cast<float>(normal[1]) * normal_scale;
                    normal_f[2] = static_cast<float>(normal[2]) * normal_scale;
                    color_out[0] = color[0];
                    color_out[1] = color[1];
                    color_out[2] = color[2];
                    color_out[3] = color[3];
                    UV0__12VIPrimBufferR7VIVect2(buffer, uv_f);
                    Normal__12VIPrimBufferR7VIVect3(buffer, normal_f);
                    Color__12VIPrimBufferR9VIColor32(buffer, color_out);
                    VertexGroup__12VIPrimBuffers(buffer, vertex_group);
                    Vertex__12VIPrimBufferR7VIVect3(buffer, position_f);
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
    unsigned char* object = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[7]));
    int result;
    if (object != 0 && *reinterpret_cast<int*>(object + 84) != 0)
        result = CalcBVolumes__12VIPrimBufferiP7VIVect3(buffer, *reinterpret_cast<int*>(object + 84), NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(object + 88)));
    else
        result = CalcBVolumes__12VIPrimBufferiP7VIVect3(buffer, 0, 0);
    if (result < 0) return -1;
    if (Unlock__12VIPrimBuffer(buffer) < 0) return -1;
    if (ReadEnd__9VIObjFile(file) < 0) return -1;
    return handle;
}
}
