// Observed: ParseHSpriteHierarchy__10VIESFParseR9VIHSprite at 0x01172928 is 736 bytes. Arguments:
// parse (a0; object file word at +36) and hsprite (a1, a hierarchical sprite). In order it:
//   1. ReadBegin__9VIObjFileRUsT1(file, &tag, &version): negative, or a tag other than 0x2400,
//      returns -1;
//   2. when the 16-bit version is non-zero: reads a signed count into the word at hsprite+168 and
//      then that many floats (Read__9VIObjFileRf, the word is read again after each float) into
//      consecutive words starting at hsprite+172 (the capacity is not checked);
//   3. reads a signed node count N (Read__9VIObjFileRi) and calls
//      Init__t8VIVector1Z13VIHSpriteNodei(hsprite + 64, N) and
//      Init__t8VIVector1Z10VIMatrix44i(hsprite + 76, N), each returning -1 when negative;
//   4. for each node index below N (the count is read again every iteration): reads a signed
//      parent index; the node is created with AddRoot__9VIHSprite(hsprite) when the parent index is
//      -1, otherwise with Add__9VIHSpritei(hsprite, parent); a negative node number returns -1;
//      then reads eight floats: four into a rotation record, one more float, and three into a
//      position record; SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3(hsprite, node, &rotation,
//      float, &position) (negative returns -1);
//   5. when the version is non-zero, reads a signed value into the word at node * 256 + 12 of the
//      node array whose pointer sits at hsprite + 72; when the version is 2 or more, reads three
//      floats as the node's fixed scale, otherwise the scale is (1.0, 1.0, 1.0);
//      SetNodeFixScale__9VIHSpriteiRC7VIVect3(hsprite, node, &scale) (negative returns -1);
//   6. returns -1 when ReadEnd__9VIObjFile is negative, else 0.
// Inferred: this reads a node hierarchy (parent links, bind pose, per-node value and optional fixed
// scale) into the sprite; the node entries are 256 bytes and the element pointer of the vector
// object at hsprite+64 is at its +8; version 1 added the first count and floats and the per-node
// value, version 2 the fixed scale. Unknown: the meaning of the extra float, of the per-node value
// and of the count/float table at hsprite+168, and the second ReadBegin output name (version is a
// guess from its use).

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Read__9VIObjFileRi(void* file, int* value);
int Read__9VIObjFileRf(void* file, float* value);
int ReadBegin__9VIObjFileRUsT1(void* file, unsigned short* tag, unsigned short* second);
int ReadEnd__9VIObjFile(void* file);
int Init__t8VIVector1Z13VIHSpriteNodei(void* vector, int count);
int Init__t8VIVector1Z10VIMatrix44i(void* vector, int count);
int AddRoot__9VIHSprite(void* hsprite);
int Add__9VIHSpritei(void* hsprite, int parent);
int SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3(void* hsprite, int node, const float* rotation, float extra, const float* position);
int SetNodeFixScale__9VIHSpriteiRC7VIVect3(void* hsprite, int node, const float* scale);

int ParseHSpriteHierarchy__10VIESFParseR9VIHSprite(unsigned char* parse, unsigned char* hsprite) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    unsigned short tag;
    unsigned short version;
    int node_count;
    float rotation[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float position[3] = {0.0f, 0.0f, 0.0f};
    float scale[3] = {0.0f, 0.0f, 0.0f};
    float extra;
    if (ReadBegin__9VIObjFileRUsT1(file, &tag, &version) < 0) return -1;
    if (tag != 0x2400) return -1;
    if (version != 0) {
        int* table_count = reinterpret_cast<int*>(hsprite + 168);
        Read__9VIObjFileRi(file, table_count);
        for (int i = 0; i < *table_count; ++i) Read__9VIObjFileRf(file, reinterpret_cast<float*>(hsprite + 172 + 4 * i));
    }
    Read__9VIObjFileRi(file, &node_count);
    if (Init__t8VIVector1Z13VIHSpriteNodei(hsprite + 64, node_count) < 0) return -1;
    if (Init__t8VIVector1Z10VIMatrix44i(hsprite + 76, node_count) < 0) return -1;
    for (int i = 0; i < node_count; ++i) {
        int parent;
        int node;
        Read__9VIObjFileRi(file, &parent);
        if (parent == -1) node = AddRoot__9VIHSprite(hsprite);
        else node = Add__9VIHSpritei(hsprite, parent);
        if (node < 0) return -1;
        Read__9VIObjFileRf(file, &rotation[0]);
        Read__9VIObjFileRf(file, &rotation[1]);
        Read__9VIObjFileRf(file, &rotation[2]);
        Read__9VIObjFileRf(file, &rotation[3]);
        Read__9VIObjFileRf(file, &extra);
        Read__9VIObjFileRf(file, &position[0]);
        Read__9VIObjFileRf(file, &position[1]);
        Read__9VIObjFileRf(file, &position[2]);
        if (SetNodeBindPose__9VIHSpriteiRC6VIQuatfRC7VIVect3(hsprite, node, rotation, extra, position) < 0) return -1;
        if (version != 0) {
            unsigned char* nodes = static_cast<unsigned char*>(NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(hsprite + 72)));
            Read__9VIObjFileRi(file, reinterpret_cast<int*>(nodes + node * 256 + 12));
        }
        if (version < 2) {
            scale[0] = 1.0f;
            scale[1] = 1.0f;
            scale[2] = 1.0f;
        } else {
            Read__9VIObjFileRf(file, &scale[0]);
            Read__9VIObjFileRf(file, &scale[1]);
            Read__9VIObjFileRf(file, &scale[2]);
        }
        if (SetNodeFixScale__9VIHSpriteiRC7VIVect3(hsprite, node, scale) < 0) return -1;
    }
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : 0;
}
}
