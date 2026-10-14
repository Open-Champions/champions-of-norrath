// Observed: QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery at 0x01143618 is 236 bytes in the pinned SLUS-20565 image. The scene is this,
// the second argument is used as the address of an actor record (the mangled name says int), then a camera and a query record.
// Using the scene's word at +1348 as a frame stamp, the routine:
//   1. returns 0 if the actor's word at +128 already equals the stamp;
//   2. returns 0 if the actor's word at +64 has either bit 0x24 set;
//   3. stores the stamp in the actor's +128 and reads the actor's word at +96 (call it index);
//   4. calls PointInSolid__C8VISphereRC7VIVect3(actor + 136, camera + 28); if that returns zero it returns 0;
//   5. stores the actor address in the query's +8 and the index in the query's +12;
//   6. calls Mul__10VIMatrix44RC10VIMatrix44T1(scene + 1232, actor, camera + 48) (scene+1232 is the output matrix);
//   7. looks up object = word at (scene's pointer at +1004) + 12*index, takes the table pointer at object+60, and calls the method
//      whose entry (a 2-byte this-adjustment at +0 and a function address at +4) is at table+56, with arguments
//      (object + adjustment, scene, scene + 1232, query); a negative result returns -1 and otherwise it returns 0.
// Inferred: this asks one actor's emitter object (reached through a per-actor index into a scene table of 12-byte records) to report
// emitters if the camera position lies inside the actor's bounding sphere; the stamp makes it run once per frame per actor; the
// matrix product is the actor's matrix with the camera's matrix at +48.
// Unknown: what the bit mask 0x24 means, which virtual method sits at table+56, the sphere at actor+136, and the query's layout beyond
// +8 and +12. Nothing here ran on the EE.
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
int PointInSolid__C8VISphereRC7VIVect3(const unsigned char* sphere, const unsigned char* point);
void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* out, const unsigned char* a, const unsigned char* b);

typedef int (*EmitterMethod)(unsigned char* self, unsigned char* scene, unsigned char* matrix, unsigned char* query);

int QueryEmittersAtLocationActor__7VISceneiRC8VICameraR14VIEmitterQuery(unsigned char* scene, int actorAddress,
                                                                        const unsigned char* camera, unsigned char* query) {
    unsigned char* actor = NORRATH_WORD_TO_POINTER(static_cast<unsigned>(actorAddress));
    unsigned stamp = *reinterpret_cast<unsigned*>(scene + 1348);
    if (*reinterpret_cast<unsigned*>(actor + 128) == stamp) return 0;
    if ((*reinterpret_cast<unsigned*>(actor + 64) & 0x24) != 0) return 0;
    *reinterpret_cast<unsigned*>(actor + 128) = stamp;
    unsigned index = *reinterpret_cast<unsigned*>(actor + 96);
    if (PointInSolid__C8VISphereRC7VIVect3(actor + 136, camera + 28) == 0) return 0;
    *reinterpret_cast<unsigned*>(query + 8) = static_cast<unsigned>(actorAddress);
    unsigned char* matrix = scene + 1232;
    *reinterpret_cast<unsigned*>(query + 12) = index;
    Mul__10VIMatrix44RC10VIMatrix44T1(matrix, actor, camera + 48);
    unsigned char* records = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(scene + 1004));
    unsigned char* object = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(records + index * 12));
    unsigned char* entry = NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(object + 60)) + 56;
    EmitterMethod method = reinterpret_cast<EmitterMethod>(NORRATH_WORD_TO_FUNCTION(*reinterpret_cast<unsigned*>(entry + 4)));
    if (method(object + *reinterpret_cast<short*>(entry), scene, matrix, query) < 0) return -1;
    return 0;
}
}
