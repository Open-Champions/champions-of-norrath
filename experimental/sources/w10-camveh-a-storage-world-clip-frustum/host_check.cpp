// Host contract for worldClipFrustum storage. Not an EE plan input.
#include "storage.cpp"

int main() {
    if (sizeof(worldClipFrustum) != 96) return 1;
    for (float f : worldClipFrustum) if (f != 0.0f) return 2;
    return 0;
}
