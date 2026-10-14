// Observed GetInstType__C8VISpriteR7VIScene at 0x1008120, 8 bytes: it returns the constant 0 and touches nothing
// else (the two arguments, the object and the scene, are unused). Inferred: the
// value is the sprite kind the scene traversal compares against, e.g. the
// group sprite's loop below tests its members' answers against 1. Unknown: what
// the number means by name.
extern "C" int GetInstType__C8VISpriteR7VIScene(const void* self, void* scene) {
    (void)self;
    (void)scene;
    return 0;
}
