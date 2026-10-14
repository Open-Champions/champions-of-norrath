// Observed: Init__t8VIVector1Z13VIHSpriteNodei at 0x012270c0, 92 bytes. Arguments: the container and an
// element count. It calls Clear__t8VIVector1Z13VIHSpriteNode(this); when that result is negative it returns -1. Otherwise it
// stores the count at this+4, calls Allocate__t8VIVector1Z13VIHSpriteNode(this) and returns -1 when that result is negative, else 0.
// Inferred: the count argument becomes the container's requested capacity word before allocation.
// Unknown: the container's remaining fields, which only Clear and Allocate touch.
extern "C" {
int Clear__t8VIVector1Z13VIHSpriteNode(void* container);
int Allocate__t8VIVector1Z13VIHSpriteNode(void* container);

int Init__t8VIVector1Z13VIHSpriteNodei(void* container, int count) {
    if (Clear__t8VIVector1Z13VIHSpriteNode(container) < 0) return -1;
    *(static_cast<int*>(container) + 1) = count;
    return Allocate__t8VIVector1Z13VIHSpriteNode(container) < 0 ? -1 : 0;
}
}
