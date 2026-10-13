// Host contract for removeMonster__FP10GameObject. Not an EE plan input.
#include <cstdio>
#include <cstring>

struct MonsterObject { unsigned char before_flags[12]; unsigned flags; unsigned char gap[570 - 16]; signed char state_byte; unsigned char tail[4]; };
extern "C" void removeMonster__FP10GameObject(MonsterObject*);
static int calls; static MonsterObject* called_with; static unsigned flags_at_call; static signed char byte_at_call;
extern "C" void objectRemoveFromGrid__FP10GameObject(MonsterObject* o) { ++calls; called_with = o; flags_at_call = o->flags; byte_at_call = o->state_byte; }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    MonsterObject o;
    std::memset(&o, 0, sizeof o);
    o.flags = 0x21; o.state_byte = 0;
    removeMonster__FP10GameObject(&o);
    expect(calls == 1 && called_with == &o, "removed from grid");
    expect(o.flags == (0x21u | 0x88000u) && o.state_byte == 4, "flags and byte updated");
    expect(flags_at_call == (0x21u | 0x88000u) && byte_at_call == 4, "stores happen before the call");
    removeMonster__FP10GameObject(&o);
    expect(calls == 1, "second call is a no-op because the byte is nonzero");
    std::memset(&o, 0, sizeof o);
    o.flags = 0x1f | 0x40; o.state_byte = 0;
    removeMonster__FP10GameObject(&o);
    expect(calls == 1 && o.flags == 0x5fu && o.state_byte == 0, "flag bit 0x20 clear: untouched");
    o.flags = 0x20; o.state_byte = -1;
    removeMonster__FP10GameObject(&o);
    expect(calls == 1 && o.flags == 0x20u && o.state_byte == -1, "nonzero (negative) byte: untouched");
    o.flags = 0xfffffffeu | 0x20u; o.state_byte = 0;
    removeMonster__FP10GameObject(&o);
    expect(calls == 2 && o.flags == 0xfffffffeu, "other flag bits preserved");
    expect(o.before_flags[0] == 0 && o.tail[0] == 0 && o.gap[0] == 0, "no other bytes written");
    if (failures) return 1;
    std::printf("removeMonster contract passed\n");
    return 0;
}
