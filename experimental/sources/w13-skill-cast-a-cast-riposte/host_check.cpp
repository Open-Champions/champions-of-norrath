// Host contract for cast__12SkillRiposteiP8CreatureG6Point3s; not an EE input.
struct Point3 { float x, y, z; };
extern "C" int cast_riposte(void*, int, void*, Point3) asm("cast__12SkillRiposteiP8CreatureG6Point3s");
int main() {
    unsigned char skill[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    unsigned char creature[8] = {9, 8, 7, 6, 5, 4, 3, 2};
    const unsigned char skill_before[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    const unsigned char creature_before[8] = {9, 8, 7, 6, 5, 4, 3, 2};
    const int ranks[] = {-1, 0, 1, 99};
    for (int rank : ranks) {
        if (cast_riposte(skill, rank, creature, Point3{1.5f, -2.0f, 9.25f}) != 0) return 1;
        for (int i = 0; i < 8; ++i)
            if (skill[i] != skill_before[i] || creature[i] != creature_before[i]) return 2;
    }
    return cast_riposte(nullptr, 0, nullptr, Point3{0, 0, 0});
}
