// Host contract for msg_getPokeName__11PushTriggerPUsi; not an EE input. Tag lookups, the
// script test and the text copy are stubs that log; scenarios cover no keys, accepted keys,
// a rejected key with and without a lock message, and a missing message.
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
static std::string calls;
static std::map<std::string, char*> tags;      // "name:index" -> value (absent = null)
static std::map<std::string, int> script;       // key text -> script result
extern "C" char* findTagString__10GameObjectPci(void*, const char* name, int n) {
    calls += std::string("tag(") + name + "," + std::to_string(n) + ");";
    auto it = tags.find(std::string(name) + ":" + std::to_string(n));
    return it == tags.end() ? nullptr : it->second;
}
extern "C" int scriptStart__FPc(const char* k) { calls += std::string("script(") + k + ");"; return script[k]; }
static unsigned short texts[4][8];
extern "C" unsigned short* textGetString__FPc(const char* k) { calls += std::string("text(") + k + ");"; return texts[k[0] == 'M' ? 0 : 1]; }
extern "C" void ustrcpy__FPUsT0(unsigned short* to, unsigned short* from) { calls += "copy;"; std::memcpy(to, from, 16); }
#include "msg_get_poke_name.cpp"
int main() {
    int bad = 0;
    char k0[] = "K0", k1[] = "K1", msg[] = "Mmsg", lock[] = "Llock";
    for (int i = 0; i < 8; ++i) { texts[0][i] = 100 + i; texts[1][i] = 200 + i; }
    unsigned short out[8];
    struct Case { bool key0, key1, ok0, ok1, hasMsg, hasLock; int ret; const char* log; unsigned short first; };
    const Case cases[] = {
        {false, false, 0, 0, true, true, 1, "tag(key,0);tag(message,0);tag(message,0);text(Mmsg);copy;", 100},
        {false, false, 0, 0, false, true, 1, "tag(key,0);tag(message,0);", 0},
        {true, true, 1, 1, true, true, 1, "tag(key,0);script(K0);tag(key,1);script(K1);tag(key,2);tag(message,0);tag(message,0);text(Mmsg);copy;", 100},
        {true, true, 1, 0, true, true, 0, "tag(key,0);script(K0);tag(key,1);script(K1);tag(lockMessage,0);text(Llock);copy;", 200},
        {true, false, 0, 0, true, false, 0, "tag(key,0);script(K0);tag(lockMessage,0);", 0},
    };
    for (const Case& c : cases) {
        calls.clear(); tags.clear(); script.clear(); std::memset(out, 0x55, sizeof out);
        if (c.key0) tags["key:0"] = k0;
        if (c.key1) tags["key:1"] = k1;
        if (c.hasMsg) tags["message:0"] = msg;
        if (c.hasLock) tags["lockMessage:0"] = lock;
        script["K0"] = c.ok0; script["K1"] = c.ok1;
        unsigned char self[16];
        const int r = msg_getPokeName__11PushTriggerPUsi(self, out);
        if (r != c.ret || calls != c.log || out[0] != c.first) { std::printf("fail ret %d log %s\n", r, calls.c_str()); bad = 1; }
        if (c.first == 0 && out[1] != 0x5555) bad = 1;
    }
    return bad;
}
