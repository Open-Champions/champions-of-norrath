// Observed: ParseActorObj__10VIESFParse at 0x01176ac8 is 408 bytes. Argument: parse (a0, the ESF
// parser record: scene word at +24, dictionary word at +32, object file word at +36). In order it
// reads from the object file:
//   1. an unsigned id; then, only when ObjectVersion__9VIObjFile(file) is non-zero, a second
//      unsigned word (otherwise that word is 0);
//   2. seven floats: three (a vector), three more (a second vector), then one scalar;
//   3. only when the object version is at least 2, four unsigned bytes (the fourth local starts
//      as 0xff, the first three as 0).
// The second unsigned word and the four bytes are stored in locals and never used again here.
// It then calls Find__12VIDictionaryUiR14VIResourceTypeRi(dict, id, &type, &handle); a zero result
// returns -1. Otherwise it calls CreateActor__7VISceneRC7VIVect3T1fi(scene, &first_vector,
// &second_vector, scalar, handle); a negative result returns -1; else ReadEnd__9VIObjFile(file)
// and the created actor value is returned, or -1 when ReadEnd is negative.
// Inferred: the two vectors are a position and a second three-vector (orientation or scale) for
// the actor, the scalar is a size or scale, the dictionary value found by id is the actor's sprite
// handle; the type output of Find is not examined. Unknown: the meaning of the extra word and bytes.

// Words that hold 32-bit target addresses: a host check redefines this macro.
#ifndef NORRATH_WORD_ADDRESS
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

struct ActorVect3 { float x, y, z; };

extern "C" {
int Read__9VIObjFileRUi(void* file, unsigned* value);
int Read__9VIObjFileRf(void* file, float* value);
int Read__9VIObjFileRUc(void* file, unsigned char* value);
unsigned ObjectVersion__9VIObjFile(void* file);
int ReadEnd__9VIObjFile(void* file);
int Find__12VIDictionaryUiR14VIResourceTypeRi(void* dictionary, unsigned id, int* type, int* handle);
int CreateActor__7VISceneRC7VIVect3T1fi(void* scene, const ActorVect3* first, const ActorVect3* second, float scalar, int handle);

int ParseActorObj__10VIESFParse(unsigned char* parse) {
    unsigned* words = reinterpret_cast<unsigned*>(parse);
    void* file = NORRATH_WORD_ADDRESS(words[9]);
    ActorVect3 first = {0.0f, 0.0f, 0.0f};
    ActorVect3 second = {0.0f, 0.0f, 0.0f};
    float scalar;
    unsigned id;
    unsigned extra;
    unsigned char bytes[4] = {0, 0, 0, 0xff};
    int type;
    int handle;
    Read__9VIObjFileRUi(file, &id);
    if (ObjectVersion__9VIObjFile(file) != 0) Read__9VIObjFileRUi(file, &extra);
    else extra = 0;
    Read__9VIObjFileRf(file, &first.x);
    Read__9VIObjFileRf(file, &first.y);
    Read__9VIObjFileRf(file, &first.z);
    Read__9VIObjFileRf(file, &second.x);
    Read__9VIObjFileRf(file, &second.y);
    Read__9VIObjFileRf(file, &second.z);
    Read__9VIObjFileRf(file, &scalar);
    if (ObjectVersion__9VIObjFile(file) >= 2) {
        Read__9VIObjFileRUc(file, &bytes[0]);
        Read__9VIObjFileRUc(file, &bytes[1]);
        Read__9VIObjFileRUc(file, &bytes[2]);
        Read__9VIObjFileRUc(file, &bytes[3]);
    }
    if (Find__12VIDictionaryUiR14VIResourceTypeRi(NORRATH_WORD_ADDRESS(words[8]), id, &type, &handle) == 0) return -1;
    int actor = CreateActor__7VISceneRC7VIVect3T1fi(NORRATH_WORD_ADDRESS(words[6]), &first, &second, scalar, handle);
    if (actor < 0) return -1;
    return ReadEnd__9VIObjFile(file) < 0 ? -1 : actor;
}
}
