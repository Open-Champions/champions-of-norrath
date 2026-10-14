// Observed: UpdateActor__7VIScenei at 0x01143ad8 is 436 bytes. Arguments: the scene and
// an actor word. Offsets are bytes; "word" means a 32-bit address or value.
//   1. Calls CalcObjectToWorld__7VIActor(actor).
//   2. info = first word of the 12-byte entry at (scene word +1004) + 12 * (actor word +96).
//   3. Calls Mul__C10VIMatrix44RC7VIVect3R7VIVect3(actor, info + 32, actor + 136),
//      then stores the float (info +44) * (actor float +92) at actor +148.
//   4. Calls RemoveActor__7VIScenei(scene, actor); a negative result returns -1.
//   5. Fills a 128-byte insert record (zeroed first: 8 x 3 floats of points then four
//      floats): word 0 = 0, word 1 = actor, word 2 = -1, word 3 = scene. Re-reads info
//      the same way and calls Transform__C6VIBBoxRC10VIMatrix44P7VIVect3(info + 8, actor,
//      record points). Then copies the four floats at actor +136..+148 into the record's
//      last four floats.
//   6. Calls InsertActor__7VIWorldR17VISceneInsertVars(scene + 4, record). A negative
//      result returns -1; otherwise the word at scene +1348 is incremented and it
//      returns 0.
// Inferred: re-inserts an actor into the scene's spatial structure after recomputing its
// world position, with a bounding sphere (centre +136, radius +148 = scale * base radius)
// and eight transformed bounding-box corners. Counter +1348 counts insertions.
// The original also zeroes a second 8-vector stack array that is never read afterwards;
// it has no observable effect and is not reproduced.
// Unknown: record field names, and the info layout beyond +8 (box), +32 (vector) and
// +44 (radius).
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
static inline unsigned char* at(unsigned address) { return norrath_host_pointer(address); }
#else
static inline unsigned char* at(unsigned address) { return reinterpret_cast<unsigned char*>(static_cast<unsigned long>(address)); }
#endif

namespace {
struct InsertRecord {
    int mode;
    unsigned actor;
    int slot;
    unsigned scene;
    float points[8][3];
    float sphere[4];
};
}

extern "C" {
void CalcObjectToWorld__7VIActor(void* actor);
void Mul__C10VIMatrix44RC7VIVect3R7VIVect3(void* matrix, const void* input, void* output);
int RemoveActor__7VIScenei(unsigned char* scene, int actor);
void Transform__C6VIBBoxRC10VIMatrix44P7VIVect3(const void* box, const void* matrix, void* points);
int InsertActor__7VIWorldR17VISceneInsertVars(void* world, InsertRecord* record);

int UpdateActor__7VIScenei(unsigned char* scene, int actor_word) {
    unsigned char* actor = at(static_cast<unsigned>(actor_word));
    InsertRecord record;
    for (int i = 0; i < 8; ++i) {
        record.points[i][0] = 0.0f;
        record.points[i][1] = 0.0f;
        record.points[i][2] = 0.0f;
    }
    record.sphere[3] = 0.0f;
    record.sphere[0] = 0.0f;
    record.sphere[1] = 0.0f;
    record.sphere[2] = 0.0f;
    CalcObjectToWorld__7VIActor(actor);
    int type = *reinterpret_cast<int*>(actor + 96);
    unsigned char* info = at(*reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(scene + 1004)) + type * 12));
    Mul__C10VIMatrix44RC7VIVect3R7VIVect3(actor, info + 32, actor + 136);
    *reinterpret_cast<float*>(actor + 148) = *reinterpret_cast<float*>(info + 44) * *reinterpret_cast<float*>(actor + 92);
    if (RemoveActor__7VIScenei(scene, actor_word) < 0)
        return -1;
    info = at(*reinterpret_cast<unsigned*>(at(*reinterpret_cast<unsigned*>(scene + 1004)) + type * 12));
    record.slot = -1;
    record.mode = 0;
    record.actor = static_cast<unsigned>(actor_word);
    record.scene = static_cast<unsigned>(reinterpret_cast<unsigned long>(scene));
    Transform__C6VIBBoxRC10VIMatrix44P7VIVect3(info + 8, actor, record.points);
    record.sphere[0] = *reinterpret_cast<float*>(actor + 136);
    record.sphere[1] = *reinterpret_cast<float*>(actor + 140);
    record.sphere[2] = *reinterpret_cast<float*>(actor + 144);
    record.sphere[3] = *reinterpret_cast<float*>(actor + 148);
    if (InsertActor__7VIWorldR17VISceneInsertVars(scene + 4, &record) < 0)
        return -1;
    *reinterpret_cast<int*>(scene + 1348) += 1;
    return 0;
}
}
