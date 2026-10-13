// Observed: Init__19VIRadialFloraSystemR7VIScene at 0x010d5ba0 is 88 bytes. It calls
// Clear__19VIRadialFloraSystem(this). If that is negative it returns -1 and nothing else is
// stored. Otherwise it stores the scene pointer at offset 0 of the system, the scene's word at
// offset 19356 at offset 4 and the scene's word at offset 19364 at offset 8, and returns zero.
// Inferred: the two scene words are values the flora code needs (their meaning is not known).
// Unknown: what the scene words at 19356 and 19364 are.
extern "C" {
struct VIRadialFloraSystem;
struct VIScene;

int Clear__19VIRadialFloraSystem(VIRadialFloraSystem* self);

int Init__19VIRadialFloraSystemR7VIScene(VIRadialFloraSystem* self, VIScene& scene) {
    if (Clear__19VIRadialFloraSystem(self) < 0) return -1;
    const char* from = reinterpret_cast<const char*>(&scene);
    unsigned* words = reinterpret_cast<unsigned*>(self);
    unsigned second = *reinterpret_cast<const unsigned*>(from + 19364);
    unsigned first = *reinterpret_cast<const unsigned*>(from + 19356);
    words[2] = second;
    words[1] = first;
    words[0] = reinterpret_cast<unsigned>(&scene);
    return 0;
}
}
