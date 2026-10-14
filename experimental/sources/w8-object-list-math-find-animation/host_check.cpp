// Host contract for objectFindAnimation__FP10GameObjectPc. Not an EE plan input.
// Build: c++ -std=c++17 find_animation.cpp host_check.cpp
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "../w8-object-list-math-common/object_types.hpp"

struct ObjectForAnimation {
    void* word0; ListNode* run_node; ListNode* grid_node; unsigned flags;
    unsigned char gap_to_36[36 - 16]; char** resource_holder;
    unsigned char gap_to_72[72 - 40]; const void* const* vtable;
    unsigned char gap_to_960[960 - 76]; char npc_resource[1];
};
extern "C" char* objectFindAnimation__FP10GameObjectPc(ObjectForAnimation*, char*);

static const TypeRecord npc_type = {"NPC"}, other_type = {"Other"};
extern "C" const TypeRecord* __tf3NPC() { return &npc_type; }
extern "C" int __eq__C9type_infoRC9type_info(const TypeRecord* a, const TypeRecord* b) { return a == b; }
static int bad_typeid_calls;
extern "C" void __throw_bad_typeid() { ++bad_typeid_calls; }
static const TypeRecord* get_npc() { return &npc_type; }
static const TypeRecord* get_other() { return &other_type; }
static const void* npc_table[2] = {nullptr, reinterpret_cast<const void*>(get_npc)};
static const void* other_table[2] = {nullptr, reinterpret_cast<const void*>(get_other)};

// findTagString: answers for a fixed set of tag names; records every call.
static std::vector<std::string> tag_calls;
static std::string tag_a_answer, tag_name_answer;
static char answer_a_storage[64], answer_name_storage[64];
static std::string known_tag;    // the "<name>name" tag that exists, or empty
extern "C" char* findTagString__10GameObjectPci(void*, const char* tag, int index) {
    tag_calls.push_back(std::string(tag) + "#" + std::to_string(index));
    if (!known_tag.empty() && known_tag == tag) return answer_a_storage;
    if (std::strcmp(tag, "name") == 0 && !tag_name_answer.empty()) return answer_name_storage;
    return nullptr;
}
static std::vector<std::string> lump_calls;
static std::vector<char*> lump_owners;
static std::string existing_resource;
static char resource_storage[8];
extern "C" char* lumpFindResource__FPcT0(char* owner, char* name) {
    lump_calls.push_back(name);
    lump_owners.push_back(owner);
    return existing_resource == name ? resource_storage : nullptr;
}

static void reset() { tag_calls.clear(); lump_calls.clear(); lump_owners.clear(); bad_typeid_calls = 0; }

int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    static union { ObjectForAnimation view; unsigned char raw[1200]; } storage;
    ObjectForAnimation& object = storage.view;
    static char holder_text[] = "holder-owner";
    static char* holder = holder_text;
    std::memset(storage.raw, 0, sizeof storage.raw);
    object.vtable = other_table;
    object.resource_holder = &holder;
    char walk[] = "walk";

    // Step 4 only: no tags at all, non-NPC owner comes from the record at +36.
    reset();
    known_tag.clear(); tag_name_answer.clear(); existing_resource = "walk.anm";
    char* r = objectFindAnimation__FP10GameObjectPc(&object, walk);
    expect(r == resource_storage, "fallback resource returned");
    expect(tag_calls.size() == 2 && tag_calls[0] == "walkname#0" && tag_calls[1] == "name#0", "tag lookups: <name>name then name, index 0");
    expect(lump_calls.size() == 1 && lump_calls[0] == "walk.anm" && lump_owners[0] == holder_text, "owner is the first word of the +36 record for other classes");

    // Per-animation tag found: used directly, looked up twice, the result decides the name.
    reset();
    known_tag = "walkname"; std::strcpy(answer_a_storage, "run"); existing_resource = "run.anm";
    r = objectFindAnimation__FP10GameObjectPc(&object, walk);
    expect(r == resource_storage && lump_calls.size() == 1 && lump_calls[0] == "run.anm", "<name>name tag redirects to its value");
    expect(tag_calls.size() == 2 && tag_calls[0] == "walkname#0" && tag_calls[1] == "walkname#0", "the same tag is looked up twice");
    existing_resource = "nope";
    reset();
    r = objectFindAnimation__FP10GameObjectPc(&object, walk);
    expect(r == nullptr && lump_calls.size() == 1, "no further fallback after a redirect");

    // Generic name tag: "<tag>_<name>.anm" first, then the plain name.
    reset();
    known_tag.clear(); tag_name_answer = "orc"; std::strcpy(answer_name_storage, "orc"); existing_resource = "orc_walk.anm";
    r = objectFindAnimation__FP10GameObjectPc(&object, walk);
    expect(r == resource_storage && lump_calls.size() == 1 && lump_calls[0] == "orc_walk.anm", "name tag builds tag_name.anm");
    reset();
    existing_resource = "walk.anm";
    r = objectFindAnimation__FP10GameObjectPc(&object, walk);
    expect(r == resource_storage && lump_calls.size() == 2 && lump_calls[0] == "orc_walk.anm" && lump_calls[1] == "walk.anm", "falls back to the plain name when the tagged one is missing");
    reset();
    existing_resource = "none";
    r = objectFindAnimation__FP10GameObjectPc(&object, walk);
    expect(r == nullptr && lump_calls.size() == 2, "both lookups fail gives null");

    // NPC objects use the inline field at +960.
    reset();
    object.vtable = npc_table;
    std::strcpy(object.npc_resource, "npc-owner");
    known_tag.clear(); tag_name_answer.clear(); existing_resource = "walk.anm";
    r = objectFindAnimation__FP10GameObjectPc(&object, walk);
    expect(lump_owners.size() == 1 && lump_owners[0] == object.npc_resource, "NPC owner is the inline text at +960");

    expect(bad_typeid_calls == 0, "no spurious bad typeid for valid objects");
    if (failures) return 1;
    std::printf("objectFindAnimation contract passed\n");
    return 0;
}
