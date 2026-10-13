// Observed cast__12SkillLifeTapiP8CreatureG6Point3s at 0x002be218, 40 bytes in .text.
// It copies the by-value Point3 argument to its own stack frame, never reads the copy, and returns zero. Neither
// the skill, the rank nor the creature is read or written. The compiled body is shorter (the copy is dropped).
// Inferred only from that body: the life-tap cast hook does nothing by itself. Unknown: why the class overrides
// the base hook, and where the life-tap effect is applied.
struct Point3 { float x, y, z; };
extern "C" int cast_life_tap(void*, int, void*, Point3) asm("cast__12SkillLifeTapiP8CreatureG6Point3s");
extern "C" int cast_life_tap(void*, int, void*, Point3) {
    return 0;
}
