// Observed: VITrailFxEnd__FP9VITrailFx at 0x01027110, 52 bytes. With the trail effect node as its
// argument it calls Remove__7VILNode(fx) and then AddTail__7VILListP7VILNode(list, fx) where list
// is the static at 0x011e5208 (the image's local sInactiveTrailFx, published by earlier work as
// `VITrailFx$sInactiveTrailFx`). There is no return value.
// Inferred: ending an effect moves its node from the active list to the inactive list.
// Unknown: the node and list layouts here, and whether the argument is a list node at offset 0.
extern "C" {
extern unsigned norrath_trail_inactive[4] __asm__("VITrailFx$sInactiveTrailFx");
void Remove__7VILNode(void* node);
void AddTail__7VILListP7VILNode(void* list, void* node);

void VITrailFxEnd__FP9VITrailFx(void* fx) {
    Remove__7VILNode(fx);
    AddTail__7VILListP7VILNode(norrath_trail_inactive, fx);
}
}
