// Host contract for animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps. Not an EE plan input.
// Build: c++ -std=c++17 host_check.cpp
#include <cstdio>
static int calls;
static void* got[4];
static float got_step;
extern "C" void animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps(
    void* state, float step, void* point3, void* point2, short* last) {
    ++calls; got[0] = state; got[1] = point3; got[2] = point2; got[3] = last; got_step = step;
}
#include "anim_advance_animation.cpp"
int main() {
    int a, b, c; short d;
    animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(&a, &b, &c, &d);
    if (calls != 1) { std::printf("calls %d\n", calls); return 1; }
    if (got[0] != &a || got[1] != &b || got[2] != &c || got[3] != &d) { std::printf("argument order\n"); return 2; }
    if (got_step != 1.0f) { std::printf("step %f\n", got_step); return 3; }
    animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(nullptr, nullptr, nullptr, nullptr);
    if (calls != 2 || got[0] || got[1] || got[2] || got[3]) { std::printf("null pass-through\n"); return 4; }
    std::printf("animAdvanceAnimation check passed\n");
    return 0;
}
