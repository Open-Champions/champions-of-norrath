// Observed: ParsePrimBufferObjV0__10VIESFParse at 0x0116f2d8, 800 bytes. Argument: parse (a0: word at +0, raster word at +12, object
// file word at +36). It reads no record header. Locals start as: a zeroed 3-float vertex, a zeroed 2-float texture coordinate, a
// zeroed 3-float normal, a 4-byte colour 0,0,0,255, and a current-material value of -1. In order it:
//   1. Read__9VIObjFileRi reads integers A, B and C;
//   2. id = CreatePrimBuffer__8VIRasteri(raster, parse word +0): a negative id returns -1;
//   3. buffer = PrimBuffer__8VIRasteri(raster, id), then Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(buffer, raster, 0, A, B, C)
//      (negative returns -1) and Lock__12VIPrimBuffer(buffer) (negative returns -1);
//   4. for each group g below B (B read again after every group): Read__9VIObjFileRi reads a vertex count N and a material M; when M
//      differs from the current material, EndMaterial__12VIPrimBuffer(buffer) is called first unless the current material is -1, then
//      BeginMaterial__12VIPrimBufferi(buffer, M) and the current material becomes M; Begin__12VIPrimBufferi(buffer, N); then for each
//      vertex below N (N read again each pass) it reads eight floats (vertex x y z, texture u v, normal x y z, in that order) and four
//      unsigned bytes (colour), and calls UV0__12VIPrimBufferR7VIVect2, Normal__12VIPrimBufferR7VIVect3,
//      Color__12VIPrimBufferR9VIColor32 and Vertex__12VIPrimBufferR7VIVect3 with the locals; End__12VIPrimBuffer(buffer) closes the group;
//   5. when A is non-zero EndMaterial__12VIPrimBuffer(buffer) is called, then Unlock__12VIPrimBuffer(buffer) (negative returns -1);
//   6. ReadEnd__9VIObjFile(file): a negative result returns -1, otherwise the buffer id from step 2.
// Inferred: the version-0 primitive buffer record: A decides whether a material was opened, B groups of N vertices each with a
// position, UV, normal and RGBA byte colour. Results of the Read calls are ignored.
// Unknown: the member names, what A, B and C mean to Init, and the VIPrimBufferType value 0.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int ReadEnd__9VIObjFile(void* file);
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int Read__9VIObjFileRUc(void* file, unsigned char* value);
int CreatePrimBuffer__8VIRasteri(void* raster, int type);
void* PrimBuffer__8VIRasteri(void* raster, int id);
int Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(void* buffer, void* raster, int type, int a, int b, int c);
int Lock__12VIPrimBuffer(void* buffer);
int Unlock__12VIPrimBuffer(void* buffer);
void BeginMaterial__12VIPrimBufferi(void* buffer, int material);
void EndMaterial__12VIPrimBuffer(void* buffer);
void Begin__12VIPrimBufferi(void* buffer, int count);
void End__12VIPrimBuffer(void* buffer);
void UV0__12VIPrimBufferR7VIVect2(void* buffer, float* uv);
void Normal__12VIPrimBufferR7VIVect3(void* buffer, float* normal);
void Color__12VIPrimBufferR9VIColor32(void* buffer, unsigned char* color);
void Vertex__12VIPrimBufferR7VIVect3(void* buffer, float* vertex);

int ParsePrimBufferObjV0__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    float vertex[3] = {0.0f, 0.0f, 0.0f};
    float uv[2] = {0.0f, 0.0f};
    float normal[3] = {0.0f, 0.0f, 0.0f};
    unsigned char color[4] = {0, 0, 0, 255};
    int currentMaterial = -1;
    int a;
    int b;
    int c;
    int count;
    int material;
    Read__9VIObjFileRi(file, &a);
    Read__9VIObjFileRi(file, &b);
    Read__9VIObjFileRi(file, &c);
    int id = CreatePrimBuffer__8VIRasteri(NORRATH_WORD_ADDRESS(words[3]), static_cast<int>(words[0]));
    if (id < 0) return -1;
    void* buffer = PrimBuffer__8VIRasteri(NORRATH_WORD_ADDRESS(words[3]), id);
    if (Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(buffer, NORRATH_WORD_ADDRESS(words[3]), 0, a, b, c) < 0) return -1;
    if (Lock__12VIPrimBuffer(buffer) < 0) return -1;
    for (int group = 0; group < b; ++group) {
        Read__9VIObjFileRi(file, &count);
        Read__9VIObjFileRi(file, &material);
        if (currentMaterial != material) {
            if (currentMaterial != -1) EndMaterial__12VIPrimBuffer(buffer);
            BeginMaterial__12VIPrimBufferi(buffer, material);
            currentMaterial = material;
        }
        Begin__12VIPrimBufferi(buffer, count);
        for (int vertexIndex = 0; vertexIndex < count; ++vertexIndex) {
            Read__9VIObjFileRf(file, &vertex[0]);
            Read__9VIObjFileRf(file, &vertex[1]);
            Read__9VIObjFileRf(file, &vertex[2]);
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
            Vertex__12VIPrimBufferR7VIVect3(buffer, vertex);
        }
        End__12VIPrimBuffer(buffer);
    }
    if (a != 0) EndMaterial__12VIPrimBuffer(buffer);
    if (Unlock__12VIPrimBuffer(buffer) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : id;
}
}
