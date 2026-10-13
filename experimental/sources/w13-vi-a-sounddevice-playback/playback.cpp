// Observed: Playback__13VISoundDevicei at 0x01126998 is 72 bytes. Arguments: device (a0) and
// an id (a1). It computes (id & 0xff) % 46, reads the word at device + 3100 + 4 * that
// value, and if the word is non-null and the word at +32 of the object it points to equals
// the full id, returns the pointer; otherwise returns 0. Only the first object of a bucket
// is examined (no chain walk). No stores.
// Inferred: a 46-entry lookup table of playback objects keyed by id, the id stored at
// +32 of each object; the function finds a playback by id.
// Unknown: the object layout beyond +32, whether buckets chain elsewhere, and why the
// table size is 46.
// A host check defines NORRATH_HOST_ARENA because a 64-bit host cannot keep a target
// pointer in a 4-byte table word; the words then hold arena offsets.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* pointerAt(const unsigned char* p) {
    unsigned word = *reinterpret_cast<const unsigned*>(p);
    return word == 0 ? nullptr : norrath_host_pointer(word);
}
#else
static inline unsigned char* pointerAt(const unsigned char* p) {
    return *reinterpret_cast<unsigned char* const*>(p);
}
#endif

extern "C" {
void* Playback__13VISoundDevicei(unsigned char* device, int id) {
    unsigned char* node = pointerAt(device + 3100 + 4 * ((id & 0xff) % 46));
    if (node != 0 && *reinterpret_cast<int*>(node + 32) == id) {
        return node;
    }
    return 0;
}
}
