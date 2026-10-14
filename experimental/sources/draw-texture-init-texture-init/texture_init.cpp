// Observed textureInit__Fii at 0x001725e0, 64 bytes. The address argument
// keeps its low 8 bits as a separate mask. 255 is added to the address, the
// mask is subtracted from the size, and both results are shifted right by 8
// with sign fill. Those words are stored before textureInternalInit__Fv.
// The size word is loaded again after that call and stored, minus one, to
// textureMaxBlockSize. The stores use the small-data addresses 0x0048695c,
// 0x00486960, and 0x004840b0. The same textureBufferStartAddress name also
// exists at 0x00486a34; this function does not reference that address.
// Fresh compilation is not an original-byte match. No EE path has run.

extern "C" {
unsigned textureBufferStartAddress;
unsigned textureBufferSize;
int textureMaxBlockSize;
void textureInternalInit__Fv();
}

static int shift_right_8(unsigned bits) {
    if ((bits & 0x80000000u) == 0u) return static_cast<int>(bits >> 8);
    return static_cast<int>((bits >> 8) | 0xff000000u);
}

extern "C" void textureInit__Fii(int address, int size) {
    const unsigned address_bits = static_cast<unsigned>(address);
    const unsigned low = address_bits & 0xffu;
    textureBufferStartAddress = static_cast<unsigned>(shift_right_8(address_bits + 255u));
    textureBufferSize = static_cast<unsigned>(shift_right_8(static_cast<unsigned>(size) - low));
    textureInternalInit__Fv();
    textureMaxBlockSize = static_cast<int>(textureBufferSize) - 1;
}
