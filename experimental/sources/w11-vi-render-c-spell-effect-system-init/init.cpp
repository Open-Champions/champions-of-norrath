// Observed Init__19VISpellEffectSystemP7VIScene at 0x01002af8, 196 bytes. With the object as this:
//   1. The scene argument is stored at this + 0 (the store sits in the delay slot of the first
//      call, so it is complete before any callee runs).
//   2. Init__7VILList(this + 100) and Init__7VILList(this + 112).
//   3. Words at this + 4, 8, ..., 96 (twenty-four of them) are set to zero.
//   4. For i = 0 .. 23, AddTail__7VILListP7VILNode(this + 112, this + 128 + 40 * i).
//   5. Stores: 1 at 1092, 0x00016062 at 1088, -1 at 1108, 0 at 1096, 0 at 1100, 0 at 1104.
// The result is zero. Unknown: what the stored constants mean.
extern "C" {
void Init__7VILList(void* list);
void AddTail__7VILListP7VILNode(void* list, void* node);

int Init__19VISpellEffectSystemP7VIScene(void* self, void* scene) {
    char* base = static_cast<char*>(self);
    *reinterpret_cast<void**>(base) = scene;
    Init__7VILList(base + 100);
    Init__7VILList(base + 112);
    for (int i = 23; i >= 0; --i) *reinterpret_cast<int*>(base + 4 + 4 * i) = 0;
    for (int i = 0; i < 24; ++i) AddTail__7VILListP7VILNode(base + 112, base + 128 + 40 * i);
    *reinterpret_cast<int*>(base + 1092) = 1;
    *reinterpret_cast<int*>(base + 1088) = 0x16062;
    *reinterpret_cast<int*>(base + 1108) = -1;
    *reinterpret_cast<int*>(base + 1096) = 0;
    *reinterpret_cast<int*>(base + 1100) = 0;
    *reinterpret_cast<int*>(base + 1104) = 0;
    return 0;
}
}
