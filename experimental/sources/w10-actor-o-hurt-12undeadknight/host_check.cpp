// Host contract for msg_hurt__12UndeadKnightfP10GameObjecti; not an EE input. Includes the source.
#include "msg_hurt.cpp"
#include <cstring>
#include <initializer_list>
char bossMeterName[32];
int bossMeterCurrent;
static int calls, order; static void *s_self, *s_attacker; static float s_damage; static int s_flag, name_set_before_base;
extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* self, float damage, void* attacker, int flag) {
    ++calls; s_self = self; s_damage = damage; s_attacker = attacker; s_flag = flag;
    name_set_before_base = std::memcmp(bossMeterName, "UndeadKnight", 13) == 0;
}
int main() {
    alignas(8) unsigned char obj[600], other[8];
    for (float v : {0.0f, 41.75f, -3.5f, 99.99f, -0.5f, 1e6f}) {
        std::memset(obj, 0, sizeof obj); std::memset(bossMeterName, 'x', sizeof bossMeterName); bossMeterCurrent = -77; calls = 0;
        std::memcpy(obj + 560, &v, 4);
        msg_hurt__12UndeadKnightfP10GameObjecti(obj, 3.25f, other, 9);
        if (calls != 1 || s_self != obj || s_damage != 3.25f || s_attacker != other || s_flag != 9) return 1;
        if (name_set_before_base) return 2;  // the base call comes first
        if (std::memcmp(bossMeterName, "UndeadKnight", 13) != 0 || bossMeterName[13] != 'x') return 3;
        if (bossMeterCurrent != static_cast<int>(v)) return 4;
    }
    return 0;
}
