// Observed: ReadFileHeader__9VIObjFilei at 0x010a5418, 216 bytes. With (this, restore): pos = Tell__6VIFile(this) (a 64-bit
// result); when pos is nonzero, Seek__6VIFilelQ26VIFile10SeekOrigin(this, 0, 1). Then Read__6VIFilePvi(this, this + 200, 32)
// fills the 32-byte header at +200. When the global word VIEndian is not 1, VIBufferSwap__FiPUc is applied to the header fields:
// 4 bytes at +200, 4 at +204, 4 at +208, 8 at +216, 8 at +224 (each swap is called with the byte count, then the address).
// When restore is nonzero, Seek(this, pos, 1) is called afterwards. The result is 0.
// Inferred: the header holds a magic and two words at +200..+211, a gap word at +212, a 64-bit data start at +216 and a 64-bit
// cursor at +224, byte-reversed to the host order when VIEndian differs from 1; origin 1 reads as "from the start", as PostOpen
// uses it. The Read result is not checked.
// Unknown: the meaning of VIEndian values other than 1, and the +212 word (it is read as part of the 32 bytes but not swapped).
extern "C" {
extern int VIEndian;
long long Tell__6VIFile(void* file);
int Seek__6VIFilelQ26VIFile10SeekOrigin(void* file, long long offset, int origin);
int Read__6VIFilePvi(void* file, void* buffer, int size);
void VIBufferSwap__FiPUc(int size, unsigned char* buffer);

int ReadFileHeader__9VIObjFilei(void* object, int restore) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned char* header = bytes + 200;
    long long position = Tell__6VIFile(object);
    if (position != 0) Seek__6VIFilelQ26VIFile10SeekOrigin(object, 0, 1);
    Read__6VIFilePvi(object, header, 32);
    if (VIEndian != 1) {
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
