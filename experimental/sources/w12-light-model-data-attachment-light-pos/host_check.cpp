// Host contract for the attachmentLightPos object. Not an EE plan input.
#include <cstdio>
#include "attachment_light_pos.cpp"
int main() {
    static_assert(sizeof(attachmentLightPos) == 12u, "Point3 extent");
    if (attachmentLightPos.x != 0.0f || attachmentLightPos.y != 0.0f || attachmentLightPos.z != 0.0f) {
        std::fprintf(stderr, "fail: not zero\n");
        return 1;
    }
    std::printf("attachmentLightPos contract passed\n");
    return 0;
}
