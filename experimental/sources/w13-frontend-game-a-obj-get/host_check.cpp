// Host check for objGet__FP10GameObjectPc. Not an EE plan input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror -DW13_FRONT_HOST -fno-builtin host_check.cpp
// The image's 32-bit pointer words (name text, property array, vtable, function words, type text)
// are small handles that hostWordAddress maps to host addresses; the pool offsets, record layout
// and the order of effects are checked against hand-written expectations.
#include "obj_get.cpp"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
unsigned char* poolStart;
unsigned char* poolEnd;
}
static std::string trace;
extern "C" void __throw_bad_typeid() { trace += "throw;"; }

alignas(8) static unsigned char pool[256];
alignas(8) static unsigned char vtable[200];
alignas(8) static unsigned char typeInfo[8];
static std::vector<const void*> handles;
static unsigned handleOf(const void* p) {
    for (size_t i = 0; i < handles.size(); ++i) if (handles[i] == p) return 0x1000u + static_cast<unsigned>(i);
    handles.push_back(p);
    return 0x1000u + static_cast<unsigned>(handles.size() - 1);
}
__UINTPTR_TYPE__ hostWordAddress(unsigned word) {
    if (word == 0) return 0;
    return reinterpret_cast<__UINTPTR_TYPE__>(handles.at(word - 0x1000u));
}
static int saveWritten;
static void* typeObjectSeen;
static void* saveObjectSeen;
static void* saveBufferSeen;
static void* typeFn(void* object) { trace += "type;"; typeObjectSeen = object; return typeInfo; }
static int saveFn(void* object, void* buffer) {
    trace += "save;";
    saveObjectSeen = object;
    saveBufferSeen = buffer;
    std::memset(buffer, 0x6b, 4);
    return saveWritten;
}

#define CHECK(c) do { if (!(c)) { std::printf("line %d: %s\n    trace: %s\n", __LINE__, #c, trace.c_str()); return 1; } } while (0)

template <class T>
static void setAt(unsigned char* base, int off, T v) { std::memcpy(base + off, &v, sizeof v); }
template <class T>
static T getAt(const unsigned char* base, int off) { T v; std::memcpy(&v, base + off, sizeof v); return v; }

static void addEntry(unsigned char*& at, const char* text) {
    *at = static_cast<unsigned char>(std::strlen(text));
    std::strcpy(reinterpret_cast<char*>(at + 1), text);
    at += std::strlen(text) + 2;
}

struct Fixture {
    alignas(8) unsigned char object[200];
    alignas(8) unsigned char record[512];
    alignas(8) unsigned char list[40];
    std::string name;
    std::vector<std::string> texts;
    std::string typeName;
};

static void setup(Fixture& f, const char* name, const char* typeName, const char* const* texts, int n) {
    handles.clear();
    trace.clear();
    std::memset(f.object, 0, sizeof f.object);
    std::memset(f.record, 0xcc, sizeof f.record);
    std::memset(f.list, 0, sizeof f.list);
    std::memset(vtable, 0, sizeof vtable);
    f.name = name ? name : "";
    f.typeName = typeName ? typeName : "";
    f.texts.clear();
    for (int i = 0; i < n; ++i) f.texts.push_back(texts[i] ? texts[i] : "");
    setAt<unsigned>(f.object, 52, name ? handleOf(f.name.c_str()) : 0);
    setAt<unsigned>(f.object, 72, handleOf(vtable));
    setAt<unsigned>(vtable, 4, handleOf(reinterpret_cast<const void*>(&typeFn)));
    setAt<short>(vtable, 72, 4);
    setAt<unsigned>(vtable, 76, handleOf(reinterpret_cast<const void*>(&saveFn)));
    setAt<unsigned>(typeInfo, 0, handleOf(f.typeName.c_str()));
    setAt<float>(f.object, 16, 1.5f); setAt<float>(f.object, 20, -2.5f); setAt<float>(f.object, 24, 3.25f);
    setAt<unsigned short>(f.object, 56, 0xbeef);
    if (n > 0) {
        setAt<unsigned>(f.object, 44, handleOf(f.list));
        for (int i = 0; i < n; ++i) setAt<unsigned>(f.list, 4 * i, texts[i] == nullptr ? 0 : handleOf(f.texts[i].c_str()));
    }
    saveWritten = 10;
}

static void setPool(const char* const* entries, int n) {
    std::memset(pool, 0xaa, sizeof pool);
    unsigned char* at = pool;
    for (int i = 0; i < n; ++i) addEntry(at, entries[i]);
    poolStart = pool;
    poolEnd = at;
}

int main() {
    Fixture f;
    // 1. typeid path: leading digits skipped, the text interned into an empty pool at offset 1
    {
        setPool(nullptr, 0);
        setup(f, nullptr, "12ItemProp", nullptr, 0);
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(trace == "type;save;" && typeObjectSeen == f.object);
        CHECK(poolEnd == pool + 10 && pool[0] == 8 && std::strcmp(reinterpret_cast<char*>(pool + 1), "ItemProp") == 0);
        CHECK(getAt<int>(f.record, 0) == 1);
        CHECK(getAt<unsigned short>(f.record, 6) == 0xbeef);
        CHECK(getAt<float>(f.record, 8) == 1.5f && getAt<float>(f.record, 12) == -2.5f && getAt<float>(f.record, 16) == 3.25f);
        CHECK(getAt<int>(f.record, 20) == 0);                        // no properties: the end marker
        // buffer = (record + 4 * count + 31) & ~7 = record + 24 for an 8-aligned record and no properties
        CHECK(saveBufferSeen == f.record + 24 && saveObjectSeen == f.object + 4);
        CHECK(f.record[24] == 0x6b && f.record[28] == 0xcc);
        // end = (record + 24 + 10 + 3) & ~3 = record + 36
        CHECK(getAt<unsigned short>(f.record, 4) == 36);
    }
    // 2. name text path: cut at ';' or ',' and found/added in the pool
    {
        const char* entries[] = {"Alpha", "Ax", "Beta"};
        setPool(entries, 3);
        unsigned char* end = poolEnd;
        setup(f, "Beta;ignored", nullptr, nullptr, 0);
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(trace == "save;");                                      // no type call when the name word is set
        CHECK(getAt<int>(f.record, 0) == (pool[0] + 2) + (pool[pool[0] + 2] + 2) + 1);
        CHECK(poolEnd == end);                                        // found: nothing added
        setup(f, "Ax,q", nullptr, nullptr, 0);
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(getAt<int>(f.record, 0) == (5 + 2) + 1);                // matches "Ax", not "Alpha" (same first letter)
        CHECK(poolEnd == end);
        setup(f, "Axe;1", nullptr, nullptr, 0);                       // a longer name sharing a prefix is a new entry
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(getAt<int>(f.record, 0) == static_cast<int>(end - pool) + 1);
        CHECK(poolEnd == end + 5 && end[0] == 3 && std::strcmp(reinterpret_cast<char*>(end + 1), "Axe") == 0);
        setup(f, "", nullptr, nullptr, 0);                            // an empty name is interned as an empty entry
        unsigned char* before = poolEnd;
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(getAt<int>(f.record, 0) == static_cast<int>(before - pool) + 1 && poolEnd == before + 2 && *before == 0);
    }
    // 2b. entries are matched at entry boundaries only: "eta" is not found inside "Zeta"
    {
        const char* entries[] = {"Zeta", "Beta"};
        setPool(entries, 2);
        unsigned char* end = poolEnd;
        setup(f, "eta", nullptr, nullptr, 0);
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(getAt<int>(f.record, 0) == static_cast<int>(end - pool) + 1 && poolEnd == end + 5);
        setup(f, "Beta", nullptr, nullptr, 0);
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(getAt<int>(f.record, 0) == 7 && poolEnd == end + 5);
    }
    // 3. property list: each text interned, offsets from out + 20, a zero ends the list, buffer after it
    {
        const char* entries[] = {"Alpha"};
        setPool(entries, 1);
        const char* texts[] = {"Beta", "Alpha", "Zed"};
        setup(f, "Alpha", nullptr, texts, 3);
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(getAt<int>(f.record, 0) == 1);
        CHECK(getAt<int>(f.record, 20) == 8);                          // "Beta" appended after "Alpha" (7 bytes)
        CHECK(getAt<int>(f.record, 24) == 1);                          // existing "Alpha"
        CHECK(getAt<int>(f.record, 28) == 8 + 6);                      // "Zed" after "Beta"
        CHECK(getAt<int>(f.record, 32) == 0);
        // count 3: buffer = (record + 12 + 31) & ~7 = record + 40
        CHECK(saveBufferSeen == f.record + 40);
        CHECK(getAt<unsigned short>(f.record, 4) == ((40 + 10 + 3) & ~3));
        CHECK(poolEnd == pool + 7 + 6 + 5);
        // a null first text means no list at all
        const char* nulls[] = {nullptr};
        setup(f, "Alpha", nullptr, nulls, 1);
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(getAt<int>(f.record, 20) == 0 && saveBufferSeen == f.record + 24);
    }
    // 4. no pool: every offset is zero and nothing is added
    {
        poolStart = nullptr; poolEnd = nullptr;
        const char* texts[] = {"Beta"};
        setup(f, "Name", nullptr, texts, 1);
        objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
        CHECK(getAt<int>(f.record, 0) == 0 && getAt<int>(f.record, 20) == 0 && getAt<int>(f.record, 24) == 0);
        CHECK(poolStart == nullptr && poolEnd == nullptr);
        CHECK(saveBufferSeen == f.record + 32);       // one property: (record + 4 + 31) & ~7
    }
    // 5. the size halfword and the rounding of the saved byte count
    {
        setPool(nullptr, 0);
        for (int w = 0; w < 9; ++w) {
            saveWritten = w;
            setup(f, "N", nullptr, nullptr, 0);
            saveWritten = w;
            objGet__FP10GameObjectPc(f.object, reinterpret_cast<char*>(f.record));
            CHECK(getAt<unsigned short>(f.record, 4) == ((24 + w + 3) & ~3));
        }
    }
    std::puts("obj get passed");
    return 0;
}
