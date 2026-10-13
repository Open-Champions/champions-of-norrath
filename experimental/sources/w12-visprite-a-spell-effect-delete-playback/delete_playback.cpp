// Observed: DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi
// at 0x01003080, 64 bytes. Only the first two argument registers are used.
// With the object as this and q the second argument: Remove__7VILNode(q), then
// AddTail__7VILListP7VILNode(this + 112, q), in that order. Nothing is returned.
// Inferred: this + 112 is the list of unused playback records and the record
// starts with the list-node links. Unknown: the further parameters the
// mangled name suggests; this body ignores them.
extern "C" {
void Remove__7VILNode(void* node);
void AddTail__7VILListP7VILNode(void* list, void* node);

void DeletePlayback__19VISpellEffectSystemPQ219VISpellEffectSystem16VISpellEffectPbi(void* self, char* playback) {
    Remove__7VILNode(playback);
    AddTail__7VILListP7VILNode(static_cast<char*>(self) + 112, playback);
}
}
