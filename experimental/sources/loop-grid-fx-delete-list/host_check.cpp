// Host contract for norrath_object_add_to_delete_list. Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" unsigned char norrath_restore_broadcast_head_view[8] = {0};
extern "C" unsigned norrath_object_add_to_delete_list(void* object);

static int failures = 0;
static int removes = 0;
static unsigned removed = 0;
static int links = 0;
static void* seen_head = nullptr;
static void* seen_second = nullptr;
static void* seen_third = nullptr;
static int order = 0;
static int remove_order = 0;
static int link_order = 0;

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
    seen_second = object_head;
    seen_third = value;
    link_order = ++order;
    return reinterpret_cast<void*>(0x7788u);
}

int main() {
    alignas(8) unsigned char object[16];
    std::memset(object, 0, sizeof object);
    unsigned result = norrath_object_add_to_delete_list(object);
    expect(removes == 0, "no removal when object+4 is zero");
    expect(links == 1, "one link call");
    expect(seen_head == norrath_restore_broadcast_head_view, "links onto the delete list head");
    expect(seen_second == object && seen_third == object, "both link arguments are the object, not object+4");
    expect(result == 0x7788u, "the link result is returned");

    removes = links = order = 0;
    object[4] = 0x34; object[5] = 0x12; object[6] = 0; object[7] = 0;
    result = norrath_object_add_to_delete_list(object);
    expect(removes == 1 && removed == 0x1234u, "removes the node word at object+4");
    expect(links == 1 && remove_order < link_order, "removal happens before the link");
    if (failures != 0) return 1;
    std::printf("delete-list contract passed\n");
    return 0;
}
