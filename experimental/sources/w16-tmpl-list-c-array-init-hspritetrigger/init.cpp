// Observed: Init__t7VIArray1Z16VIHSpriteTriggeri at 0x01227180, 92 bytes. Arguments: the container and an
// element count. It calls Clear__t7VIArray1Z16VIHSpriteTrigger(this); when that result is negative it returns -1. Otherwise it
// stores the count at this+0, calls Allocate__t7VIArray1Z16VIHSpriteTrigger(this) and returns -1 when that result is negative, else 0.
// Inferred: the count argument becomes the container's requested capacity word before allocation.
// Unknown: the container's remaining fields, which only Clear and Allocate touch.
extern "C" {
int Clear__t7VIArray1Z16VIHSpriteTrigger(void* container);
int Allocate__t7VIArray1Z16VIHSpriteTrigger(void* container);

int Init__t7VIArray1Z16VIHSpriteTriggeri(void* container, int count) {
    if (Clear__t7VIArray1Z16VIHSpriteTrigger(container) < 0) return -1;
    *(static_cast<int*>(container) + 0) = count;
    return Allocate__t7VIArray1Z16VIHSpriteTrigger(container) < 0 ? -1 : 0;
}
}
