#define NORRATH_VSLOTS_B_HOST 1
#include "run_natasla.cpp"
#include <cstdio>
static int frames = 0;
static int checks = 0;
static int advances = 0;
static unsigned frame_result = 1u;
static int seen_distance = 0;
static float seen_amount = 0;
static void* seen_model = nullptr;
extern "C" unsigned everyFrame__8CreatureP12_modelHeader(void* self, void* model) {
    (void)self;
    ++frames;
    seen_model = model;
    return frame_result;
}
extern "C" void objectActivationCheck__8Creaturei(void* self, int distance) {
    (void)self;
    ++checks;
    seen_distance = distance;
}
extern "C" void creatureAdvanceAnimation__8Creaturef(void* self, float amount) {
    (void)self;
    ++advances;
    seen_amount = amount;
}
int main() {
    unsigned char object[40];
    unsigned char group[16];
    unsigned char model[4] = {};
    vslots_b::host_note(group);
    vslots_b::host_note(model);
    vslots_b::write_word(vslots_b::address(object) + 36u, vslots_b::address_bits(group));
    vslots_b::write_word(vslots_b::address(group) + 12u, vslots_b::address_bits(model));
    msg_run__7Natasla(object);
    if (frames != 1 || checks != 0 || advances != 0 || seen_model != model) return 1;
    frame_result = 0u;
    msg_run__7Natasla(object);
    if (checks != 1 || advances != 1 || seen_distance != 600 || seen_amount != 1.0f) return 1;
    return 0;
}
