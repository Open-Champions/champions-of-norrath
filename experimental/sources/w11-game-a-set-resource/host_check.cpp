// Host contract for GameObject::setResource; not an EE input.
#include "set_resource.cpp"
#include <cstdio>
#include <cstring>
#include <vector>
int resourceFrame;
static std::vector<const char*> seen; static const char* seenLump; static int redirect;
static unsigned char* spare; static unsigned char* objectUnderTest;
void* lumpFindResource__FPcT0(char* l, char* n) {
    seenLump = l; seen.push_back(n);
    if (redirect && objectUnderTest) { std::memcpy(objectUnderTest + 36, &spare, sizeof spare); redirect = 0; }
    return n + 1;
}
struct M3 { char* lump; int frame; struct { char* name; void* resource; } e[4]; };
int main() {
    int bad = 0;
    alignas(8) unsigned char object[64] = {};
    char lump[] = "L", a[] = "aaa", b[] = "bbb", c[] = "ccc";
    M3 map = {lump, 5, {{a, nullptr}, {b, nullptr}, {c, nullptr}, {nullptr, nullptr}}};
    resourceFrame = 5;
    setResource__10GameObjectPQ210GameObject11resourceMap(object, &map);
    void* stored; std::memcpy(&stored, object + 36, sizeof stored);
    if (stored != &map || !seen.empty() || map.e[0].resource) { std::puts("fail: same frame"); bad = 1; }
    resourceFrame = 6;
    setResource__10GameObjectPQ210GameObject11resourceMap(object, &map);
    if (seen.size() != 3 || seen[0] != a || seen[1] != b || seen[2] != c || seenLump != lump) { std::puts("fail: lookups"); bad = 1; }
    if (map.e[0].resource != a + 1 || map.e[1].resource != b + 1 || map.e[2].resource != c + 1 || map.e[3].resource || map.frame != 6) { std::puts("fail: results"); bad = 1; }
    M3 empty = {lump, 1, {{nullptr, nullptr}}};
    seen.clear(); resourceFrame = 9;
    setResource__10GameObjectPQ210GameObject11resourceMap(object, &empty);
    if (!seen.empty() || empty.frame != 9) { std::puts("fail: empty"); bad = 1; }
    setResource__10GameObjectPQ210GameObject11resourceMap(object, nullptr);
    std::memcpy(&stored, object + 36, sizeof stored);
    if (stored != nullptr) { std::puts("fail: null store"); bad = 1; }
    // a lookup that retargets the object's map pointer redirects the later stores
    M3 other = {lump, 0, {{a, nullptr}, {b, nullptr}, {nullptr, nullptr}}};
    M3 spareMap = {lump, 0, {{a, nullptr}, {b, nullptr}, {nullptr, nullptr}}};
    spare = reinterpret_cast<unsigned char*>(&spareMap); objectUnderTest = object; redirect = 1; seen.clear();
    setResource__10GameObjectPQ210GameObject11resourceMap(object, &other);
    if (spareMap.e[0].resource == nullptr || other.e[0].resource != nullptr || spareMap.frame != 9) { std::puts("fail: reload"); bad = 1; }
    if (!bad) std::puts("setResource ok");
    return bad;
}
