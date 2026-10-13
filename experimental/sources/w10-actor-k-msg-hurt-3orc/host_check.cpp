// Host contract for msg_hurt__3OrcfP10GameObjecti; not an EE input. The Creature handler is
// replaced by a recorder; the contract fails unless it sees the same four arguments once.
static int calls;
static void* seen[2];
static float seen_amount;
static int seen_extra;
extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* attacker, int extra) {
    ++calls; seen[0] = self; seen[1] = attacker; seen_amount = amount; seen_extra = extra;
}
#include "msg_hurt.cpp"
int main() {
    static unsigned char o[64], a[64];
    msg_hurt__3OrcfP10GameObjecti(o, 7.25f, a, -9);
    return !(calls == 1 && seen[0] == o && seen[1] == a && seen_amount == 7.25f && seen_extra == -9);
}
