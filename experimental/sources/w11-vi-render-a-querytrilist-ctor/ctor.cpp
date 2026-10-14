// Observed: __14VIQueryTriList at 0x010d0c58 is 128 bytes and returns nothing
// observable. It calls __t7VIArray1Z7VIVect3 on offsets 12 and 20, stores 0.0f in
// the three floats at 28, 32 and 36, and stores 0.0f in six consecutive
// 16-byte groups of four floats from offset 44 (96 bytes, offsets 44 to 139).
// Finally it stores zero in the words at 8, 0 and 4.
// Inferred: the 12 bytes at 28 are a vector and the 96 bytes at 44 are six
// four-float records (Init sets up a nine-plane clipper, see Init).
extern "C" {
void __t7VIArray1Z7VIVect3(void* array);

void __14VIQueryTriList(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    __t7VIArray1Z7VIVect3(bytes + 12);
    __t7VIArray1Z7VIVect3(bytes + 20);
    float* vector = reinterpret_cast<float*>(bytes + 28);
    vector[0] = 0.0f;
    vector[1] = 0.0f;
    vector[2] = 0.0f;
    float* group = reinterpret_cast<float*>(bytes + 44);
    for (int i = 0; i < 6; ++i) {
        group[0] = 0.0f;
        group[1] = 0.0f;
        group[2] = 0.0f;
        group[3] = 0.0f;
        group += 4;
    }
    *reinterpret_cast<int*>(bytes + 8) = 0;
    *reinterpret_cast<int*>(bytes + 0) = 0;
    *reinterpret_cast<int*>(bytes + 4) = 0;
}
}
