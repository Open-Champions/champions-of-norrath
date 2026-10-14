// Observed: Init__12VICollBuffer16VICollBufferTypeiii at 0x0108b950 is 148 bytes. It stores the
// three integer arguments at offsets 48, 56 and 64 and the type at offset 8 (a type of 2 is stored as
// 3). It then calls CalcDataSize__12VICollBuffer(this); a negative result returns -1. It calls
// Allocate__12VICollBuffer(this); a negative result also returns -1. On success it adds the word
// at offset 88 (the allocated block base) to the three pointer-like words at offsets 68, 72 and 76,
// and returns 0. The failure paths leave those three words as the callees left them.
// Inferred: offsets 68/72/76 hold section offsets inside one block that Allocate places at
// offset 88; the integers are capacity-style sizes. Unknown: names and meaning of the three
// integers and of the numeric type values.

extern "C" {
int CalcDataSize__12VICollBuffer(unsigned char* self);
int Allocate__12VICollBuffer(unsigned char* self);

int Init__12VICollBuffer16VICollBufferTypeiii(unsigned char* self, int type, int a, int b, int c) {
    *reinterpret_cast<int*>(self + 48) = a;
    *reinterpret_cast<int*>(self + 56) = b;
    *reinterpret_cast<int*>(self + 64) = c;
    *reinterpret_cast<int*>(self + 8) = type;
    if (type == 2)
        *reinterpret_cast<int*>(self + 8) = 3;
    if (CalcDataSize__12VICollBuffer(self) < 0 || Allocate__12VICollBuffer(self) < 0)
        return -1;
    unsigned base = *reinterpret_cast<unsigned*>(self + 88);
    *reinterpret_cast<unsigned*>(self + 68) += base;
    *reinterpret_cast<unsigned*>(self + 72) += base;
    *reinterpret_cast<unsigned*>(self + 76) += base;
    return 0;
}
}
