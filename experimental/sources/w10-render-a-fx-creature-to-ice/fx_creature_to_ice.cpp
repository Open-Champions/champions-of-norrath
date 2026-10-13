// Observed fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff at
// 0x001defe0, 696 bytes. Arguments: model, animation state, a Point3 (passed by address, copied
// to a local), a short angle, an FxCreatureState pointer (never read), then floats f12, f13, f14.
// f12 is converted to an integer count of pieces; f13 is passed on to the shadow-position lookup;
// f14 is a push distance. In the order of the image:
//  - the shadow point count is read with modelGetNmShadowPoints__FP12_modelHeader, and the
//    generator word syncRandState steps once (state * 0x19660d + 0x3c6ef35f), then
//    sprintf(name, "expl_ice%d.anm", (state >> 16) % 3 + 1) names a resource that is looked up in
//    the lump "fx.lmp" and played with soundPlaySequence at the Point3 copy with volume 1.0f;
//  - the step is the shadow point count divided by the piece count (signed). The routine returns
//    when the step is zero or the piece count is not positive;
//  - otherwise it repeats the piece count times: the shadow count is read again, an index advances
//    by the step modulo that count (it starts at zero), the shadow position for that index is
//    fetched with modelGetShadowPos (out Point3 and a float height, null matrix, f13 as scale);
//    syncRandState steps once more and r = (state >> 16) % 10 picks a kind: 2 when r is zero, 3 when
//    r is 1 or 2, otherwise 4; when f14 is not zero the position is moved by icos(angle) * f14 in x and
//    isin(angle) * f14 in y, and 0.0f is added to z; then 584 bytes come from blockAlloc__Fi, and if
//    that succeeded the PhProp constructor runs on them with the position, "fx.lmp", "icechunk.vif",
//    "icechunk.tex", the kind, 778, a null pointer and 1.0f, and the vtable _vt$13LooseIceChunk is
//    stored at object+72.
// Nothing is returned.
// Inferred: pieces of broken ice are thrown from the creature's shadow points. Unknown: what the
// unread state pointer and the short angle mean beyond their use here, what the kinds 2, 3 and 4
// select, and the divide-by-zero outcome when the shadow count is zero inside the loop (the image
// has no trap there, this source's divide does).
struct Point3 { float x, y, z; };
extern "C" {
extern unsigned syncRandState;
extern unsigned char _vt$13LooseIceChunk[];
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* state, Point3 point, short value, int index, Point3* position, float* height,
    void* matrix, float scale);
int sprintf(char* buffer, const char* format, ...);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderG6Point3f(void* sequence, Point3 position, float volume);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int bytes);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* first, char* second,
                                                    char* third, long kind, int flags, void* description,
                                                    float scale);

void fxCreatureToIce__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefff(
    void* model, void* state, Point3 point, short angle, void* creature, float count, float scale,
    float push) {
    (void)creature;
    const int pieces = static_cast<int>(count);
    const Point3 origin = point;
    const int points = modelGetNmShadowPoints__FP12_modelHeader(model);
    syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
    char name[64];
    sprintf(name, "expl_ice%d.anm", static_cast<int>((syncRandState >> 16) % 3u) + 1);
    soundPlaySequence__FP15AnimationHeaderG6Point3f(
        lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name), origin, 1.0f);
    if (pieces <= 0) return;
    const int step = points / pieces;
    if (step == 0) return;
    int cursor = 0;
    for (int left = pieces; left > 0; --left) {
        const int total = modelGetNmShadowPoints__FP12_modelHeader(model);
        long kind = 2;
        cursor = (cursor + step) % total;
        Point3 position;
        float height;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, state, origin, angle, cursor, &position, &height, nullptr, scale);
        syncRandState = syncRandState * 0x19660du + 0x3c6ef35fu;
        const unsigned roll = ((syncRandState >> 16) % 10u) & 0xffffu;
        if (roll != 0) kind = roll < 3 ? 3 : 4;
        if (push != 0.0f) {
            const float dx = icos__Fi(angle) * push;
            const float dy = isin__Fi(angle) * push;
            const float dz = 0.0f;
            position.z = position.z + dz;
            position.x = position.x + dx;
            position.y = position.y + dy;
        }
        void* chunk = blockAlloc__Fi(584);
        if (chunk != nullptr) {
            __6PhPropG6Point3PcN22liPQ24item11descriptionf(
                chunk, position, const_cast<char*>("fx.lmp"), const_cast<char*>("icechunk.vif"),
                const_cast<char*>("icechunk.tex"), kind, 778, nullptr, 1.0f);
            *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(chunk) + 72) =
                static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(_vt$13LooseIceChunk));
        }
    }
}
}
