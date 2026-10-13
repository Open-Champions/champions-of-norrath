// Host contract for setParameters__8Creaturefffffi. Not an EE plan input.
// Build: c++ -std=c++17 -ffp-contract=off host_check.cpp   (the source is included for its structure)
#include <cmath>
#include <cstdio>
#include <cstring>
#include <vector>
#include "set_parameters.cpp"

int nmPlayers;
signed char netNetworkMode;
float worldDifficulty;
int gameDifficulty;
static float host_pow(float b, float e) { return static_cast<float>(::pow(static_cast<double>(b), static_cast<double>(e))); }
static std::vector<int> connected_calls;
static int rand_value, rand_calls, tag_result, tag_calls;
static std::vector<std::pair<float, float>> pow_calls;
static void* rand_object; static void* tag_object; static char tag_text[16]; static int tag_occurrence;
extern "C" int eRand__10GameObject(void* o) { ++rand_calls; rand_object = o; return rand_value; }
extern "C" float powf(float b, float e) { pow_calls.push_back({b, e}); return host_pow(b, e); }
extern "C" int norrath_network_player_connected(int i) { connected_calls.push_back(i); return 0; }
extern "C" int norrath_game_object_find_tag_int(void* o, char* t, int occ) {
    ++tag_calls; tag_object = o; std::strcpy(tag_text, t); tag_occurrence = occ; return tag_result;
}

static int failures;
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } }
static ParameterCreature creature;
static void reset() {
    std::memset(&creature, 0, sizeof creature);
    connected_calls.clear(); pow_calls.clear(); rand_calls = 0; tag_calls = 0; tag_result = 0;
}
static short as_short(float v) { return static_cast<short>(static_cast<int>(v)); }

int main() {
    const float c13 = 1.3f, c035 = 0.35f, c08 = 0x1.999998p-1f, c11 = 0x1.199998p+0f, c132 = 0x1.526e96p+0f;
    worldDifficulty = 1.5f; gameDifficulty = 0; nmPlayers = 1; netNetworkMode = 0;

    // Plain call: no boss, one player, no randomization.
    reset();
    int r = setParameters__8Creaturefffffi(&creature, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 0);
    expect(r == 0 && tag_calls == 1 && tag_object == &creature && std::strcmp(tag_text, "Dead") == 0 && tag_occurrence == 0, "tag query is \"Dead\" occurrence 0 on the creature");
    expect(rand_calls == 0 && creature.health == 0.0f && creature.health_copy == 0.0f, "no randomization leaves +560/+564 alone");
    expect(creature.second_amount == as_short(3.0f * 12.0f * 1.5f), "second goes to +232");
    float p11 = host_pow(1.5f, c11);
    expect(creature.low_amount == as_short(4.0f * p11), "third goes to +200");
    expect(creature.high_amount == as_short((4.0f + 4.0f) * p11), "double third goes to +202");
    expect(creature.fourth_amount == as_short(1.5f * 12.0f * 5.0f), "fourth goes to +204");
    expect(creature.power == host_pow(6.0f * 1.5f, c132) * 8.0f, "fifth goes to +220");
    expect(pow_calls.size() == 3 && pow_calls[0].second == c11 && pow_calls[1].second == c11 && pow_calls[2].second == c132, "three pow calls without randomization");

    // Boss bit.
    reset();
    creature.flags = 0x100000001ull;
    setParameters__8Creaturefffffi(&creature, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 0);
    float third = 4.0f * c13, fourth = 5.0f * 3.0f, fifth = 6.0f * 7.0f;
    expect(creature.low_amount == as_short(third * p11) && creature.high_amount == as_short((third + third) * p11), "boss multiplies third by 1.3");
    expect(creature.fourth_amount == as_short(1.5f * 12.0f * fourth), "boss multiplies fourth by 3");
    expect(creature.power == host_pow(fifth * 1.5f, c132) * 8.0f, "boss multiplies fifth by 7");
    expect(creature.flags == 0x100000001ull, "flags are only read");

    // Several players: networked mode polls every index; the factors follow.
    reset();
    nmPlayers = 3; netNetworkMode = 1;
    setParameters__8Creaturefffffi(&creature, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 1);
    expect(connected_calls.size() == 3 && connected_calls[0] == 0 && connected_calls[1] == 1 && connected_calls[2] == 2, "every player index is polled once in order");
    float extra = 2.0f;
    float first = 2.0f * (extra * c035 + 1.0f);
    fifth = 6.0f * (extra + 1.0f);
    expect(pow_calls.size() == 4 && pow_calls[0].first == first * 1.5f && pow_calls[0].second == c08, "first is scaled by extra * 0.35 + 1 before the health power");
    expect(creature.power == host_pow(fifth * 1.5f, c132) * 8.0f, "fifth is scaled by the player count");
    netNetworkMode = 0; connected_calls.clear();
    setParameters__8Creaturefffffi(&creature, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 0);
    expect(connected_calls.empty(), "no polling when netNetworkMode is zero");
    nmPlayers = 1; netNetworkMode = 1; pow_calls.clear();
    setParameters__8Creaturefffffi(&creature, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 0);
    expect(connected_calls.empty(), "one player skips the whole block");

    // Randomization and difficulty.
    nmPlayers = 1; netNetworkMode = 0;
    for (int difficulty = 0; difficulty <= 3; ++difficulty) {
        reset();
        gameDifficulty = difficulty; rand_value = 40000;
        setParameters__8Creaturefffffi(&creature, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 1);
        float health = static_cast<float>(40000u % 6u + 18u) * host_pow(2.0f * 1.5f, c08);
        if (difficulty == 2) health = health * 1.5f; else if (difficulty == 3) health = health + health;
        expect(rand_calls == 1 && rand_object == &creature, "random draw on the creature");
        expect(creature.health == health && creature.health_copy == health, "health scaled by difficulty and copied");
    }
    gameDifficulty = 0;

    // High amount raised to low + 1.
    reset();
    worldDifficulty = 1.0f;
    setParameters__8Creaturefffffi(&creature, 1.0f, 1.0f, 0.4f, 1.0f, 1.0f, 0);
    expect(creature.low_amount == 0 && creature.high_amount == 1, "high below low+1 is raised");
    reset();
    setParameters__8Creaturefffffi(&creature, 1.0f, 1.0f, 10.0f, 1.0f, 1.0f, 0);
    expect(creature.low_amount == 10 && creature.high_amount == 20, "high above low+1 is kept");
    reset();
    setParameters__8Creaturefffffi(&creature, 1.0f, 1.0f, -3.0f, 1.0f, 1.0f, 0);
    expect(creature.low_amount == -3 && creature.high_amount == -2, "signed compare: -6 < -2 raises high to low+1");
    worldDifficulty = 1.5f;

    // A nonzero tag result zeroes +560 and is returned.
    reset();
    gameDifficulty = 0; rand_value = 7; tag_result = 5;
    r = setParameters__8Creaturefffffi(&creature, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 1);
    expect(r == 5 && creature.health == 0.0f && creature.health_copy != 0.0f, "tag result zeroes +560 only, after the stores");
    if (failures) return 1;
    std::printf("setParameters contract passed\n");
    return 0;
}
