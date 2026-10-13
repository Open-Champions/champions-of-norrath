// Observed processParams__7VehiclePPc at 0x00271870, 688 bytes. V is the vehicle and P a null-terminated array of
// text pointers. Nothing happens for an empty array. For each text T the first matching rule applies, tested in
// this order with strstr (a substring test, not a prefix test):
//  1. "seatoffset": sscanf(T, "seatoffset%d=%f,%f,%f", &i, &x, &y, &z) and the floats (x, y, z) are stored at
//     V + 1932 + 12 * i. The index is not range checked.
//  2. "turret": sscanf(T, "turret%d=\"%[a-zA-Z0-9_-.]\",%f,%f,%f", &i, name, &x, &y, &z) with a 64-byte name buffer. A
//     304-byte block is allocated with __builtin_new; when it is not null its byte at +13 is set to 8. The block
//     address word is stored in the word slot at V + 2088 + 4 * i (null if the allocation failed; the following
//     stores then go through that slot without a check). In the block: the floats x, y and z go to +288, +292 and
//     +296. The word at +0 becomes lumpFindResource__FPcT0(V + 76, name). The first ".vif" inside name is replaced
//     by ".tex" (the pointer strstr returns is used unchecked). The word at +4 becomes
//     lumpFindResource__FPcT0(V + 76, name) again. The doubleword at +272 becomes
//     modelGetActiveChunks__FP12_modelHeaderi(word at +0 as a model, 0).
//  3. "disable": the word at V + 1024 becomes 1.
//  4. "stack": with n the word at V + 2168, sscanf(T, "stack%d=%f,%f,%f,%f,%f,%f", &i, ...) stores six floats at
//     V + 2172 + 24 * n, then the word at V + 2168 becomes n + 1.
// Inferred: this reads the "name=value" parameter strings of a vehicle definition (seat offsets, turrets with
// model files, a disable switch, stacked entries). Unknown: the field meanings, the size of the stack table, and the
// type of the 304-byte block. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
char* strstr(const char* haystack, const char* needle);
int sscanf(const char* text, const char* format, ...);
void* __builtin_new(unsigned size);
void* lumpFindResource__FPcT0(char* lump, char* name);
unsigned long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int mask);

void processParams__7VehiclePPc(void* self, char** params) {
    using namespace w12c;
    unsigned char* const v = static_cast<unsigned char*>(self);
    for (; *params != nullptr; ++params) {
        char* const text = *params;
        if (strstr(text, "seatoffset") != nullptr) {
            int i;
            float x, y, z;
            sscanf(text, "seatoffset%d=%f,%f,%f", &i, &x, &y, &z);
            real(v, 1932 + 12 * i) = x;
            real(v, 1936 + 12 * i) = y;
            real(v, 1940 + 12 * i) = z;
        } else if (strstr(text, "turret") != nullptr) {
            int i;
            float x, y, z;
            char name[64];
            sscanf(text, "turret%d=\"%[a-zA-Z0-9_-.]\",%f,%f,%f", &i, name, &x, &y, &z);
            unsigned char* object = static_cast<unsigned char*>(__builtin_new(304));
            if (object != nullptr) object[13] = 8;
            word(v, 2088 + 4 * i) = to_word(object);
            unsigned char* const record = from_word(word(v, 2088 + 4 * i));
            real(record, 288) = x;
            real(record, 296) = z;
            real(record, 292) = y;
            word(record, 0) = to_word(lumpFindResource__FPcT0(reinterpret_cast<char*>(v + 76), name));
            char* const extension = strstr(name, ".vif");
            extension[0] = '.';
            extension[1] = 't';
            extension[2] = 'e';
            extension[3] = 'x';
            extension[4] = '\0';
            word(from_word(word(v, 2088 + 4 * i)), 4) = to_word(lumpFindResource__FPcT0(reinterpret_cast<char*>(v + 76), name));
            unsigned char* const again = from_word(word(v, 2088 + 4 * i));
            dword(again, 272) = modelGetActiveChunks__FP12_modelHeaderi(from_word(word(again, 0)), 0);
        } else if (strstr(text, "disable") != nullptr) {
            word(v, 1024) = 1u;
        } else if (strstr(text, "stack") != nullptr) {
            int i;
            const int n = sword(v, 2168);
            sscanf(text, "stack%d=%f,%f,%f,%f,%f,%f", &i, &real(v, 2172 + 24 * n), &real(v, 2176 + 24 * n), &real(v, 2180 + 24 * n),
                   &real(v, 2184 + 24 * n), &real(v, 2188 + 24 * n), &real(v, 2192 + 24 * n));
            sword(v, 2168) = sword(v, 2168) + 1;
        }
    }
}
}
