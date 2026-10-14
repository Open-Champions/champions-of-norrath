// Host contract for norrath_actor_add_medium_run_list. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_medium_run_list_heads[60];
extern "C" unsigned norrath_medium_list_cycle_word;
extern "C" unsigned norrath_actor_add_medium_run_list(void* object);

static int failures = 0;
static int removes = 0;
static void* removed = nullptr;
static int links = 0;
static void* seen_head = nullptr;
static void* seen_object_head = nullptr;
static void* seen_value = nullptr;
static unsigned cycle_at_link = 0xffffffffu;
static void* link_result = nullptr;

static void write_word(unsigned char* p, unsigned value) {
    p[0] = static_cast<unsigned char>(value);
    p[1] = static_cast<unsigned char>(value >> 8);
    p[2] = static_cast<unsigned char>(value >> 16);
    p[3] = static_cast<unsigned char>(value >> 24);
}
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

extern "C" unsigned norrath_list_remove(unsigned char* node) {
    ++removes;
    removed = node;
    return 0x111u;
}
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value) {
    ++links;
    seen_head = head;
    seen_object_head = object_head;
    seen_value = value;
    cycle_at_link = norrath_medium_list_cycle_word;
    return link_result;
}

static void reset_calls() {
    removes = 0;
    removed = nullptr;
    links = 0;
    seen_head = nullptr;
    seen_object_head = nullptr;
    seen_value = nullptr;
    cycle_at_link = 0xffffffffu;
}

static void check_insert(unsigned start, unsigned expected_slot, unsigned existing, const char* label) {
    alignas(16) unsigned char object[16];
    std::memset(object, 0x5a, sizeof object);
    write_word(object + 4, existing);
    norrath_medium_list_cycle_word = start;
    link_result = reinterpret_cast<void*>(static_cast<unsigned long>(0x4000u + expected_slot));
    reset_calls();
    const unsigned returned = norrath_actor_add_medium_run_list(object);
    expect(returned == static_cast<unsigned>(reinterpret_cast<unsigned long>(link_result)), label);
    expect(links == 1, label);
    expect(seen_value == object, label);
    expect(seen_object_head == object + 4, label);
    expect(seen_head == norrath_medium_run_list_heads + expected_slot * 4u, label);
    expect(cycle_at_link == expected_slot, label);
    expect(norrath_medium_list_cycle_word == expected_slot, label);
    if (existing == 0u) {
        expect(removes == 0, label);
    } else {
        expect(removes == 1, label);
        expect(removed == reinterpret_cast<void*>(static_cast<unsigned long>(existing)), label);
    }
}

int main() {
    expect(sizeof norrath_medium_run_list_heads == 60u, "fifteen heads");
    for (unsigned char byte : norrath_medium_run_list_heads) expect(byte == 0, "heads start clear");
    expect(norrath_medium_list_cycle_word == 0u, "cycle starts clear");
    check_insert(0u, 1u, 0u, "advance from zero");
    check_insert(13u, 14u, 0u, "last head");
    check_insert(14u, 0u, 0u, "wrap at fifteen");
    check_insert(15u, 0u, 0u, "already at fifteen");
    check_insert(0xffffffffu, 0u, 0u, "negative cycle increments to zero");
    check_insert(0x7fffffffu, 0x80000000u, 0u, "signed compare does not clamp a high bit");
    check_insert(7u, 8u, 0xabcdu, "remove the existing node first");
    if (failures != 0) return 1;
    std::printf("medium-run-list contract passed\n");
    return 0;
}
