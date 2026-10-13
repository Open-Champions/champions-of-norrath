// Host contract for cast__17SkillRegenerationiP8CreatureG6Point3s; not an EE input.
struct Point3 { float x, y, z; };
extern "C" void* cast_regeneration(void* skill, int rank, void* creature, Point3 point, short angle) asm("cast__17SkillRegenerationiP8CreatureG6Point3s");
int main() {
    char a, b;
    const int ranks[] = {-3, 0, 1, 99};
    for (int rank : ranks)
        if (cast_regeneration(&a, rank, &b, Point3{1.0f, 2.0f, 3.0f}, 5) != nullptr || cast_regeneration(nullptr, rank, nullptr, Point3{0, 0, 0}, -1) != nullptr) return 1;
    return 0;
}
