// Host contract for getModel__7Critter; not an EE input. 32-bit words in the object are offsets
// from a host base (W10_ACTOR_D_HOST), so the real field offset (+1036) is used.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <initializer_list>
unsigned char* w10_actor_d_host_base;
#include "get_model.cpp"
int main() {
    alignas(16) static unsigned char arena[4096], object[1100];
    w10_actor_d_host_base = arena;
    int bad = 0;
    for (unsigned model : {0u, 64u, 3000u}) {
        std::memset(object, 0, sizeof object);
        std::memcpy(object + 1036, &model, 4);
        unsigned other = 77; std::memcpy(object + 1032, &other, 4); std::memcpy(object + 1040, &other, 4);
        if (getModel__7Critter(object) != arena + model) { std::puts("fail: wrong word"); bad = 1; }
    }
    return bad;
}
