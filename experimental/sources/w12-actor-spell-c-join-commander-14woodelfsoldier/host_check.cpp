// Host contract for joinCommander__14WoodElfSoldier; not an EE input. Includes the source.
#include "join_commander.cpp"
#include <cstdio>
#include <string>

static unsigned char a[2048], b[2048];
static void* found;
static std::string calls;
extern "C" {
void* findCommander__14WoodElfSoldier(void* self) { calls += self == a ? "find(a);" : "find(?);"; return found; }
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    auto name = [](const void* p) -> std::string {
        if (p == a + 960) return "a+960"; if (p == b + 960) return "b+960"; if (p == a) return "a"; if (p == b) return "b"; return "?";
    };
    calls += "link(" + name(head) + "," + name(link) + "," + name(data) + ");";
}
}

int main() {
    int bad = 0;
    found = nullptr; calls.clear();
    if (joinCommander__14WoodElfSoldier(a) != 0 || calls != "find(a);") { std::puts(calls.c_str()); std::puts("fail: no commander"); bad = 1; }
    found = b; calls.clear();
    if (joinCommander__14WoodElfSoldier(a) != 1 || calls != "find(a);link(a+960,b,b);link(b+960,a,a);") { std::puts(calls.c_str()); std::puts("fail: commander found"); bad = 1; }
    return bad;
}
