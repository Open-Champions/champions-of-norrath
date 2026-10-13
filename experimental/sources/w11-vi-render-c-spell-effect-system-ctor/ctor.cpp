// Observed __19VISpellEffectSystem at 0x01002a78, 124 bytes. With the object as this:
//   __7VILList(this + 100); __7VILList(this + 112);
//   __7VILNode(this + 128 + 40 * i) for i = 0 .. 23, in that order;
//   __t6VIPool1ZP13VISpellEffect(this + 1112); then 0 is stored at this + 1092.
// Nothing is returned. The twenty-four 40-byte elements from this + 128 are what Init later
// links into the list at this + 112; their type is not established here.
extern "C" {
void __7VILList(void* list);
void __7VILNode(void* node);
void __t6VIPool1ZP13VISpellEffect(void* pool);

void __19VISpellEffectSystem(void* self) {
    char* base = static_cast<char*>(self);
    __7VILList(base + 100);
    __7VILList(base + 112);
    for (int i = 0; i < 24; ++i) __7VILNode(base + 128 + 40 * i);
    __t6VIPool1ZP13VISpellEffect(base + 1112);
    *reinterpret_cast<int*>(base + 1092) = 0;
}
}
