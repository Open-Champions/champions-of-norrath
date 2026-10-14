// Observed: CreateActor__7VISceneRC7VIVect3T1fi at 0x0113ed08, 500 bytes. Arguments: the scene (S), two three-float vectors (A and B, read-only), a float (F)
// and a sprite index (I). It calls Add__t6VIList1Z7VIActor(S+972); a negative result returns -1, otherwise the result N is the new actor
// record. N+68..N+76 receive A, N+80..N+88 receive B, N+92 gets F and N+96 gets I. The words at N+64, N+100, N+128 and N+132 are set
// to zero and N+104, N+108, N+152 and N+156 to -1; N+112..N+120 are set to 0.0f and N+124 to 1.0f.
// With P the sprite word of record I in the pool at S+996 (record array pointer 8 bytes in, 12-byte records), it calls the function in slot 16 of
// the table at P+60 with (P + halfword adjustment, S, N+80). Only when that returns exactly 1 it calls slot 72 of P's table (re-read from the pool) with
// (P + adjustment, S, the words at S+19356, S+19360 and S+19364, 0); the result R replaces N+96, the sprite at index R gets its word at +56
// set to I, and the sprite at index I has its word at +4 incremented. In every case the sprite whose index is now in N+96 then has
// its word at +4 incremented, and UpdateActor__7VIScenei(S, N) is called; a negative result returns -1, otherwise the function returns N.
// Inferred: slot 16 asks whether the sprite needs its own per-actor copy, slot 72 makes that copy (the copy points back at the shared sprite through
// +56, which ReleaseSprite__7VIScenei follows), +4 is a reference count and N+112..N+124 is an identity quaternion or four-component vector.
// The slot-16 call leaves the raw sprite pointer in the fourth argument register; it is not an argument and is not passed here.
// Unknown: the meaning of the zero and -1 fields, of F, and of the slot-16 return values other than 1.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
extern "C" void* norrath_host_function(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#define NORRATH_WORD_TO_FUNCTION(word) norrath_host_function(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#define NORRATH_WORD_TO_FUNCTION(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
int Add__t6VIList1Z7VIActor(unsigned char* list);
int UpdateActor__7VIScenei(unsigned char* scene, int actor);

int CreateActor__7VISceneRC7VIVect3T1fi(unsigned char* self, const unsigned char* a, const unsigned char* b, float value, int sprite) {
    int added = Add__t6VIList1Z7VIActor(self + 972);
    if (added < 0) return -1;
    unsigned char* actor = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(added));
    unsigned char* pool = self + 996;
    *reinterpret_cast<unsigned*>(actor + 68) = *reinterpret_cast<const unsigned*>(a);
    *reinterpret_cast<unsigned*>(actor + 72) = *reinterpret_cast<const unsigned*>(a + 4);
    *reinterpret_cast<unsigned*>(actor + 76) = *reinterpret_cast<const unsigned*>(a + 8);
    unsigned char* object = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + sprite * 12));
    unsigned char* slot = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(object + 60)) + 16;
    *reinterpret_cast<unsigned*>(actor + 80) = *reinterpret_cast<const unsigned*>(b);
    *reinterpret_cast<unsigned*>(actor + 128) = 0;
    *reinterpret_cast<unsigned*>(actor + 84) = *reinterpret_cast<const unsigned*>(b + 4);
    *reinterpret_cast<unsigned*>(actor + 64) = 0;
    *reinterpret_cast<int*>(actor + 152) = -1;
    *reinterpret_cast<unsigned*>(actor + 88) = *reinterpret_cast<const unsigned*>(b + 8);
    *reinterpret_cast<int*>(actor + 156) = -1;
    *reinterpret_cast<unsigned*>(actor + 132) = 0;
    *reinterpret_cast<float*>(actor + 92) = value;
    *reinterpret_cast<int*>(actor + 96) = sprite;
    *reinterpret_cast<float*>(actor + 124) = 1.0f;
    *reinterpret_cast<float*>(actor + 120) = 0.0f;
    *reinterpret_cast<float*>(actor + 112) = 0.0f;
    *reinterpret_cast<float*>(actor + 116) = 0.0f;
    *reinterpret_cast<unsigned*>(actor + 100) = 0;
    *reinterpret_cast<int*>(actor + 104) = -1;
    typedef int (*Probe)(unsigned char*, unsigned char*, unsigned char*);
    Probe probe = reinterpret_cast<Probe>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(slot + 4)));
    *reinterpret_cast<int*>(actor + 108) = -1;
    if (probe(object + *reinterpret_cast<short*>(slot), self, actor + 80) == 1) {
        object = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8)) + sprite * 12));
        unsigned char* make = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(object + 60)) + 72;
        typedef int (*Make)(unsigned char*, unsigned char*, unsigned, unsigned, unsigned, int);
        Make function = reinterpret_cast<Make>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(make + 4)));
        int copy = function(object + *reinterpret_cast<short*>(make), self, *reinterpret_cast<unsigned*>(self + 19356), *reinterpret_cast<unsigned*>(self + 19360),
                            *reinterpret_cast<unsigned*>(self + 19364), 0);
        *reinterpret_cast<int*>(actor + 96) = copy;
        unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8));
        unsigned char* shared = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(records + sprite * 12));
        *reinterpret_cast<int*>(NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(records + copy * 12)) + 56) = sprite;
        *reinterpret_cast<int*>(shared + 4) += 1;
    }
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(pool + 8));
    unsigned char* own = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(records + *reinterpret_cast<int*>(actor + 96) * 12));
    *reinterpret_cast<int*>(own + 4) += 1;
    if (UpdateActor__7VIScenei(self, added) < 0) return -1;
    return added;
}
}
