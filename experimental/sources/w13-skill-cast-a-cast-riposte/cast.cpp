// Observed cast__12SkillRiposteiP8CreatureG6Point3s at 0x002a50e0, 40 bytes in .text.
// It copies the by-value Point3 argument to its own stack frame, never reads the copy, and returns zero. Neither
// the skill, the rank nor the creature is read or written. The compiled body is shorter (the copy is dropped).
// Inferred only from that body: the riposte cast hook does nothing by itself. Unknown: why the class overrides
// the base hook, and where the riposte effect is applied.
struct Point3 { float x, y, z; };
extern "C" int cast_riposte(void*, int, void*, Point3) asm("cast__12SkillRiposteiP8CreatureG6Point3s");
extern "C" int cast_riposte(void*, int, void*, Point3) {
    return 0;
}
