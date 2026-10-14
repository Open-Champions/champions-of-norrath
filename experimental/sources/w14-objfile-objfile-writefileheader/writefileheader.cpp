// Observed: WriteFileHeader__9VIObjFilei at 0x010a52e0, 308 bytes. With (this, restore): pos = Tell__6VIFile(this) (64-bit);
// when pos is nonzero, Seek__6VIFilelQ26VIFile10SeekOrigin(this, 0, 1). The global word VIEndian is read once. When it is not 1,
// VIBufferSwap__FiPUc is applied to the header fields (4 bytes at +200, 4 at +204, 4 at +208, 8 at +216, 8 at +224). Then
// Write__6VIFilePCvi(this, this + 200, 32) writes the 32 header bytes. When VIEndian (as read before) was not 1 the same five swaps
// run again, restoring the in-memory header. When restore is nonzero, Seek(this, pos, 1) follows. The result is 0.
// Inferred: the mirror of ReadFileHeader: the header is swapped to file order for the write and back afterwards; origin 1 reads as
// "from the start". The Write result is not checked.
// Unknown: the meaning of VIEndian values other than 1, and the +212 word (written as part of the 32 bytes but never swapped).
extern "C" {
extern int VIEndian;
long long Tell__6VIFile(void* file);
int Seek__6VIFilelQ26VIFile10SeekOrigin(void* file, long long offset, int origin);
int Write__6VIFilePCvi(void* file, const void* buffer, int size);
void VIBufferSwap__FiPUc(int size, unsigned char* buffer);

int WriteFileHeader__9VIObjFilei(void* object, int restore) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned char* header = bytes + 200;
    long long position = Tell__6VIFile(object);
    if (position != 0) Seek__6VIFilelQ26VIFile10SeekOrigin(object, 0, 1);
    int endian = VIEndian;
    if (endian != 1) {
        VIBufferSwap__FiPUc(4, header);
        VIBufferSwap__FiPUc(4, bytes + 204);
        VIBufferSwap__FiPUc(4, bytes + 208);
        VIBufferSwap__FiPUc(8, bytes + 216);
        VIBufferSwap__FiPUc(8, bytes + 224);
    }
    Write__6VIFilePCvi(object, header, 32);
    if (endian != 1) {
        VIBufferSwap__FiPUc(4, header);
        VIBufferSwap__FiPUc(4, bytes + 204);
        VIBufferSwap__FiPUc(4, bytes + 208);
        VIBufferSwap__FiPUc(8, bytes + 216);
        VIBufferSwap__FiPUc(8, bytes + 224);
    }
    if (restore != 0) Seek__6VIFilelQ26VIFile10SeekOrigin(object, position, 1);
    return 0;
}
}
