// Host contract for finishConstruction__8Creature. Not an EE plan input.
// The source is included so the host check uses the same structure declarations;
// the virtual table is a raw buffer filled with entries of that structure.
#include <cstdio>
#include <cstring>
#include "finish_construction.cpp"

static int order;
static int boss_calls, model_calls, bound_calls, tag_calls, entry184_calls, end_calls;
static int boss_order, tag_order, e184_order, end_order, bound_order;
static void* boss_self; static void* model_self[4]; static void* e184_self;
static int e184_arg;
static void* bound_model; static int bound_index;
static int bound_values[6];
static void* tag_object; static char* tag_text; static int tag_index; static char* tag_result;
static void* end_state; static int end_result;
static void* models[2];
static int model_result_null;
static unsigned long long flags_at_e184;
static FinishedCreature* subject;

static void boss(void* self) { ++boss_calls; boss_self = self; boss_order = ++order; }
static void* model(void* self) {
    model_self[model_calls < 4 ? model_calls : 3] = self;
    void* result = model_result_null ? nullptr : models[model_calls == 0 ? 0 : 1];
    ++model_calls;
    return result;
}
static void entry184(void* self, int arg) { ++entry184_calls; e184_self = self; e184_arg = arg; e184_order = ++order; flags_at_e184 = subject->flags; }
extern "C" void norrath_model_bounding_box(void* m, int index, int* a, int* b, int* c, int* d, int* e, int* f) {
    ++bound_calls; bound_model = m; bound_index = index; bound_order = ++order;
    *a = bound_values[0]; *b = bound_values[1]; *c = bound_values[2]; *d = bound_values[3]; *e = bound_values[4]; *f = bound_values[5];
}
extern "C" char* findTagString__10GameObjectPci(void* o, char* t, int i) { ++tag_calls; tag_object = o; tag_text = t; tag_index = i; tag_order = ++order; return tag_result; }
extern "C" unsigned norrath_animation_set_to_end_fully_blended(void* s) { ++end_calls; end_state = s; end_order = ++order; return end_result; }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    alignas(16) unsigned char table[256];
    VirtualEntry* e128 = reinterpret_cast<VirtualEntry*>(table + 128);
    VirtualEntry* e144 = reinterpret_cast<VirtualEntry*>(table + 144);
    VirtualEntry* e184 = reinterpret_cast<VirtualEntry*>(table + 184);
    static FinishedCreature creature;
    subject = &creature;
    e128->delta = 8; e128->function = reinterpret_cast<void*>(model);
    e144->delta = -4; e144->function = reinterpret_cast<void*>(boss);
    e184->delta = 16; e184->function = reinterpret_cast<void*>(entry184);
    char* self_base = reinterpret_cast<char*>(&creature);
    creature.vtable = reinterpret_cast<const VirtualEntry*>(table);
    models[0] = &models[0]; models[1] = &models[1];
    bound_values[2] = 10; bound_values[5] = 70;  // third = 10, sixth = 70
    // Plain case: no boss bit, tag absent.
    tag_result = nullptr;
    unsigned r = finishConstruction__8Creature(&creature);
    expect(boss_calls == 0, "no boss call without bit 0");
    expect(model_calls == 2 && model_self[0] == self_base + 8 && model_self[1] == self_base + 8, "model getter called twice with the adjusted object");
    expect(bound_calls == 1 && bound_model == models[1] && bound_index == -1, "the second result is the model, index -1");
    expect(creature.height == 60.0f && creature.height_byte == 61, "height 60 is not below 60: kept, byte is 61");
    expect(tag_calls == 1 && tag_object == &creature && tag_index == 0 && std::strcmp(tag_text, "dead") == 0, "tag query uses \"dead\" and index 0");
    expect(r == 0 && entry184_calls == 0 && end_calls == 0 && creature.flags == 0, "absent tag stops there and returns 0");
    // Height from the box, above the floor.
    bound_values[5] = 100; model_calls = 0;
    finishConstruction__8Creature(&creature);
    expect(creature.height == 90.0f && creature.height_byte == 91, "sixth minus third, byte is truncation plus one");
    bound_values[5] = 12; bound_values[2] = 10; model_calls = 0;
    finishConstruction__8Creature(&creature);
    expect(creature.height == 60.0f && creature.height_byte == 61, "small boxes are raised to 60");
    bound_values[5] = 10 + 75; model_calls = 0;
    finishConstruction__8Creature(&creature);
    expect(creature.height == 75.0f && creature.height_byte == 76, "exact integer height");
    // Null model: no box, height untouched.
    creature.height = 5.0f; creature.height_byte = 3; bound_calls = 0; model_calls = 0; model_result_null = 1;
    finishConstruction__8Creature(&creature);
    expect(model_calls == 1 && bound_calls == 0 && creature.height == 5.0f && creature.height_byte == 3, "null model skips the box and leaves the fields");
    model_result_null = 0;
    // Boss bit, tag present.
    creature.flags = 1; tag_result = const_cast<char*>("x"); end_result = 77; order = 0; model_calls = 0; boss_calls = 0;
    r = finishConstruction__8Creature(&creature);
    expect(boss_calls == 1 && boss_self == self_base - 4 && boss_order == 1, "boss entry first, with its own delta");
    expect(entry184_calls == 1 && e184_self == self_base + 16 && e184_arg == 10, "+184 entry called with 10");
    expect(flags_at_e184 == (0x100000001ull) && creature.flags == 0x100000001ull, "bit 32 is set before the +184 call and bit 0 kept");
    expect(tag_order < e184_order && e184_order < end_order, "tag, then +184 entry, then end-of-blend");
    expect(end_calls == 1 && end_state == creature.animation_state && r == 77, "animation state at +288 passed and its result returned");
    if (failures) return 1;
    std::printf("finishConstruction contract passed\n");
    return 0;
}
