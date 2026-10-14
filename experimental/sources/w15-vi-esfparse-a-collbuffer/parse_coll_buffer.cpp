// Observed: ParseCollBuffer__10VIESFParse at 0x01170c50 is 1,208 bytes. Argument: parse (a0; creation
// argument word at +0, collision system word at +20, current-object word at +28 whose +84 word is
// a vertex count and +88 word a table of 12-byte entries, object file word at +36). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &version): negative, or a tag other than 0x4200,
//      returns -1;
//   2. reads, as signed words: a kind (only when the 16-bit version is 2 or more, else 0), then
//      three more values a, group count and c, and a packing (only when the version is 2 or more, else 0);
//   3. handle = CreateCollBuffer__9VICollidei(collide, parse[0]) (negative returns -1);
//      buffer = CollBuffer__9VICollidei(collide, handle);
//      Init__12VICollBuffer16VICollBufferTypeiii(buffer, kind, a, group count, c) (negative returns -1),
//      Lock__12VICollBuffer(buffer) (negative returns -1) and SetPacking__12VICollBufferi(buffer,
//      packing) (negative returns -1);
//   4. scale = 1.0f / powf(2.0f, (float)packing);
//   5. for each of the groups (the count is read again after every group): reads a vertex count, a group id
//      and a Begin argument (three Read__9VIObjFileRi); when the group id differs from the previous
//      one (starting at -1) it calls EndPrimGroup__12VICollBuffer(buffer) unless the previous id was
//      -1, then BeginPrimGroup__12VICollBuffer(buffer); then Begin__12VICollBufferi(buffer, argument),
//      the vertices, and End__12VICollBuffer(buffer). The vertices depend on the kind:
//        kind 0: three floats (Read__9VIObjFileRf) per vertex to Vertex__12VICollBufferR7VIVect3;
//        kind 1: three 16-bit values (Read__9VIObjFileRs) to
//                Vertex__12VICollBufferR10VIVect3S16;
//        kind 2: the word at buffer+8 selects: 2 reads three 16-bit values and a fourth 16-bit
//                value whose low byte (signed) goes to VertexGroup__12VICollBufferSc, then the
//                S16 vertex; 0 reads the same four values and converts the first three to floats
//                multiplied by the scale and added to the entry of the table at (+28 word)+88
//                indexed by the fourth value (12 bytes each), then Vertex (R7VIVect3) without any
//                group call; 3 reads the same four values, calls VertexGroup with the low byte,
//                FloraType__12VICollBufferSc with -1, then the S16 vertex; any other value
//                returns -1;
//        kind 3: requires the word at buffer+8 to be 3 (else -1); reads three 16-bit values and two
//                signed bytes (Read__9VIObjFileRSc), calls VertexGroup with the first byte,
//                FloraType with the second, then the S16 vertex;
//        any other kind returns -1;
//   6. after the groups it calls EndPrimGroup when a is non-zero; then
//      CalcBVolumes__12VICollBufferiP7VIVect3(buffer, count, table) with the +84 and +88 words of the
//      current object (0 and 0 when the +28 word or the count is zero); negative returns -1;
//      Unlock__12VICollBuffer(buffer) (negative returns -1);
//   7. returns -1 when ReadEnd__9VIObjFile is negative, else the collision buffer handle.
// Inferred: this builds a collision geometry buffer (the buffer types 0-3 appear to be float
// vertices, 16-bit fixed vertices, grouped vertices and flora-typed vertices; the packing is a
// power-of-two fixed-point exponent) and registers it with the collision system. Unknown: the
// names of the values a and c and of the buffer type at buffer+8, how the parse word at +28 is
// set, and the second ReadBegin output name (version is a guess from its use).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRs(void* file, short* value);
int Read__9VIObjFileRSc(void* file, signed char* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
float powf(float base, float exponent);
int CreateCollBuffer__9VICollidei(void* collide, int argument);
void* CollBuffer__9VICollidei(void* collide, int handle);
int Init__12VICollBuffer16VICollBufferTypeiii(void* buffer, int kind, int a, int b, int c);
int Lock__12VICollBuffer(void* buffer);
int Unlock__12VICollBuffer(void* buffer);
int SetPacking__12VICollBufferi(void* buffer, int packing);
int BeginPrimGroup__12VICollBuffer(void* buffer);
int EndPrimGroup__12VICollBuffer(void* buffer);
int Begin__12VICollBufferi(void* buffer, int argument);
int End__12VICollBuffer(void* buffer);
int Vertex__12VICollBufferR7VIVect3(void* buffer, const float* vertex);
int Vertex__12VICollBufferR10VIVect3S16(void* buffer, const short* vertex);
int VertexGroup__12VICollBufferSc(void* buffer, signed char group);
int FloraType__12VICollBufferSc(void* buffer, signed char type);
int CalcBVolumes__12VICollBufferiP7VIVect3(void* buffer, int count, void* table);

int ParseCollBuffer__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    void* collide = NORRATH_WORD_ADDRESS(words[5]);
    unsigned short tag;
    unsigned short version;
    int kind;
    int a;
    int group_count;
    int c;
    int packing;
    int previous_group = -1;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &version) < 0) return -1;
    if (tag != 0x4200) return -1;
    if (version < 2) kind = 0;
    else Read__9VIObjFileRi(file, &kind);
    Read__9VIObjFileRi(file, &a);
    Read__9VIObjFileRi(file, &group_count);
    Read__9VIObjFileRi(file, &c);
    if (version < 2) packing = 0;
    else Read__9VIObjFileRi(file, &packing);
    int handle = CreateCollBuffer__9VICollidei(collide, static_cast<int>(words[0]));
    if (handle < 0) return -1;
    void* buffer = CollBuffer__9VICollidei(collide, handle);
    if (Init__12VICollBuffer16VICollBufferTypeiii(buffer, kind, a, group_count, c) < 0) return -1;
    if (Lock__12VICollBuffer(buffer) < 0) return -1;
    if (SetPacking__12VICollBufferi(buffer, packing) < 0) return -1;
    float scale = 1.0f / powf(2.0f, static_cast<float>(packing));
    for (int index = 0; index < group_count; ++index) {
        int vertex_count;
        int group;
        int begin_argument;
        Read__9VIObjFileRi(file, &vertex_count);
        Read__9VIObjFileRi(file, &group);
        Read__9VIObjFileRi(file, &begin_argument);
        if (previous_group != group) {
            if (previous_group != -1) EndPrimGroup__12VICollBuffer(buffer);
            BeginPrimGroup__12VICollBuffer(buffer);
            previous_group = group;
        }
        if (kind == 0) {
            float vertex[3] = {0.0f, 0.0f, 0.0f};
            Begin__12VICollBufferi(buffer, begin_argument);
            for (int i = 0; i < vertex_count; ++i) {
                Read__9VIObjFileRf(file, &vertex[0]);
                Read__9VIObjFileRf(file, &vertex[1]);
                Read__9VIObjFileRf(file, &vertex[2]);
                Vertex__12VICollBufferR7VIVect3(buffer, vertex);
            }
        } else if (kind == 1) {
            short vertex[3] = {0, 0, 0};
            Begin__12VICollBufferi(buffer, begin_argument);
            for (int i = 0; i < vertex_count; ++i) {
                Read__9VIObjFileRs(file, &vertex[0]);
                Read__9VIObjFileRs(file, &vertex[1]);
                Read__9VIObjFileRs(file, &vertex[2]);
                Vertex__12VICollBufferR10VIVect3S16(buffer, vertex);
            }
        } else if (kind == 2) {
            int buffer_type = *reinterpret_cast<int*>(static_cast<unsigned char*>(buffer) + 8);
            short vertex[3] = {0, 0, 0};
            short vertex_group = 0;
            if (buffer_type == 2) {
                Begin__12VICollBufferi(buffer, begin_argument);
                for (int i = 0; i < vertex_count; ++i) {
                    Read__9VIObjFileRs(file, &vertex[0]);
                    Read__9VIObjFileRs(file, &vertex[1]);
                    Read__9VIObjFileRs(file, &vertex[2]);
                    Read__9VIObjFileRs(file, &vertex_group);
                    VertexGroup__12VICollBufferSc(buffer, static_cast<signed char>(vertex_group));
                    Vertex__12VICollBufferR10VIVect3S16(buffer, vertex);
                }
            } else if (buffer_type == 0) {
                float position[3] = {0.0f, 0.0f, 0.0f};
                Begin__12VICollBufferi(buffer, begin_argument);
                for (int i = 0; i < vertex_count; ++i) {
                    Read__9VIObjFileRs(file, &vertex[0]);
                    Read__9VIObjFileRs(file, &vertex[1]);
                    Read__9VIObjFileRs(file, &vertex[2]);
                    Read__9VIObjFileRs(file, &vertex_group);
                    position[0] = static_cast<float>(vertex[0]) * scale;
                    position[1] = static_cast<float>(vertex[1]) * scale;
                    position[2] = static_cast<float>(vertex[2]) * scale;
                    unsigned char* object = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[7]));
                    const float* offset = reinterpret_cast<const float*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(object + 88) + 12 * vertex_group));
                    position[0] += offset[0];
                    position[1] += offset[1];
                    position[2] += offset[2];
                    Vertex__12VICollBufferR7VIVect3(buffer, position);
                }
            } else if (buffer_type == 3) {
                Begin__12VICollBufferi(buffer, begin_argument);
                for (int i = 0; i < vertex_count; ++i) {
                    Read__9VIObjFileRs(file, &vertex[0]);
                    Read__9VIObjFileRs(file, &vertex[1]);
                    Read__9VIObjFileRs(file, &vertex[2]);
                    Read__9VIObjFileRs(file, &vertex_group);
                    VertexGroup__12VICollBufferSc(buffer, static_cast<signed char>(vertex_group));
                    FloraType__12VICollBufferSc(buffer, -1);
                    Vertex__12VICollBufferR10VIVect3S16(buffer, vertex);
                }
            } else {
                return -1;
            }
        } else if (kind == 3) {
            if (*reinterpret_cast<int*>(static_cast<unsigned char*>(buffer) + 8) != 3) return -1;
            short vertex[3] = {0, 0, 0};
            signed char vertex_group;
            signed char flora_type;
            Begin__12VICollBufferi(buffer, begin_argument);
            for (int i = 0; i < vertex_count; ++i) {
                Read__9VIObjFileRs(file, &vertex[0]);
                Read__9VIObjFileRs(file, &vertex[1]);
                Read__9VIObjFileRs(file, &vertex[2]);
                Read__9VIObjFileRSc(file, &vertex_group);
                Read__9VIObjFileRSc(file, &flora_type);
                VertexGroup__12VICollBufferSc(buffer, vertex_group);
                FloraType__12VICollBufferSc(buffer, flora_type);
                Vertex__12VICollBufferR10VIVect3S16(buffer, vertex);
            }
        } else {
            return -1;
        }
        End__12VICollBuffer(buffer);
    }
    if (a != 0) EndPrimGroup__12VICollBuffer(buffer);
    unsigned char* object = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(words[7]));
    int result;
    if (object != 0 && *reinterpret_cast<int*>(object + 84) != 0)
        result = CalcBVolumes__12VICollBufferiP7VIVect3(buffer, *reinterpret_cast<int*>(object + 84), NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(object + 88)));
    else
        result = CalcBVolumes__12VICollBufferiP7VIVect3(buffer, 0, 0);
    if (result < 0) return -1;
    if (Unlock__12VICollBuffer(buffer) < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : handle;
}
}
