// Observed getSoulAttachMatrix__6MinionP8Matrix34 at 0x00235420, 88 bytes.
// Calls modelGetCharAttachmentMatrix with: the model header (word +12 of the resource block whose
// pointer is the word at object+36), a stack copy of the 12 bytes at object+16, the signed halfword
// at object+56, the animation state at object+288, the value 0, the caller's matrix pointer, and
// the single at object+224 as the float argument. The callee's result is not used.
// Inferred: the point is the position, the halfword a heading and the single a scale, from the
// callee's parameter types only. Unknown: why this one attachment has no index argument here.
// Nothing here ran on the EE.
extern "C" {
void modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(
    void* header, const void* point, short heading, void* animation, int index, void* matrix, float scale);
}

namespace {
struct Point3 { float x, y, z; };
}

extern "C" void getSoulAttachMatrix__6MinionP8Matrix34(void* self, void* matrix) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char* resources = *reinterpret_cast<unsigned char**>(object + 36);
    const Point3 position = *reinterpret_cast<const Point3*>(object + 16);
    modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f(
        *reinterpret_cast<void**>(resources + 12), &position, *reinterpret_cast<short*>(object + 56), object + 288, 0, matrix,
        *reinterpret_cast<float*>(object + 224));
}
