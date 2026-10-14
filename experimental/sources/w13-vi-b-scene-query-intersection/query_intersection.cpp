// Observed: QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList at
// 0x011437f8, 328 bytes. It builds a 156-byte argument block on its stack and calls
// QueryIntersection__7VIWorldR20VISceneIntersectVars(scene + 4, block). Block layout (byte
// offsets): +0 integer 0; +4 the box's first three floats; +16 the box's floats 3-5; +28 the
// scene pointer; +32 the filter pointer; +36 the scene word at +19364; +40 the triangle-list
// pointer; +44 eight three-float vectors (96 bytes) filled by Store__C6VIBBoxP7VIVect3(box,
// +44); +140 four floats initialized by Init__8VISphereRC6VIBBox(+140, box). Everything not
// listed is zeroed first. If the world call returns a negative value the function returns -1;
// otherwise it increments the scene word at +1348 and returns 0.
// Inferred: the +4/+16 pair is a copy of the box's minimum and maximum, the eight vectors its
// corners, and +140 its bounding sphere (center and radius).
// Unknown: the member names, the box layout beyond its first six floats, and the meaning of
// the first zero word.
struct VISceneIntersectVars {
    int zero;
    float minimum[3];
    float maximum[3];
    void* scene;
    void* filter;
    int sceneWord;
    void* triList;
    float corners[8][3];
    float sphere[4];
};

extern "C" {
void Store__C6VIBBoxP7VIVect3(const void* box, float* corners);
void Init__8VISphereRC6VIBBox(float* sphere, const void* box);
int QueryIntersection__7VIWorldR20VISceneIntersectVars(void* world, VISceneIntersectVars* vars);

int QueryIntersection__7VISceneR6VIBBoxR13VIActorFilterR14VIQueryTriList(void* scene, void* box, void* filter, void* triList) {
    unsigned char* s = static_cast<unsigned char*>(scene);
    const float* b = static_cast<const float*>(box);
    VISceneIntersectVars vars;
    for (int i = 0; i < 3; ++i) { vars.minimum[i] = 0.0f; vars.maximum[i] = 0.0f; }
    for (int i = 0; i < 8; ++i) { vars.corners[i][0] = 0.0f; vars.corners[i][1] = 0.0f; vars.corners[i][2] = 0.0f; }
    vars.sphere[0] = 0.0f;
    vars.sphere[1] = 0.0f;
    vars.sphere[2] = 0.0f;
    vars.sphere[3] = 0.0f;
    vars.minimum[0] = b[0];
    vars.minimum[1] = b[1];
    vars.minimum[2] = b[2];
    vars.maximum[0] = b[3];
    vars.maximum[1] = b[4];
    vars.maximum[2] = b[5];
    vars.filter = filter;
    vars.sceneWord = *reinterpret_cast<int*>(s + 19364);
    vars.triList = triList;
    vars.scene = scene;
    vars.zero = 0;
    Store__C6VIBBoxP7VIVect3(box, &vars.corners[0][0]);
    Init__8VISphereRC6VIBBox(vars.sphere, box);
    if (QueryIntersection__7VIWorldR20VISceneIntersectVars(s + 4, &vars) < 0) return -1;
    *reinterpret_cast<int*>(s + 1348) += 1;
    return 0;
}
}
