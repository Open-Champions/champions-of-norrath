// Observed msg_trigger__11MissileTrapi at 0x1e5c78, 44 bytes. A nonzero
// argument adds the object to the run list (objectAddToRunList__FP10GameObject,
// a tail call); zero removes it (objectRemoveFromRunList__FP10GameObject).
// Nothing is returned.
// Inferred: the integer is an on/off switch. Unknown: what the run list drives
// for this class.
extern "C" {
void objectAddToRunList__FP10GameObject(void* object);
void objectRemoveFromRunList__FP10GameObject(void* object);
}

extern "C" void msg_trigger__11MissileTrapi(void* self, int on) {
    if (on != 0) {
        objectAddToRunList__FP10GameObject(self);
        return;
    }
    objectRemoveFromRunList__FP10GameObject(self);
}
