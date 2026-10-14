// Observed: Init__16VIParticleSystemlP8VIRasteri at 0x010b7cb8 is 376 bytes.
// Arguments: object, a 64-bit value (stored whole at offset 8), a raster pointer
// (stored at 0), and a count (stored at 76). It requests 104*count bytes with
// __builtin_vec_new and stores the result at 80 (zero on failure). When the
// result is nonzero, __7VILNode is called on each of the count 104-byte records
// in order and in each record three floats at +32, three at +44, four at +56 and
// four at +72 are set to 0.0f. It then calls Init__7VILList on this+64 and, for
// index 0 up to the signed count at 76 (rechecked each time),
// AddTail__7VILListP7VILNode(this+64, *(this+80) + 104*index). It stores the
// float bits 0x3d088888 in the eight words at offsets 20 through 48 (descending
// from 48), zero at 16, and returns 0. The failure path of vector new is not
// reported to the caller; the AddTail loop runs over the count regardless.
// Inferred: 0x3d088888 is about 0.0333 (1/30). Unknown: the field meanings.
extern "C" {
void* __builtin_vec_new(unsigned int size);
void __7VILNode(void* node);
void Init__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);

static unsigned int& word_at(unsigned char* base, int offset) {
    return *reinterpret_cast<unsigned int*>(base + offset);
}

int Init__16VIParticleSystemlP8VIRasteri(void* object, long long value, void* raster, int count) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    *reinterpret_cast<long long*>(bytes + 8) = value;
    word_at(bytes, 0) = static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(raster));
    unsigned char* records = static_cast<unsigned char*>(__builtin_vec_new(count * 104));
    word_at(bytes, 76) = count;
    if (records != 0) {
        unsigned char* record = records;
        for (int remaining = count; remaining != 0; --remaining) {
            __7VILNode(record);
            float* a = reinterpret_cast<float*>(record + 32);
            a[2] = 0.0f; a[0] = 0.0f; a[1] = 0.0f;
            float* b = reinterpret_cast<float*>(record + 44);
            b[2] = 0.0f; b[0] = 0.0f; b[1] = 0.0f;
            float* c = reinterpret_cast<float*>(record + 56);
            c[3] = 0.0f; c[0] = 0.0f; c[1] = 0.0f; c[2] = 0.0f;
            float* d = reinterpret_cast<float*>(record + 72);
            d[3] = 0.0f; d[0] = 0.0f; d[1] = 0.0f; d[2] = 0.0f;
            record += 104;
        }
    }
    word_at(bytes, 80) = static_cast<unsigned int>(reinterpret_cast<__UINTPTR_TYPE__>(records));
    Init__7VILList(bytes + 64);
    for (int index = 0; index < static_cast<int>(word_at(bytes, 76)); ++index) {
        unsigned char* base = reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word_at(bytes, 80)));
        AddTail__7VILListP7VILNode(bytes + 64, base + index * 104);
    }
    for (int offset = 48; offset >= 20; offset -= 4) {
        *reinterpret_cast<unsigned int*>(bytes + offset) = 0x3d088888u;
    }
    word_at(bytes, 16) = 0;
    return 0;
}
}
