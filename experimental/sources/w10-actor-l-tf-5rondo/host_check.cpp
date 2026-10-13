// Host contract for __tf5Rondo; not an EE input. With the node's first word zero the base accessor
// and __rtti_si run once, in that order, with the class text and the Creature node; with a
// nonzero first word nothing is called. The node address is returned either way.
#include <cstring>
#include <string>
static std::string calls;
extern "C" unsigned char __ti8Creature[12];
unsigned char __ti8Creature[12];
extern "C" void* __tf8Creature() { calls += "base;"; return __ti8Creature; }
extern "C" void __rtti_si(void* node, const char* name, const void* base) {
    calls += std::string("si(") + name + "," + std::to_string(base == __ti8Creature) + ");";
    unsigned v = 0x1234; std::memcpy(node, &v, 4);
}
#include "typeinfo.cpp"
int main() {
    int bad = 0;
    std::memset(__ti5Rondo, 0, 12); calls.clear();
    if (__tf5Rondo() != __ti5Rondo || calls != "base;si(5Rondo,1);") bad = 1;
    calls.clear();
    if (__tf5Rondo() != __ti5Rondo || !calls.empty()) bad = 1;
    unsigned v = 0x100; std::memcpy(__ti5Rondo, &v, 4);
    if (__tf5Rondo() != __ti5Rondo || !calls.empty()) bad = 1;
    return bad;
}
