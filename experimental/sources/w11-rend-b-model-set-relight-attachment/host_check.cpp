// Host contract for modelSetRelightAttachment__FG6Point3. Not an EE plan input.
#include <cstdio>
#include <initializer_list>
#include <cstring>
#include "../w9-render-math-b-common/math_types.hpp"
extern "C" {
int modelOption;
Point3 attachmentLightPos;
void modelSetRelightAttachment__FG6Point3(Point3);
}
int main() {
    int failures = 0;
    for (int start : {0, 1, 0x40, -1, 0x3f}) {
        modelOption = start;
        attachmentLightPos = Point3{9, 9, 9};
        modelSetRelightAttachment__FG6Point3(Point3{1.5f, -2.0f, 3.25f});
        if (modelOption != (start | 0x40)) { std::fprintf(stderr, "fail: option %d\n", start); ++failures; }
        if (attachmentLightPos.x != 1.5f || attachmentLightPos.y != -2.0f || attachmentLightPos.z != 3.25f) {
            std::fprintf(stderr, "fail: position\n"); ++failures;
        }
    }
    if (failures) return 1;
    std::printf("modelSetRelightAttachment contract passed\n");
    return 0;
}
