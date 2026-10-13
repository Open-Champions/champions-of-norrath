// Observed animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps at 0x001639c8, 28
// bytes. Forwards its four arguments unchanged to animAdvanceAnimationTime with a time
// step of 1.0 as the float argument, and returns what that call leaves behind (the
// original ends with a jump, so the callee's return is the caller's).
// Inferred: the step is one frame; animAdvanceAnimationTime is described by its own
// importers and is not part of this unit.
// Unknown: the pointee types behind the Point3, Point2 and short pointers.
extern "C" {
void animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(
    void* state, float step, void* point3, void* point2, short* last);
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(
    void* state, void* point3, void* point2, short* last);
}

extern "C" void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(
    void* state, void* point3, void* point2, short* last) {
    animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(state, 1.0f, point3, point2, last);
}
