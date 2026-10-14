// Observed: ReplaceStaticProxyActors__7VISceneR6VIZone at 0x01144218 is 296 bytes.
// Arguments: the scene and a zone. The zone holds an array at +92 (count at +92, data
// pointer at +96) of 120-byte room records, walked from 0 to the count (re-read each
// pass, as is the data pointer). Each room has an actor count at +60 and a pointer
// table at +64 of 32-bit actor words. For each actor whose word +64 has bit 2 (mask
// 0x4) set it calls Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, actor word
// +100, &type, &value), where dictionary is the word at scene + 19352; a zero result
// returns -1. Otherwise it calls ReplaceProxyActor__7VISceneii(scene, actor, value) and
// a negative result returns -1. The type value is neither read nor checked. It returns
// 0 when every room has been walked.
// Inferred: the +100 word is a resource id naming the real actor to substitute for a
// proxy actor; mask 0x4 marks proxy actors. Unknown: what the actor words point to.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

extern "C" {
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* value);
int ReplaceProxyActor__7VISceneii(unsigned char* scene, int actor, int replacement);

int ReplaceStaticProxyActors__7VISceneR6VIZone(unsigned char* scene, unsigned char* zone) {
    unsigned* array = reinterpret_cast<unsigned*>(zone + 92);
    if (static_cast<int>(array[0]) > 0) {
        int room = 0;
        do {
            unsigned char* record = at(array[1]) + room * 120;
            if (*reinterpret_cast<int*>(record + 60) > 0) {
                int j = 0;
                do {
                    unsigned char* current = at(array[1]) + room * 120;
                    unsigned actor_word = reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(current + 64)))[j];
                    unsigned char* actor = at(actor_word);
                    ++j;
                    if (*reinterpret_cast<unsigned*>(actor + 64) & 4u) {
                        int found[2];
                        void* dictionary = at(*reinterpret_cast<unsigned*>(scene + 19352));
                        if (Find__12VIDictionaryUiR14VIResourceTypeRi(dictionary, *reinterpret_cast<unsigned*>(actor + 100), &found[0], &found[1]) == 0)
                            return -1;
                        if (ReplaceProxyActor__7VISceneii(scene, static_cast<int>(actor_word), found[1]) < 0)
                            return -1;
                    }
                } while (j < *reinterpret_cast<int*>(at(array[1]) + room * 120 + 60));
            }
            ++room;
        } while (room < static_cast<int>(array[0]));
    }
    return 0;
}
}
