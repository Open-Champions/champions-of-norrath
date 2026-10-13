// Host contract for msg_collision__16beetleProjectileP10GameObject; not an EE input. 32-bit words are offsets
// from a host base. The authored function was also compared with the original in a private R5900 run.
#define W13_SPELL_MSG_B_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w13_spell_msg_b_host_base;
static std::string calls;
static unsigned type_result;
unsigned w13_spell_msg_b_host_call(unsigned fn, void*, int, unsigned long, unsigned long) { calls += "type;"; return fn == 0x700 ? type_result : 0; }
extern "C" {
void __throw_bad_typeid() { calls += "throw;"; }
void* __tf10FireBeetle() { return w13_spell_msg_b_host_base + 0x300; }
bool __eq__C9type_infoRC9type_info(const void* a, const void* b) { calls += "eq;"; return a == b; }
void msg_collision__13MissileWeaponP10GameObject(void* s, void* o) {
    calls += "missile(" + std::to_string(static_cast<unsigned char*>(s) - w13_spell_msg_b_host_base) + "," + std::to_string(static_cast<unsigned char*>(o) - w13_spell_msg_b_host_base) + ");";
}
}
#include "msg_collision.cpp"

static unsigned char arena[0x1000] __attribute__((aligned(16)));
int main() {
    w13_spell_msg_b_host_base = arena;
    unsigned char* const ball = arena + 0x100;
    unsigned char* const other = arena + 0x200;
    unsigned table = 0x400;
    std::memcpy(other + 72, &table, 4);
    unsigned fn = 0x700;
    std::memcpy(arena + 0x404, &fn, 4);
    type_result = 0x300;  // same type as FireBeetle: nothing else happens
    msg_collision__16beetleProjectileP10GameObject(ball, other);
    if (calls != "type;eq;") { std::puts(calls.c_str()); return 1; }
    calls.clear(); type_result = 0x340;
    msg_collision__16beetleProjectileP10GameObject(ball, other);
    if (calls != "type;eq;missile(256,512);") { std::puts(calls.c_str()); return 2; }
    return 0;
}
