// Host check: records the call order and arguments of the straight-line path.
#include "throw_bad_typeid.cpp"
#include <cstring>

static int step;
static unsigned allocated_size;
static void* block;
static void* pushed_object;
static void* pushed_type;
static void (*pushed_destructor)();
static int order[8];
static int count;
static unsigned char type_token[1];

extern "C" {
unsigned char bad_typeid_vtable[32];
void* eh_alloc(unsigned size) {
    order[count++] = 1;
    allocated_size = size;
    static void* storage[1];
    storage[0] = nullptr;
    block = storage;
    return block;
}
void* tf_bad_typeid() {
    order[count++] = 2;
    // The vtable word must already be in place when this is called.
    if (*static_cast<void**>(block) != bad_typeid_vtable) order[count++] = 99;
    return type_token;
}
void cp_push_exception(void* object, void* type, void (*destructor)()) {
    order[count++] = 3;
    pushed_object = object;
    pushed_type = type;
    pushed_destructor = destructor;
}
void raise_exception() { order[count++] = 4; }
void bad_typeid_destructor() {}
}

int main() {
    __throw_bad_typeid();
    const int expected[4] = {1, 2, 3, 4};
    if (count != 4) return 1;
    if (std::memcmp(order, expected, sizeof expected) != 0) return 2;
    if (allocated_size != 4) return 3;
    if (*static_cast<void**>(block) != bad_typeid_vtable) return 4;
    if (pushed_object != block || pushed_type != type_token) return 5;
    if (pushed_destructor != bad_typeid_destructor) return 6;
    return 0;
}
