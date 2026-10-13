// Observed: Render__16VIParticleSystemlR8VIRaster at 0x010b8158 is 1280 bytes and
// returns 0. Arguments: object, a 64-bit time value, a raster.
//  1. dt = (float)(int)(time - the 64-bit value at 8) * 0.001f (bits 0x3a83126e),
//     limited to at most 0x3dcccccc with min.s; the time is stored at 8. The
//     seven floats at 20 through 44 are summed in that order, dt is added, the
//     total is multiplied by 0.125f and stored at 16. The eight floats at 20
//     through 48 shift up one slot (48 takes 44, ... 24 takes 20) and dt is stored
//     at 20.
//  2. The list whose head word is at 52 is walked (a node's first word is its next
//     node; a node whose next is zero ends the walk). For each node whose list
//     head word at node+160 points to a node with a nonzero next:
//     SetMaterial__8VIRasteri(raster, word at (word at node+84) + 776); the six
//     floats at node+940 are set to 1e7 (the first three) and -1e7 (the last
//     three); the active count is the word at node+172.
//  3. For each particle (104 bytes, next word first), with dt16 the float at 16:
//     a displacement is (floats at +44, +48, +52) * dt16; the three velocity floats
//     are multiplied by 1.0f - float24 * dt16. If no billboard batch is open,
//     BeginBillboards__8VIRasterPP11VIBillboardi(raster, &cursor, count) opens one
//     (its return is the batch capacity) with count the remaining active count.
//     Mul__C10VIMatrix44RC7VIVect3R7VIVect3(node+96, particle+32, cursor) writes
//     the transformed position to the 48-byte entry; float88 goes to entry+16 and
//     +20, word100 << 2 to +12, and 16 bytes from node+244 + 16 * index to +32
//     with index = (int)((float16 * 31.0f) * float28) & 31. The position at +32,
//     +36, +40 then gains the displacement. Per axis, the minimum and maximum of
//     the position over all particles are kept in node+940..948 and node+952..960.
//     The cursor advances 48 bytes; when the capacity is used up
//     EndBillboards__8VIRaster closes the batch and the cursor is cleared.
//  4. When the word at node+984 is nonzero the vector (node+964) - position is
//     normalized with Normalize__7VIVect3, scaled by float(node+976) * dt16 and
//     added to the velocity.
//  5. float(+48) gains float(+96) * dt16, float(+16) gains float(+20) * dt16 and
//     float(+88) gains float(+92) * dt16. When 1.0f <= float(+16) the particle is
//     removed with Remove__7VILNode, the count at node+172 drops by one and it is
//     appended with AddTail__7VILListP7VILNode to the list at this+64.
//  6. A batch left open at the end of a node's list is closed.
// Inferred: this is a particle emitter list drawn as billboards and the list at
// this+64 is the free list. Unknown: names of the node and particle fields.
extern "C" {
void SetMaterial__8VIRasteri(void* raster, int material);
int BeginBillboards__8VIRasterPP11VIBillboardi(void* raster, unsigned char** cursor, int count);
void EndBillboards__8VIRaster(void* raster);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(const void* matrix, const void* in, void* out);
int Normalize__7VIVect3(void* vector);
void Remove__7VILNode(void* node);
void AddTail__7VILListP7VILNode(void* list, void* node);

static float& float_at(unsigned char* base, int offset) {
    return *reinterpret_cast<float*>(base + offset);
}
static unsigned int& word_at(unsigned char* base, int offset) {
    return *reinterpret_cast<unsigned int*>(base + offset);
}
static unsigned char* pointer_at(unsigned char* base, int offset) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word_at(base, offset)));
}

static float float_from_bits(unsigned int bits) {
    return __builtin_bit_cast(float, bits);
}

int Render__16VIParticleSystemlR8VIRaster(void* object, long long time, void* raster) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    int elapsed = static_cast<int>(time - *reinterpret_cast<long long*>(bytes + 8));
    *reinterpret_cast<long long*>(bytes + 8) = time;
    float dt = static_cast<float>(elapsed) * float_from_bits(0x3a83126eu);
    float limit = float_from_bits(0x3dccccccu);
    dt = dt < limit ? dt : limit;
    float total = 0.0f;
    for (int offset = 20; offset <= 44; offset += 4) total += float_at(bytes, offset);
    total += dt;
    total *= 0.125f;
    for (int offset = 48; offset >= 24; offset -= 4) float_at(bytes, offset) = float_at(bytes, offset - 4);
    float_at(bytes, 20) = dt;
    float_at(bytes, 16) = total;

    unsigned char* node = pointer_at(bytes, 52);
    unsigned char* next_node = pointer_at(node, 0);
    while (next_node != 0) {
        unsigned char* particle = pointer_at(node, 160);
        unsigned char* following = pointer_at(particle, 0);
        if (following != 0) {
            SetMaterial__8VIRasteri(raster, word_at(pointer_at(node, 84), 776));
            int remaining = word_at(node, 172);
            float vector[3];
            unsigned char* cursor = 0;
            int capacity = 0;
            float_at(node, 940) = 1e7f; float_at(node, 944) = 1e7f; float_at(node, 948) = 1e7f;
            float_at(node, 952) = -1e7f; float_at(node, 956) = -1e7f; float_at(node, 960) = -1e7f;
            while (following != 0) {
                float step = float_at(bytes, 16);
                float* velocity = reinterpret_cast<float*>(particle + 44);
                float* position = reinterpret_cast<float*>(particle + 32);
                vector[0] = velocity[0] * step; vector[1] = velocity[1] * step; vector[2] = velocity[2] * step;
                float drag = 1.0f - float_at(particle, 24) * step;
                velocity[2] *= drag; velocity[0] *= drag; velocity[1] *= drag;
                if (cursor == 0) capacity = BeginBillboards__8VIRasterPP11VIBillboardi(raster, &cursor, remaining);
                Mul__C10VIMatrix44RC7VIVect3R7VIVect3(node + 96, position, cursor);
                float_at(cursor, 16) = float_at(particle, 88);
                float_at(cursor, 20) = float_at(particle, 88);
                word_at(cursor, 12) = word_at(particle, 100) << 2;
                int index = static_cast<int>((float_at(particle, 16) * 31.0f) * float_at(particle, 28)) & 31;
                const unsigned char* color = node + 244 + index * 16;
                for (int k = 0; k < 16; ++k) cursor[32 + k] = color[k];
                position[0] = position[0] + vector[0];
                position[1] = position[1] + vector[1];
                position[2] = position[2] + vector[2];
                if (position[0] < float_at(node, 940)) float_at(node, 940) = position[0];
                if (position[1] < float_at(node, 944)) float_at(node, 944) = position[1];
                if (position[2] < float_at(node, 948)) float_at(node, 948) = position[2];
                if (float_at(node, 952) < position[0]) float_at(node, 952) = position[0];
                if (float_at(node, 956) < position[1]) float_at(node, 956) = position[1];
                if (float_at(node, 960) < position[2]) float_at(node, 960) = position[2];
                cursor += 48;
                --capacity;
                --remaining;
                if (capacity == 0) {
                    EndBillboards__8VIRaster(raster);
                    cursor = 0;
                }
                if (word_at(node, 984) != 0) {
                    vector[0] = float_at(node, 964) - position[0];
                    vector[1] = float_at(node, 968) - position[1];
                    vector[2] = float_at(node, 972) - position[2];
                    Normalize__7VIVect3(vector);
                    float pull = float_at(node, 976) * float_at(bytes, 16);
                    vector[2] *= pull; vector[0] *= pull; vector[1] *= pull;
                    velocity[0] = velocity[0] + vector[0];
                    velocity[1] = velocity[1] + vector[1];
                    velocity[2] = velocity[2] + vector[2];
                }
                step = float_at(bytes, 16);
                float_at(particle, 48) = float_at(particle, 48) + float_at(particle, 96) * step;
                float_at(particle, 16) = float_at(particle, 16) + float_at(particle, 20) * float_at(bytes, 16);
                float_at(particle, 88) = float_at(particle, 88) + float_at(particle, 92) * float_at(bytes, 16);
                if (1.0f <= float_at(particle, 16)) {
                    Remove__7VILNode(particle);
                    word_at(node, 172) = word_at(node, 172) - 1;
                    AddTail__7VILListP7VILNode(bytes + 64, particle);
                }
                particle = following;
                following = pointer_at(particle, 0);
            }
            if (cursor != 0) {
                EndBillboards__8VIRaster(raster);
                cursor = 0;
            }
        }
        node = next_node;
        next_node = pointer_at(node, 0);
    }
    return 0;
}
}
