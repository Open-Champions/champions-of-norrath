#define NORRATH_HOST_CONTRACT 1
#include "check_save.cpp"

#include <cstdio>
#include <cstring>

int saveOk;
int nmPlayers;
unsigned char player[4 * 23984];

static int eq_result = 1;
static int throws;
static void* seen_type;
static void* trigger_type = &trigger_type;
static int type_calls;
static void* object_type = &object_type;
static void* vtable[2];

extern "C" void __throw_bad_typeid() { throws += 1; }
extern "C" void* __tf7Trigger() { return trigger_type; }
extern "C" int __eq__C9type_infoRC9type_info(const void* left, const void* right) {
    seen_type = const_cast<void*>(left);
    return left == right && eq_result;
}
extern "C" int strcmp(const char* left, const char* right) {
    while (*left != 0 && *left == *right) {
        ++left;
        ++right;
    }
    return static_cast<unsigned char>(*left) - static_cast<unsigned char>(*right);
}
extern "C" void** norrath_host_vtable(void*) { return vtable; }
static void* type_of(void*) {
    type_calls += 1;
    return object_type;
}

static void put_float(unsigned char* at, float value) { std::memcpy(at, &value, 4); }

struct Fixture {
    unsigned char object[128];
    unsigned char other[64];
    char zone[16];
    Fixture() {
        std::memset(object, 0, sizeof object);
        std::memset(other, 0, sizeof other);
        std::memset(player, 0, sizeof player);
        std::strcpy(zone, "noSaveZone");
        const char* zone_pointer = zone;
        std::memcpy(object + 92, &zone_pointer, sizeof zone_pointer);
        put_float(object + 76, 1.0f);
        put_float(object + 80, 2.0f);
        put_float(object + 84, 4.0f);
        put_float(object + 88, 6.0f);
        unsigned char* other_pointer = other;
        std::memcpy(player + 21872, &other_pointer, sizeof other_pointer);
        put_float(player + 172, 1.0f);
        put_float(other + 16, 2.0f);
        put_float(other + 20, 3.0f);
        nmPlayers = 1;
        saveOk = 1;
        eq_result = 1;
        trigger_type = object_type;
        vtable[1] = reinterpret_cast<void*>(type_of);
        throws = 0;
        type_calls = 0;
    }
};

int main() {
    {
        Fixture f;
        checkSave__FP10GameObject(f.object);
        if (saveOk != 0 || type_calls != 1) return 1;
    }
    {  // Edges of the box are inclusive.
        Fixture f;
        put_float(f.other + 16, 1.0f);
        put_float(f.other + 20, 6.0f);
        checkSave__FP10GameObject(f.object);
        if (saveOk != 0) return 2;
    }
    {  // Outside on each side leaves saveOk alone.
        const float xs[] = {0.5f, 4.5f, 2.0f, 2.0f};
        const float ys[] = {3.0f, 3.0f, 1.5f, 6.5f};
        for (int i = 0; i < 4; ++i) {
            Fixture f;
            put_float(f.other + 16, xs[i]);
            put_float(f.other + 20, ys[i]);
            checkSave__FP10GameObject(f.object);
            if (saveOk != 1) return 3;
        }
    }
    {  // The record's float must be greater than zero, and the pointer nonzero.
        Fixture f;
        put_float(player + 172, 0.0f);
        checkSave__FP10GameObject(f.object);
        if (saveOk != 1) return 4;
        Fixture g;
        std::memset(player + 21872, 0, sizeof(void*));
        checkSave__FP10GameObject(g.object);
        if (saveOk != 1) return 5;
    }
    {  // A different type, a missing or different zone name, or no players change nothing.
        Fixture f;
        trigger_type = &eq_result;  // the object's type no longer equals Trigger's
        checkSave__FP10GameObject(f.object);
        if (saveOk != 1) return 6;
        Fixture g;
        g.zone[0] = 'x';
        checkSave__FP10GameObject(g.object);
        if (saveOk != 1) return 7;
        Fixture h;
        std::memset(h.object + 92, 0, sizeof(void*));
        checkSave__FP10GameObject(h.object);
        if (saveOk != 1) return 8;
        Fixture k;
        nmPlayers = 0;
        checkSave__FP10GameObject(k.object);
        if (saveOk != 1) return 9;
    }
    {  // Only the first nmPlayers records count; a later hit is ignored.
        Fixture f;
        unsigned char* other_pointer = f.other;
        std::memcpy(player + 23984 + 21872, &other_pointer, sizeof other_pointer);
        put_float(player + 23984 + 172, 1.0f);
        put_float(player + 172, 0.0f);
        nmPlayers = 1;
        checkSave__FP10GameObject(f.object);
        if (saveOk != 1) return 10;
        nmPlayers = 2;
        saveOk = 1;
        checkSave__FP10GameObject(f.object);
        if (saveOk != 0) return 11;
    }
    {  // A null object reports through __throw_bad_typeid.
        Fixture f;
        checkSave__FP10GameObject(nullptr);
        if (throws != 1) return 12;
    }
    std::puts("check save passed");
    return 0;
}
