// Host check only; not an EE input. __tf10GameObject and __rtti_si are recording stubs; the nodes are local arrays.
#include "typeinfo.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char __ti8HolyBolt[12] __attribute__((aligned(4)));
unsigned char __ti10Projectile[12] __attribute__((aligned(4)));
unsigned char __ti10GameObject[8] __attribute__((aligned(4)));
static std::string calls;
extern "C" void* __tf10GameObject() { calls += "base;"; return __ti10GameObject; }
extern "C" void __rtti_si(void* t, const char* name, const void* base) {
    calls += std::string("si(") + (t == __ti8HolyBolt ? "node" : t == __ti10Projectile ? "proj" : "?") + "," + name + ","
        + (base == __ti10Projectile ? "proj" : base == __ti10GameObject ? "base" : "?") + ");";
    static_cast<unsigned char*>(t)[0] = 1;
}

int main() {
    int failures = 0;
    struct Row { bool node_set, proj_set; const char* want; };
    static const Row rows[] = {
        {false, false, "base;si(proj,10Projectile,base);si(node,8HolyBolt,proj);"},
        {false, true, "si(node,8HolyBolt,proj);"},
        {true, false, ""},
        {true, true, ""},
    };
    for (const Row& r : rows) {
        std::memset(__ti8HolyBolt, 0, sizeof __ti8HolyBolt);
        std::memset(__ti10Projectile, 0, sizeof __ti10Projectile);
        std::memset(__ti10GameObject, 0, sizeof __ti10GameObject);
        if (r.node_set) std::memcpy(__ti8HolyBolt, "\x01\0\0\0", 4);
        if (r.proj_set) std::memcpy(__ti10Projectile, "\x01\0\0\0", 4);
        calls.clear();
        if (__tf8HolyBolt() != __ti8HolyBolt) { std::fprintf(stderr, "fail: return\n"); ++failures; }
        if (calls != r.want) { std::fprintf(stderr, "fail: calls %s\n", calls.c_str()); ++failures; }
    }
    if (!failures) std::puts("w16-m-data-a-tf-8holybolt ok");
    return failures ? 1 : 0;
}
