// Observed draw__5Spell at 0x00314628, extent 8, a weak symbol: returns at once
// without touching the object. The mangled name is the member Spell::draw().
// Inferred: this is the empty base-class implementation that spell subclasses
// override through their vtables. Unknown: whether the shipped build emitted it
// as an inline out-of-line copy or a defined empty method.
extern "C" void draw__5Spell(void* self) {
    (void)self;
}
