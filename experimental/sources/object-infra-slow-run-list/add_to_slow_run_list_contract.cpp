// Host contract for norrath_object_add_to_slow_run_list. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char norrath_actor_view_483ad0[4];
}
extern "C" unsigned char norrath_object_slow_run_list_view[240];
extern "C" void* norrath_object_add_to_slow_run_list(void* object);

static int failures = 0;
static int removes = 0;
static void* removed = nullptr;
static int links = 0;
static void* seen_head = nullptr;
static void* seen_object_head = nullptr;
static void* seen_value = nullptr;
static unsigned cycle_at_link = 0xffffffffu;
static void* link_result = nullptr;

static unsigned read_word(const unsigned char* p) {
    return unsigned(p[0]) | (unsigned(p[1]) << 8) |
           (unsigned(p[2]) << 16) | (unsigned(p[3]) << 24);
}
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
    cycle_at_link = read_word(norrath_actor_view_483ad0);
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
    write_word(norrath_actor_view_483ad0, start);
    link_result = reinterpret_cast<void*>(static_cast<unsigned long>(0x4000u + expected_slot));
    reset_calls();
    void* returned = norrath_object_add_to_slow_run_list(object);
    expect(returned == link_result, label);
    expect(links == 1, label);
    expect(seen_value == object, label);
    expect(seen_object_head == object + 4, label);
    expect(seen_head == norrath_object_slow_run_list_view + expected_slot * 4u, label);
    expect(cycle_at_link == expected_slot, label);
    expect(read_word(norrath_actor_view_483ad0) == expected_slot, label);
    if (existing == 0u) {
        expect(removes == 0, label);
    } else {
        expect(removes == 1, label);
        expect(removed == reinterpret_cast<void*>(static_cast<unsigned long>(existing)), label);
    }
}

int main() {
    expect(sizeof norrath_object_slow_run_list_view == 240u, "sixty heads");
    for (unsigned char byte : norrath_object_slow_run_list_view) expect(byte == 0, "heads start clear");
    check_insert(0u, 1u, 0u, "advance from zero");
    check_insert(14u, 15u, 0u, "fifteen is inside the slow range");
    check_insert(58u, 59u, 0u, "last head");
    check_insert(59u, 0u, 0u, "wrap at sixty");
    check_insert(60u, 0u, 0u, "already at sixty");
    check_insert(0xffffffffu, 0u, 0u, "negative cycle increments to zero");
    check_insert(7u, 8u, 0xabcdu, "remove the existing node first");
    write_word(norrath_actor_view_483ad0, 3u);
    check_insert(3u, 4u, 0u, "second step");
    check_insert(4u, 5u, 0u, "third step");
    if (failures != 0) return 1;
    std::printf("slow-run-list contract passed\n");
    return 0;
}
