/* Observed lgCodecDecode at 0x002eb028, 208 bytes. Arguments: a codec object (whose first
 * word is the codec kind), a source, a source size, a destination and a pointer
 * to the destination size. A null codec returns 0x80000004. Kind 300 is
 * passed on unchanged to lgCodecGSMDecode as a tail call (the five arguments and
 * the result are its own). Any other kind except 10 returns 0x80000000. Kind
 * 10 is a plain copy: a null destination or source returns 0x80000004;
 * otherwise the smaller (signed) of the source size and the word at the size
 * pointer is copied from the source to the destination, that count is stored
 * through the size pointer, and the result is 0. The smaller-of helper is the
 * original's own two-instruction routine at 0x002eb670; it is written here as
 * a file-local function.
 * The meaning of kinds 10 and 300 is not established here.
 */
extern "C" {
unsigned lgCodecGSMDecode(const void* codec, const void* source, int source_size, void* destination, int* size);
void* memcpy(void* destination, const void* source, __SIZE_TYPE__ count);

static int smaller(int first, int second) { return first < second ? first : second; }

unsigned lgCodecDecode(const int* codec, const void* source, int source_size, void* destination, int* size) {
    if (codec == nullptr) return 0x80000004u;
    if (codec[0] == 10) {
        if (destination == nullptr || source == nullptr) return 0x80000004u;
        const int count = smaller(source_size, *size);
        memcpy(destination, source, static_cast<__SIZE_TYPE__>(count));
        *size = count;
        return 0;
    }
    if (codec[0] != 300) return 0x80000000u;
    return lgCodecGSMDecode(codec, source, source_size, destination, size);
}
}
