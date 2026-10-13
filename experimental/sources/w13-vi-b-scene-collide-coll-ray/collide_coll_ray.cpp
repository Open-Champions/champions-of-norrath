// Observed: Collide__7VISceneR9VICollRayR13VISceneFilter at 0x0113dfd0, 172 bytes. It builds
// a 44-byte argument block on its stack: word 0 is zero, word 1 the scene pointer, word 2 the
// filter pointer, word 3 the ray pointer, word 4 the scene word at +19364, words 5-7 the
// first three floats of the ray, words 8-10 the floats at ray + 12. It calls
// Collide__7VIWorldR18VISceneCollRayVars(scene + 4, block), increments the scene word at
// +1348 and returns the world call's result unchanged.
// Inferred: scene + 4 is an embedded VIWorld; the block is the VISceneCollRayVars named in
// the callee, whose two vectors hold the ray's start and end (or start and direction).
// Unknown: the member names, the ray's layout beyond 24 bytes, the meaning of word 0 and the
// true return type (an int is assumed).
struct VISceneCollRayVars {
    int zero;
    void* scene;
    void* filter;
    void* ray;
    int sceneWord;
    float first[3];
    float second[3];
};

extern "C" {
int Collide__7VIWorldR18VISceneCollRayVars(void* world, VISceneCollRayVars* vars);

int Collide__7VISceneR9VICollRayR13VISceneFilter(void* scene, void* ray, void* filter) {
    unsigned char* s = static_cast<unsigned char*>(scene);
    const float* r = static_cast<const float*>(ray);
    VISceneCollRayVars vars;
    vars.zero = 0;
    vars.scene = scene;
    vars.filter = filter;
    vars.ray = ray;
    vars.sceneWord = *reinterpret_cast<int*>(s + 19364);
    vars.first[0] = r[0];
    vars.first[1] = r[1];
    vars.first[2] = r[2];
    vars.second[0] = r[3];
    vars.second[1] = r[4];
    vars.second[2] = r[5];
    int result = Collide__7VIWorldR18VISceneCollRayVars(s + 4, &vars);
    *reinterpret_cast<int*>(s + 1348) += 1;
    return result;
}
}
