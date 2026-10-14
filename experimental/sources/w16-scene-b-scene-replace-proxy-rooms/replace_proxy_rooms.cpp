// Observed: ReplaceProxyRooms__7VISceneR6VIZone at 0x01144138 is 220 bytes. Arguments:
// the scene and a zone. The zone holds an array at +92 (count at +92, data pointer
// at +96) of 120-byte room records, walked from index 0 up to the count, with the
// count and data pointer re-read every pass. For each record whose flag word (+0) has
// bit 1 (mask 0x2) set, it calls Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary,
// record word +40, &type, &value), where dictionary is the word at scene + 19352.
// If the lookup succeeds (non-zero) and type is 4, it stores value in the record's
// word +28 and clears mask 0x2 in the record's flag word; if type is not 4 it returns
// -1 at once. A failed lookup leaves the record alone. It returns 0 otherwise.
// Inferred: proxy rooms are placeholders resolved to a real resource of type 4 by
// the id in +40; clearing the flag marks the record resolved. Unknown: what type 4
// is named and what word +28 is used for afterwards.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* value);

int ReplaceProxyRooms__7VISceneR6VIZone(unsigned char* scene, unsigned char* zone) {
    unsigned* array = reinterpret_cast<unsigned*>(zone + 92);
    int i = 0;
    if (static_cast<int>(array[0]) > 0) {
        do {
            unsigned char* record = at(array[1]) + i * 120;
            if (*reinterpret_cast<unsigned*>(record) & 2u) {
                int found[2];
                void* dictionary = at(*reinterpret_cast<unsigned*>(scene + 19352));
                if (Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, *reinterpret_cast<unsigned*>(record + 40), &found[0], &found[1]) != 0) {
                    if (found[0] != 4)
                        return -1;
                    record = at(array[1]) + i * 120;
                    unsigned flags = *reinterpret_cast<unsigned*>(record);
                    *reinterpret_cast<int*>(record + 28) = found[1];
                    *reinterpret_cast<unsigned*>(record) = flags & ~2u;
                }
            }
            ++i;
        } while (i < static_cast<int>(array[0]));
    }
    return 0;
}
}
