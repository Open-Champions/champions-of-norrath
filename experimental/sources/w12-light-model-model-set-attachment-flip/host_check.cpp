// Host contract for modelSetAttachmentFlip__Fv. Not an EE plan input.
#include <cstdio>
#include <initializer_list>
extern "C" {
unsigned modelOption;
void modelSetAttachmentFlip__Fv();
}
int main() {
    int failures = 0;
    for (unsigned v : {0u, 0x80u, 0x7fu, 0xffffffffu, 0x12345678u}) {
        modelOption = v;
        modelSetAttachmentFlip__Fv();
        if (modelOption != (v | 0x80u)) { std::fprintf(stderr, "fail: %08x got %08x\n", v, modelOption); ++failures; }
    }
    if (failures) return 1;
    std::printf("modelSetAttachmentFlip contract passed\n");
    return 0;
}
