// Observed: VITrailFxBegin__Flif at 0x01027090 is 128 bytes. Arguments (l, i, f): a long, an int and a float. It calls
// RemHead__7VILList on the free trail list (image-local sFreeTrailFx at 0x011e51e8). When that returns a node, it calls
// Init__9VITrailFxlif(node, l, i, f) and then AddTail__7VILListP7VILNode(active list sActiveTrailFx at 0x011e51f8, node). It returns the
// node, or zero when the free list was empty (Init and AddTail are then not called).
// Inferred: this takes a trail effect from the free list, initialises it and makes it active.
// Unknown: the meaning of the three arguments beyond being forwarded to Init.
extern "C" {
extern unsigned norrath_trail_free[4] __asm__("VITrailFx$sFreeTrailFx");
extern unsigned norrath_trail_active[4] __asm__("VITrailFx$sActiveTrailFx");

void* RemHead__7VILList(void* list);
void Init__9VITrailFxlif(void* trail, long a, int b, float c);
void AddTail__7VILListP7VILNode(void* list, void* node);

void* VITrailFxBegin__Flif(long a, int b, float c) {
    void* trail = RemHead__7VILList(norrath_trail_free);
    if (trail) {
        Init__9VITrailFxlif(trail, a, b, c);
        AddTail__7VILListP7VILNode(norrath_trail_active, trail);
    }
    return trail;
}
}
