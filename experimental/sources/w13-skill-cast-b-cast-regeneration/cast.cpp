// Observed cast__17SkillRegenerationiP8CreatureG6Point3s at 0x002b6ba8, 40 bytes in .text.
// Copies the by-value Point3 to its frame without reading it and returns zero. It reads no other argument and calls nothing.
// Inferred: this skill starts no spell block of its own. Unknown: whether another path applies the regeneration.
struct Point3 { float x, y, z; };
extern "C" {
void* cast_regeneration(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__17SkillRegenerationiP8CreatureG6Point3s");
}
extern "C" void* cast_regeneration(void*, int, void*, Point3, short) { return nullptr; }
