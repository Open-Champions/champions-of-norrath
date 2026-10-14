// Observed objectFindAnimation__FP10GameObjectPc at 0x152cc8, extent 356.
// Looks up an animation resource for an object by name. Words used: +36 a pointer
// to a record whose first word is a char* (the resource owner for most
// classes), +72 the virtual table, +960 an inline text field that is the owner
// when the object's class is exactly NPC (typeid(*object) == typeid(NPC), tested
// with __eq__C9type_infoRC9type_info against __tf3NPC(); a null object calls
// __throw_bad_typeid). Two 160-byte stack buffers hold the generated names.
//   1. sprintf(a, "%sname", name); tag = findTagString(object, a, 0).
//   2. If tag is found, findTagString is called again with the same arguments,
//      sprintf(b, "%s.anm", result) and the answer is lumpFindResource(owner, b).
//   3. Otherwise tag = findTagString(object, "name", 0). If that is found,
//      sprintf(b, "%s_%s.anm", tag, name) and lumpFindResource(owner, b); a
//      nonzero answer is returned.
//   4. Fallback: sprintf(b, "%s.anm", name) and lumpFindResource(owner, b).
// Unknown: the layout of the record at +36, what the NPC field is, and whether
// the repeated findTagString call in step 2 was a source-level duplicate.
#include "../w8-object-list-math-common/object_types.hpp"

struct ObjectForAnimation {
    void* word0;
    ListNode* run_node;
    ListNode* grid_node;
    unsigned flags;
    unsigned char gap_to_36[36 - 16];
    char** resource_holder;
    unsigned char gap_to_72[72 - 40];
    const void* const* vtable;
    unsigned char gap_to_960[960 - 76];
    char npc_resource[1];
};

#if defined(__mips__)
static_assert(__builtin_offsetof(ObjectForAnimation, resource_holder) == 36 && __builtin_offsetof(ObjectForAnimation, vtable) == 72 &&
                  __builtin_offsetof(ObjectForAnimation, npc_resource) == 960, "observed offsets");
#endif

extern "C" {
const TypeRecord* __tf3NPC();
int __eq__C9type_infoRC9type_info(const TypeRecord* left, const TypeRecord* right);
void __throw_bad_typeid();
int sprintf(char* destination, const char* format, ...);
char* findTagString__10GameObjectPci(void* object, const char* tag, int index);
char* lumpFindResource__FPcT0(char* owner, char* name);
}

extern "C" char* objectFindAnimation__FP10GameObjectPc(ObjectForAnimation* object, char* name) {
    if (object == nullptr) __throw_bad_typeid();
    const TypeRecordGetter get_type = reinterpret_cast<TypeRecordGetter>(const_cast<void*>(object->vtable[1]));
    char* owner = object->npc_resource;
    if (!__eq__C9type_infoRC9type_info(get_type(), __tf3NPC())) owner = object->resource_holder[0];

    char tag_name[160];
    char resource_name[160];
    sprintf(tag_name, "%sname", name);
    if (findTagString__10GameObjectPci(object, tag_name, 0)) {
        sprintf(resource_name, "%s.anm", findTagString__10GameObjectPci(object, tag_name, 0));
        return lumpFindResource__FPcT0(owner, resource_name);
    }
    char* tag = findTagString__10GameObjectPci(object, "name", 0);
    if (tag) {
        sprintf(resource_name, "%s_%s.anm", tag, name);
        char* found = lumpFindResource__FPcT0(owner, resource_name);
        if (found) return found;
    }
    sprintf(resource_name, "%s.anm", name);
    return lumpFindResource__FPcT0(owner, resource_name);
}
