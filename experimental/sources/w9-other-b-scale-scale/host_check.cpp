// Host check for scale_scale.cpp; not an EE plan input.
#include "scale_scale.cpp"

int main() {
    if (scaleScale != 1.0f) return 1;
    scaleScale = 2.5f;
    if (scaleScale != 2.5f) return 2;
    return 0;
}
