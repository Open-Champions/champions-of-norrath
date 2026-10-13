// Observed fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff at 0x001e5f98,
// 240 bytes. Arguments: model (a0), animation state (a1), a Point3 passed by address (a2),
// a short (a3), then two floats f12 and f13. The Point3 is copied to a local first. The
// routine asks modelGetNmShadowPoints__FP12_modelHeader(model) for a count and, for each
// index from 0 below that count, calls
// modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f with
// the model, state, the local Point3 copy, the short, the index, a local Point3 output, a
// local float output, a null Matrix33 pointer and the first float. After each position it
// calls addEffect__FiG6Point3(2, that output position). With a count of zero or less
// nothing is done. Nothing is returned. The second float is not read.
// Inferred: each shadow point of a creature spawns an effect of kind 2 (dust) when it is
// turned to dust. Unknown: what the second float is for, what the short means, and the
// meaning of the effect kind beyond its numeric value.
extern "C" {
int modelGetNmShadowPoints__FP12_modelHeader(unsigned char* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    unsigned char* model, unsigned char* state, const unsigned char* point, short value, int index,
    float* position, float* height, unsigned char* matrix, float scale);
void addEffect__FiG6Point3(int kind, const float* position);
void fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff(
    unsigned char* model, unsigned char* state, const unsigned char* point, short value, float scale,
    float unused) {
    (void)unused;
    unsigned char copy[12];
    for (int i = 0; i != 12; ++i) copy[i] = point[i];
    const int count = modelGetNmShadowPoints__FP12_modelHeader(model);
    float position[3];
    float height;
    for (int i = 0; i < count; ++i) {
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, state, copy, value, i, position, &height, nullptr, scale);
        addEffect__FiG6Point3(2, position);
    }
}
}
