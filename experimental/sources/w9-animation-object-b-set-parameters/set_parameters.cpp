// Observed setParameters__8Creaturefffffi at 0x1092f0, extent 600.
// Derives a creature's combat numbers from five scale floats and a flag, then
// returns the result of a tag query. Arguments after the creature: first, second,
// third, fourth, fifth (floats) and `randomize` (int). Words used on the creature:
// +160 a 64-bit flag word, +200/+202/+204/+232 signed halfwords, +220 a float,
// +560 and +564 floats. Constants: the floats at 0x47fb58..0x47fb68 are, in order,
// 1.3, 0.35, 0.8, 1.1, 1.32 (exact bit patterns written as hex floats below).
// In order:
//   1. If bit 0 of the +160 word is set: first *= 15, third *= 1.3, fourth *= 3,
//      fifth *= 7.
//   2. If nmPlayers >= 2: when netNetworkMode (signed byte) is nonzero, call
//      netPlayerIsConnected for every index below nmPlayers (results unused; the
//      count is re-read each pass). Then extra = (float)(nmPlayers - 1);
//      fifth *= extra + 1; first *= extra * 0.35 + 1.
//   3. If randomize is nonzero: draw r = eRand(creature); base = pow(first *
//      worldDifficulty, 0.8); health = (float)(r % 6 + 18) * base; the float at +560
//      is health, replaced by health * 1.5 when gameDifficulty is 2, or by
//      health + health when it is 3; +564 receives a copy of +560.
//   4. +232 = (short)(int)(second * 12 * worldDifficulty).
//      +200 = (short)(int)(third * pow(worldDifficulty, 1.1)).
//      +202 = (short)(int)((third + third) * pow(worldDifficulty, 1.1)), raised to
//      +200 + 1 when it is below that (signed halfword compare).
//      +204 = (short)(int)(worldDifficulty * 12 * fourth).
//      +220 = pow(fifth * worldDifficulty, 1.32) * 8.
//   5. result = findTagInt(creature, "Dead", 0). A nonzero result zeroes the word at
//      +560 (after the stores above). The result is returned.
// Inferred: +200/+202 look like a damage range, +560 a hit-point figure. Unknown:
// the meaning of every field, and the signed-ness of the returned count.
// norrath_creature_set_parameters is the name the constructor sources import; it is
// defined as a second symbol for the same code. The earlier authored
// norrath_actor_set_parameters implements the same original routine.
struct ParameterCreature {
    unsigned char before_flags[160];
    unsigned long long flags;
    unsigned char before_amounts[200 - 168];
    short low_amount;
    short high_amount;
    short fourth_amount;
    unsigned char before_power[220 - 206];
    float power;
    unsigned char before_second[232 - 224];
    short second_amount;
    unsigned char before_health[560 - 234];
    float health;
    float health_copy;
};
#if defined(__mips__)
static_assert(__builtin_offsetof(ParameterCreature, flags) == 160 && __builtin_offsetof(ParameterCreature, low_amount) == 200 &&
                  __builtin_offsetof(ParameterCreature, high_amount) == 202 && __builtin_offsetof(ParameterCreature, fourth_amount) == 204 &&
                  __builtin_offsetof(ParameterCreature, power) == 220 && __builtin_offsetof(ParameterCreature, second_amount) == 232 &&
                  __builtin_offsetof(ParameterCreature, health) == 560 && __builtin_offsetof(ParameterCreature, health_copy) == 564,
              "observed offsets");
#endif

extern "C" {
int eRand__10GameObject(void* object);
float powf(float base, float exponent);
int norrath_network_player_connected(int index);
int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
extern int nmPlayers;
extern signed char netNetworkMode;
extern float worldDifficulty;
extern int gameDifficulty;
}

extern "C" int setParameters__8Creaturefffffi(ParameterCreature* creature, float first, float second, float third, float fourth,
                                               float fifth, int randomize) {
    if ((creature->flags & 1u) != 0) {
        first = first * 15.0f;
        third = third * 0x1.4ccccep+0f;
        fourth = fourth * 3.0f;
        fifth = fifth * 7.0f;
    }
    if (nmPlayers >= 2) {
        if (netNetworkMode != 0) {
            int index = 0;
            do {
                norrath_network_player_connected(index);
                ++index;
            } while (index < nmPlayers);
        }
        const float extra = static_cast<float>(nmPlayers - 1);
        const float first_factor = extra * 0x1.666666p-2f + 1.0f;
        const float total = extra + 1.0f;
        fifth = fifth * total;
        first = first * first_factor;
    }
    if (randomize != 0) {
        const unsigned draw = static_cast<unsigned>(eRand__10GameObject(creature));
        const float base = powf(first * worldDifficulty, 0x1.999998p-1f);
        const float health = static_cast<float>(draw % 6u + 18u) * base;
        creature->health = health;
        if (gameDifficulty == 2) creature->health = health * 1.5f;
        else if (gameDifficulty == 3) creature->health = health + health;
        creature->health_copy = creature->health;
    }
    creature->second_amount = static_cast<short>(static_cast<int>(second * 12.0f * worldDifficulty));
    creature->low_amount = static_cast<short>(static_cast<int>(third * powf(worldDifficulty, 0x1.199998p+0f)));
    const short high = static_cast<short>(static_cast<int>((third + third) * powf(worldDifficulty, 0x1.199998p+0f)));
    creature->high_amount = high;
    if (high < creature->low_amount + 1) creature->high_amount = static_cast<short>(creature->low_amount + 1);
    creature->fourth_amount = static_cast<short>(static_cast<int>(worldDifficulty * 12.0f * fourth));
    creature->power = powf(fifth * worldDifficulty, 0x1.526e96p+0f) * 8.0f;
    static char dead_tag[] = "Dead";
    const int result = norrath_game_object_find_tag_int(creature, dead_tag, 0);
    if (result != 0) creature->health = 0.0f;
    return result;
}

#if defined(__mips__)
extern "C" int norrath_creature_set_parameters(void* object, float a, float b, float c, float d, float e, int flag)
    __attribute__((alias("setParameters__8Creaturefffffi")));
#endif
