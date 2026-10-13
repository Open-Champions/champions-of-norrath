// Host check only; not an EE build input.
#include "msg_hurt.cpp"
#include <cstdio>
static int calls, failures;
static void* seen_self; static float seen_amount; static GameObject* seen_source; static int seen_kind;
extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, GameObject* source, int kind) {
    ++calls; seen_self = self; seen_amount = amount; seen_source = source; seen_kind = kind;
}
int main() {
    int a, b;
    msg_hurt__14WoodElfSoldierfP10GameObjecti(&a, 12.5f, reinterpret_cast<GameObject*>(&b), -7);
    if (calls != 1) { std::fprintf(stderr, "fail: one call\n"); ++failures; }
    if (seen_self != &a || seen_amount != 12.5f || seen_source != reinterpret_cast<GameObject*>(&b) || seen_kind != -7) {
        std::fprintf(stderr, "fail: arguments forwarded unchanged\n"); ++failures;
    }
    if (!failures) std::puts("msg_hurt ok");
    return failures ? 1 : 0;
}
