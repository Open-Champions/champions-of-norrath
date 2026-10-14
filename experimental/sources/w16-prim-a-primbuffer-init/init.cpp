// Observed: Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii at 0x01102a10 is 136 bytes. Of
// the six arguments the raster reference (second) is never read. It stores the type at offset 8,
// the first integer at offset 68 and the second at offset 76, then calls
// CalcDataSize__12VIPrimBufferiii(this, first, second, third); a negative result returns -1. It
// then calls Allocate__12VIPrimBuffer(this); a negative result also returns -1. On success it adds
// the word at offset 104 (the allocated block base) to the words at offsets 88, 92 and 96 and
// returns 0. On failure those three words stay as the callees left them.
// Inferred: 88/92/96 are section offsets inside one block that Allocate places at offset 104,
// the same shape as the VICollBuffer initialiser. Unknown: the meaning of the three integers
// and of the type values.
extern "C" {
int CalcDataSize__12VIPrimBufferiii(unsigned char* self, int a, int b, int c);
int Allocate__12VIPrimBuffer(unsigned char* self);

int Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii(unsigned char* self, void*, int type, int a, int b, int c) {
    *reinterpret_cast<int*>(self + 8) = type;
    *reinterpret_cast<int*>(self + 68) = a;
    *reinterpret_cast<int*>(self + 76) = b;
    if (CalcDataSize__12VIPrimBufferiii(self, a, b, c) < 0 || Allocate__12VIPrimBuffer(self) < 0)
        return -1;
    unsigned base = *reinterpret_cast<unsigned*>(self + 104);
    *reinterpret_cast<unsigned*>(self + 88) += base;
    *reinterpret_cast<unsigned*>(self + 92) += base;
    *reinterpret_cast<unsigned*>(self + 96) += base;
    return 0;
}
}
