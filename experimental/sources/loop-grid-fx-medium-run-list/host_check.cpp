// Host contract for norrath_object_add_to_medium_run_list. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_medium_run_list_heads[60] = {0};
extern "C" unsigned norrath_medium_list_cycle_word = 0;
extern "C" unsigned norrath_object_add_to_medium_run_list(void* object);

static int failures = 0;
static int removes = 0, links = 0, order = 0, remove_order = 0, link_order = 0;
static unsigned removed = 0;
static void* seen_head = nullptr;
static void* seen_node = nullptr;
static void* seen_value = nullptr;
static unsigned cycle_at_link = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}
extern "C" unsigned norrath_list_remove(unsigned char* node) {
    ++removes;
    removed = static_cast<unsigned>(reinterpret_cast<unsigned long>(node));
    remove_order = ++order;
    return 0;
}
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value) {
    ++links;
    seen_head = head;
    seen_node = object_head;
    seen_value = value;
    cycle_at_link = norrath_medium_list_cycle_word;
    link_order = ++order;
    return reinterpret_cast<void*>(0x1357u);
}
static unsigned run(unsigned start, unsigned existing_word) {
    alignas(8) unsigned char object[16];
    std::memset(object, 0, sizeof object);
    std::memcpy(object + 4, &existing_word, 4);
    norrath_medium_list_cycle_word = start;
    removes = links = order = 0;
    const unsigned result = norrath_object_add_to_medium_run_list(object);
    expect(seen_node == object + 4 && seen_value == object, "node is object+4 and value is the object");
    return result;
}

int main() {
    unsigned result = run(3, 0);
    expect(removes == 0 && links == 1, "no removal when object+4 is zero");
    expect(norrath_medium_list_cycle_word == 4u && cycle_at_link == 4u, "cycle advances before the link call");
    expect(seen_head == norrath_medium_run_list_heads + 16, "head is heads + cycle * 4");
    expect(result == 0x1357u, "link result is returned");
    run(13, 0);
    expect(norrath_medium_list_cycle_word == 14u && seen_head == norrath_medium_run_list_heads + 56, "cycle 14 is the last head");
    run(14, 0);
    expect(norrath_medium_list_cycle_word == 0u && seen_head == norrath_medium_run_list_heads, "15 wraps to zero");
    run(0x7fffffffu, 0);
    expect(norrath_medium_list_cycle_word == 0x80000000u, "the compare is signed, so a negative count is not clamped");
    run(2, 0xabcdu);
    expect(removes == 1 && removed == 0xabcdu && remove_order < link_order, "existing node is removed first");
    if (failures != 0) return 1;
    std::printf("medium-run-list contract passed\n");
    return 0;
}
