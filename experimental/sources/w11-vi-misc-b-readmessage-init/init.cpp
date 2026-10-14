// Observed: Init__16VIWndReadMessage at 0x010d14e0 is 32 bytes. With the object as this it calls
// Clear__16VIWndReadMessage(this) and returns 0.
// Unknown: what Clear does; its body is not part of this unit.
extern "C" {
void Clear__16VIWndReadMessage(void* self);

int Init__16VIWndReadMessage(void* self) {
    Clear__16VIWndReadMessage(self);
    return 0;
}
}
