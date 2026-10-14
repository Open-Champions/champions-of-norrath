// Host contract for objectFindObjectByClass__FPcPP10GameObjecti. Not an EE plan input.
// Build: c++ -std=c++17 find_object_by_class.cpp ../w8-object-list-math-run-list-heads/run_list_heads.cpp \
//        ../w8-object-list-math-safe-iterator-construct/safe_iterator_construct.cpp \
//        ../w8-object-list-math-safe-iterator-next/safe_iterator_next.cpp \
//        ../w8-object-list-math-safe-iterator-destroy/safe_iterator_destroy.cpp host_check.cpp
#include <cstdio>
#include <cstring>
#include "../w8-object-list-math-common/safe_iterator.hpp"

struct HeadSlot { ListHead head; unsigned unobserved_word; };
extern "C" HeadSlot runList;
extern "C" ListHead mediumRunList[16];
extern "C" ListHead slowRunList[60];
extern "C" int objectIgnoreFlags = 0;
extern "C" void objectFindObjectByClass__FPcPP10GameObjecti(char*, GameObjectVirtual**, int);
extern "C" void norrath_animation_state_deallocate(void*) {}
static int bad_typeid_calls;
extern "C" void __throw_bad_typeid() { ++bad_typeid_calls; }

static const TypeRecord goblin_type = {"Goblin"};
static const TypeRecord orc_type = {"Orc"};
static const TypeRecord* goblin_getter() { return &goblin_type; }
static const TypeRecord* orc_getter() { return &orc_type; }
static const void* goblin_table[2] = {nullptr, reinterpret_cast<const void*>(goblin_getter)};
static const void* orc_table[2] = {nullptr, reinterpret_cast<const void*>(orc_getter)};

struct Entry {
    GameObjectVirtual object;
    ListNode node;
};
static Entry entries[16];
static int used;

static Entry* add(ListHead* head, const void* const* table, unsigned flags = 0) {
    Entry* e = &entries[used++];
    std::memset(e, 0, sizeof *e);
    e->object.vtable = table;
    e->object.flags = flags;
    e->node.value = &e->object;
    // Append at the tail so list order is insertion order.
    ListNode** link = &head->first;
    ListNode* previous = reinterpret_cast<ListNode*>(head);
    while (*link) { previous = *link; link = &(*link)->next; }
    *link = &e->node;
    e->node.prev = previous;
    return e;
}
static void reset() {
    std::memset(entries, 0, sizeof entries);
    used = 0;
    std::memset(&runList, 0, sizeof runList);
    std::memset(mediumRunList, 0, sizeof mediumRunList);
    std::memset(slowRunList, 0, sizeof slowRunList);
    objectIgnoreFlags = 0;
}

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    GameObjectVirtual* out[64];
    char goblin[] = "Goblin";

    reset();
    Entry* a = add(&runList.head, goblin_table);
    add(&runList.head, orc_table);
    Entry* c = add(&runList.head, goblin_table);
    Entry* d = add(&mediumRunList[3], goblin_table);
    Entry* e = add(&mediumRunList[14], goblin_table);
    add(&mediumRunList[14], orc_table);
    Entry* f = add(&slowRunList[0], goblin_table);
    Entry* g = add(&slowRunList[59], goblin_table);
    Entry* ignored_list_member = add(&mediumRunList[15], goblin_table);  // head 15 is never walked
    (void)ignored_list_member;

    std::memset(out, 0xaa, sizeof out);
    objectFindObjectByClass__FPcPP10GameObjecti(goblin, out, 50);
    expect(out[0] == &a->object && out[1] == &c->object && out[2] == &d->object && out[3] == &e->object && out[4] == &f->object &&
               out[5] == &g->object && out[6] == nullptr,
           "run list, then medium heads in order, then slow heads; terminated; head 15 skipped");
    expect(runList.head.first == &a->node && a->node.next != nullptr && runList.head.first->prev == reinterpret_cast<ListNode*>(&runList.head),
           "lists are left as they were (iterator removed)");

    objectFindObjectByClass__FPcPP10GameObjecti(goblin, out, 3);
    expect(out[0] == &a->object && out[1] == &c->object && out[2] == nullptr, "stops when count reaches limit - 1");
    objectFindObjectByClass__FPcPP10GameObjecti(goblin, out, 4);
    expect(out[2] == &d->object && out[3] == nullptr, "limit 4 stops after the first medium match");
    expect(d->node.next == nullptr && d->node.prev == reinterpret_cast<ListNode*>(&mediumRunList[3]), "early exit also detaches the iterator");
    objectFindObjectByClass__FPcPP10GameObjecti(goblin, out, 1);
    expect(out[0] == &a->object && out[5] == &g->object && out[6] == nullptr, "a limit of 1 never stops the walk (equality test)");
    objectFindObjectByClass__FPcPP10GameObjecti(goblin, out, 6);
    expect(out[4] == &f->object && out[5] == nullptr, "stop at exactly limit - 1 in the slow lists");

    char orc[] = "Orc", gob[] = "Gob", none[] = "";
    objectFindObjectByClass__FPcPP10GameObjecti(orc, out, 50);
    expect(out[0] == &entries[1].object && out[1] == &entries[5].object && out[2] == nullptr, "other class names match their own objects");
    objectFindObjectByClass__FPcPP10GameObjecti(gob, out, 50);
    expect(out[0] == nullptr, "a prefix of the class name does not match");
    objectFindObjectByClass__FPcPP10GameObjecti(none, out, 50);
    expect(out[0] == nullptr, "empty name matches nothing here");

    // objectIgnoreFlags is the iterator mask: flagged objects are never offered.
    a->object.flags = 0x40;
    objectIgnoreFlags = 0x40;
    objectFindObjectByClass__FPcPP10GameObjecti(goblin, out, 50);
    expect(out[0] == &c->object && out[4] == &g->object && out[5] == nullptr, "objects with an ignore bit are skipped");

    reset();
    std::memset(out, 0xaa, sizeof out);
    objectFindObjectByClass__FPcPP10GameObjecti(goblin, out, 50);
    expect(out[0] == nullptr, "no objects gives an empty, terminated array");
    if (failures) return 1;
    std::printf("objectFindObjectByClass contract passed\n");
    return 0;
}
