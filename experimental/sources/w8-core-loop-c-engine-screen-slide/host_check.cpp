// Host contract for engineScreenSlide. Not an EE plan input.
#include "engine_screen_slide.cpp"

int main() {
    if (engineScreenSlide != 0) return 1;
    if (sizeof engineScreenSlide != 4u) return 2;
    engineScreenSlide = engineScreenSlide + 1;
    if (engineScreenSlide != 0 + 1) return 3;
    return 0;
}
