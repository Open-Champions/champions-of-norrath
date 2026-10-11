// Host contract for animSetSpecialSoundHandler. Not an EE plan input.
#include "special_handler.cpp"

extern "C" void (*specialHandler)(int code, const unsigned char* point) = nullptr;

static void sample(int, const unsigned char*) {}

int main() {
    specialHandler = sample;
    animSetSpecialSoundHandler__FPFiG6Point3_v(nullptr);
    if (specialHandler != nullptr) return 1;
    animSetSpecialSoundHandler__FPFiG6Point3_v(sample);
    if (specialHandler != sample) return 2;
    return 0;
}
