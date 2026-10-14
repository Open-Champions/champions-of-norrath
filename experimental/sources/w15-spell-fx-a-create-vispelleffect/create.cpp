// Observed: Create__13VISpellEffectP7VIScene at 0x01000828 is 12 bytes in the pinned SLUS-20565 image. It stores its second
// argument (the scene) in the word at +8 of the object and returns zero.
// Inferred: a creation step that records the owning scene and cannot fail. Unknown: what else a caller expects of it. Nothing here
// ran on the EE.
extern "C" int Create__13VISpellEffectP7VIScene(void* self, void* scene) {
    *reinterpret_cast<void**>(static_cast<unsigned char*>(self) + 8) = scene;
    return 0;
}
