// Observed _GLOBAL_$I$netInterfaceIsUp at 0x001357a0, 32 bytes. It calls the file-local helper at 0x00134b90
// (816 bytes) with arguments 1 and 0xffff; the helper does nothing for any other pair. For that pair it
// constructs 8 identical 80-byte objects, in this order:
//   compImmediateCompressor (0x004e2140), compImmediateDecompressor (0x004e2190), compLocalCompressor
//   (0x004e21e0), compLocalDecompressor (0x004e2230), then for each of the 4 records of `partners`
//   (0x004e2280, stride 17632) the object at record +16 and the object at record +96.
// Constructing one object: five memset calls of 16 bytes each over its bytes 0 to 79, then the word at +72
// is set to 0 and the floats at +8, +24, +40 and +56 are set to 128.0 (bits 0x43000000).
// The standalone four have symbol-table spacing of exactly 80 bytes, so 80 is both the memset total and the gap.
// The partners records (17632 bytes) are 4 x 17632 = 70528 bytes, the extent the session-roster package gives.
// Inferred: the 80-byte unit is an inlined class constructor; the names suggest compression state but nothing
//   here relies on that.
// Unknown: the class, what the four 128.0 values are, the 16 bytes at the start of each partners record
//   and the bytes after +176 (none is touched here), and why the call carries 1 and 0xffff.
extern "C" {
extern unsigned char compImmediateCompressor[80];
extern unsigned char compImmediateDecompressor[80];
extern unsigned char compLocalCompressor[80];
extern unsigned char compLocalDecompressor[80];
extern unsigned char partners[4 * 17632];
void* memset(void* destination, int value, __SIZE_TYPE__ count);
}

namespace {
void store_float_128(unsigned char* where) {
    where[0] = 0;
    where[1] = 0;
    where[2] = 0;
    where[3] = 0x43;
}

void construct(unsigned char* object) {
    memset(object, 0, 16);
    memset(object + 16, 0, 16);
    memset(object + 32, 0, 16);
    memset(object + 48, 0, 16);
    memset(object + 64, 0, 16);
    object[72] = 0;
    object[73] = 0;
    object[74] = 0;
    object[75] = 0;
    store_float_128(object + 56);
    store_float_128(object + 8);
    store_float_128(object + 24);
    store_float_128(object + 40);
}

void initialize(int phase, int mask) {
    if (mask != 0xffff || phase != 1) return;
    construct(compImmediateCompressor);
    construct(compImmediateDecompressor);
    construct(compLocalCompressor);
    construct(compLocalDecompressor);
    for (int i = 0; i < 4; ++i) {
        unsigned char* record = partners + i * 17632;
        construct(record + 16);
        construct(record + 96);
    }
}
}

extern "C" void _GLOBAL_$I$netInterfaceIsUp() { initialize(1, 0xffff); }
