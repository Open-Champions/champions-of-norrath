#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "msg_get_poke_name.cpp"

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

// tag name -> list of values by index
static std::map<std::string, std::vector<std::string>> tags;
static std::map<std::string, int> script_results;
static std::vector<std::string> log_calls;
static unsigned short wide_entry[4];
static std::string last_format;
static std::string last_sprintf_arg;
static std::map<std::string, std::string> tag_storage;

extern "C" char* findTagString__10GameObjectPci(void*, const char* tag, int index) {
    log_calls.push_back(std::string("find:") + tag + ":" + std::to_string(index));
    auto it = tags.find(tag);
    if (it == tags.end() || index < 0 || index >= static_cast<int>(it->second.size())) return nullptr;
    return const_cast<char*>(it->second[index].c_str());
}
extern "C" int scriptStart__FPc(char* script) {
    log_calls.push_back(std::string("script:") + script);
    return script_results[script];
}
extern "C" unsigned short* textGetString__FPc(char* key) {
    log_calls.push_back(std::string("text:") + key);
    wide_entry[0] = static_cast<unsigned short>(key[0]);
    wide_entry[1] = 0;
    return wide_entry;
}
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, const unsigned short* source) {
    log_calls.push_back("wide-copy");
    destination[0] = source[0];
    destination[1] = source[1];
}
extern "C" void ustrcpy__FPUsPc(unsigned short* destination, const char* source) {
    log_calls.push_back(std::string("narrow-copy:") + source);
    destination[0] = static_cast<unsigned short>(source[0]);
    destination[1] = 0;
}
extern "C" int format_text(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    last_format = format;
    int n;
    if (std::strcmp(format, "message%d") == 0) {
        int value = va_arg(args, int);
        n = std::snprintf(buffer, 64, "message%d", value);
        log_calls.push_back(std::string("sprintf:") + buffer);
    } else {
        const char* s = va_arg(args, const char*);
        last_sprintf_arg = s;
        n = std::snprintf(buffer, 64, "%s: Missing lock message.", s);
        log_calls.push_back(std::string("sprintf:") + buffer);
    }
    va_end(args);
    return n;
}

alignas(8) static unsigned char object[512];
static unsigned short name[4];

static void setup(unsigned flags, int state, const char* object_name) {
    std::memset(object, 0, sizeof object);
    std::memcpy(object + 284, &flags, 4);
    std::memcpy(object + 376, &state, 4);
    std::strcpy(reinterpret_cast<char*>(object + 76), object_name);
    std::memset(name, 0xff, sizeof name);
    tags.clear();
    script_results.clear();
    log_calls.clear();
}
static bool has(const char* entry) {
    for (auto& s : log_calls) if (s == entry) return true;
    return false;
}

int main() {
    setup(0, 0, "x");
    tags["key"] = {"k0"};
    expect(msg_getPokeName__8AutoPropPUsi(object, name, 0) == 0 && log_calls.empty(), "bit 4 clear: returns 0 at once");

    // two keys, both pass, then the plain message tag
    setup(0x10, 5, "door");
    tags["key"] = {"k0", "k1"};
    script_results["k0"] = 1;
    script_results["k1"] = 7;
    tags["message"] = {"mtext"};
    int r = msg_getPokeName__8AutoPropPUsi(object, name, 0);
    expect(r == 1, "unlocked with message returns 1");
    const std::vector<std::string> want = {"find:key:0", "script:k0", "find:key:1", "script:k1", "find:key:2", "sprintf:message5",
                                           "find:message:0", "find:message:0", "text:mtext", "wide-copy"};
    expect(log_calls == want, "unlocked sequence with plain message");
    expect(name[0] == 'm' && name[1] == 0 && name[2] == 0xffff, "buffer holds the entry");

    // no key tags, numbered message only
    setup(0x10, 3, "door");
    tags["message3"] = {"num"};
    r = msg_getPokeName__8AutoPropPUsi(object, name, 0);
    expect(r == 1 && has("text:num") && has("sprintf:message3"), "numbered message");
    expect(log_calls[0] == "find:key:0" && log_calls[1] == "sprintf:message3", "key lookup precedes formatting");

    // neither message
    setup(0x10, 3, "door");
    r = msg_getPokeName__8AutoPropPUsi(object, name, 0);
    expect(r == 0 && name[0] == 0xffff, "no message returns 0 and writes nothing");

    // locked with lockMessage
    setup(0x10, 1, "door");
    tags["key"] = {"k0", "k1"};
    script_results["k0"] = 1;
    script_results["k1"] = 0;
    tags["lockMessage"] = {"lock"};
    r = msg_getPokeName__8AutoPropPUsi(object, name, 0);
    expect(r == 0, "locked returns 0");
    const std::vector<std::string> want_locked = {"find:key:0", "script:k0", "find:key:1", "script:k1", "find:lockMessage:0",
                                                  "text:lock", "wide-copy"};
    expect(log_calls == want_locked, "locked sequence with lockMessage");
    expect(name[0] == 'l', "lock text copied");

    // locked without lockMessage
    setup(0x10, 1, "chest");
    tags["key"] = {"k0"};
    script_results["k0"] = 0;
    r = msg_getPokeName__8AutoPropPUsi(object, name, 0);
    expect(r == 0, "locked, no message: returns 0");
    const std::vector<std::string> want_missing = {"find:key:0", "script:k0", "find:lockMessage:0",
                                                   "sprintf:chest: Missing lock message.", "narrow-copy:chest"};
    expect(log_calls == want_missing, "diagnostic then narrow copy of the object name");
    expect(name[0] == 'c', "object name copied");
    return failures ? 1 : 0;
}
