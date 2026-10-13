// Observed: __12VIAtmosphere at 0x01066538 is 580 bytes and returns nothing. It
// calls __7VILList on offsets 20 and 36, then stores 0.0f into these float
// ranges: 3 floats at 60; 6 floats at 80 (two groups of three); 6 floats at 104
// (two groups of three); 192 floats at 168 (64 groups of three); three 64-byte
// zero blocks at 1280, 1344 and 1408 (the original zeroes each as sixteen floats
// and with four 16-byte vector stores, which are the inlined VIMatrix44
// constructor); 512 floats at 1568 (128 groups of four); 3 floats at 3888; 3
// floats at 3900; and 128 floats at 3936 (32 groups of four). It then calls
// __7VILList on offsets 4448 and 4464 and stores zero in the words at 1228, 4476,
// 12 and 48.
// Inferred: the zero ranges are arrays of vectors and matrices of the atmosphere
// state; the three 64-byte blocks are VIMatrix44 members. Unknown: the member
// names and which ranges are structure arrays.
extern "C" {
void __7VILList(void* list);

static void zero_floats(unsigned char* base, int offset, int count) {
    float* p = reinterpret_cast<float*>(base + offset);
    for (int i = 0; i < count; ++i) p[i] = 0.0f;
}

void __12VIAtmosphere(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    __7VILList(bytes + 20);
    __7VILList(bytes + 36);
    zero_floats(bytes, 60, 3);
    zero_floats(bytes, 80, 6);
    zero_floats(bytes, 104, 6);
    zero_floats(bytes, 168, 192);
    zero_floats(bytes, 1280, 16);
    zero_floats(bytes, 1344, 16);
    zero_floats(bytes, 1408, 16);
    zero_floats(bytes, 1568, 512);
    zero_floats(bytes, 3888, 3);
    zero_floats(bytes, 3900, 3);
    zero_floats(bytes, 3936, 128);
    __7VILList(bytes + 4448);
    __7VILList(bytes + 4464);
    *reinterpret_cast<int*>(bytes + 1228) = 0;
    *reinterpret_cast<int*>(bytes + 4476) = 0;
    *reinterpret_cast<int*>(bytes + 12) = 0;
    *reinterpret_cast<int*>(bytes + 48) = 0;
}
}
