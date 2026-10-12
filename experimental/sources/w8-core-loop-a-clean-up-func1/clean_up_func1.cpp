// Observed cleanUpFunc1__FP10GameObject at 0x0017e0d8, 304 bytes (file-local in the original). It is an
// objectIterateGrid callback used by cleanUp__Fv. The type query is the function pointer at vtable byte 4 (the
// vtable pointer is the word at object +72), called with no argument assignment, and a null object calls
// __throw_bad_typeid first. In order, the query result is compared with the types of PushTrigger, Savepoint and
// NPC (__tf11PushTrigger, __tf9Savepoint, __tf3NPC through __eq__C9type_infoRC9type_info), re-running the
// query for every comparison. A match returns at once. Otherwise, if the object word at byte 12 has bit
// 0x00100000 set, the virtual slot at vtable byte 112 is called (halfword this-adjustment at +112, function
// pointer at +116) with arguments (object + adjustment, 0), and then objectIterateGrid is called with
// killExpendableFunc__FP10GameObject. Without the bit nothing more happens.
// Inferred: none of the class names is claimed beyond the three comparisons.
// Unknown: what the slot at byte 112 does, and why the whole grid is walked after each object.
extern "C" {
void* __tf11PushTrigger();
void* __tf9Savepoint();
void* __tf3NPC();
int __eq__C9type_infoRC9type_info(void* left, void* right);
void __throw_bad_typeid();
void objectIterateGrid__FPFP10GameObject_v(void (*visit)(unsigned char*));
void killExpendableFunc__FP10GameObject(unsigned char* object);
}

namespace {
unsigned load_word(const unsigned char* bytes) {
    return static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8) |
           (static_cast<unsigned>(bytes[2]) << 16) | (static_cast<unsigned>(bytes[3]) << 24);
}

short load_half(const unsigned char* bytes) {
    return static_cast<short>(static_cast<unsigned>(bytes[0]) | (static_cast<unsigned>(bytes[1]) << 8));
}

unsigned char* load_pointer(const unsigned char* bytes) {
    unsigned char* value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

using TypeCall = void* (*)();
using Slot = void (*)(void* self, int argument);

TypeCall load_type(const unsigned char* bytes) {
    TypeCall value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

Slot load_slot(const unsigned char* bytes) {
    Slot value = nullptr;
    auto* out = reinterpret_cast<unsigned char*>(&value);
    for (unsigned i = 0; i < sizeof value; ++i) out[i] = bytes[i];
    return value;
}

void* query_type(unsigned char* object) {
    if (object == nullptr) {
        __throw_bad_typeid();
        return nullptr;
    }
    return load_type(load_pointer(object + 72) + 4)();
}
}

extern "C" void cleanUpFunc1__FP10GameObject(unsigned char* object) {
    if (__eq__C9type_infoRC9type_info(query_type(object), __tf11PushTrigger())) return;
    if (__eq__C9type_infoRC9type_info(query_type(object), __tf9Savepoint())) return;
    if (__eq__C9type_infoRC9type_info(query_type(object), __tf3NPC())) return;
    if (!(load_word(object + 12) & 0x00100000u)) return;
    const unsigned char* vtable = load_pointer(object + 72);
    const short adjust = load_half(vtable + 112);
    load_slot(vtable + 116)(object + adjust, 0);
    objectIterateGrid__FPFP10GameObject_v(killExpendableFunc__FP10GameObject);
}
