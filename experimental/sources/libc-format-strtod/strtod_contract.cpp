#include <cstdint>
#include <cstdio>

extern "C" unsigned long long strtod(const char* text, char** end);
extern "C" unsigned char norrath_factory_impure_pointer_word[4] = {0x78, 0x56, 0x34, 0x12};

static const char* g_text;
static char** g_end;
static unsigned long long g_reent;
static int g_calls;

extern "C" unsigned long long norrath_player_strtod_reentrant(unsigned char* reent, const char* text, char** end) {
    ++g_calls;
    g_reent = static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(reent));
    g_text = text;
    g_end = end;
    if (end != nullptr) *end = const_cast<char*>(text + 6);
    return 0x3ff004189374bc6aULL;
}

static unsigned long long call(const char* text, char** end) {
    auto* function = strtod;
    __asm__ __volatile__("" : "+r"(function));
    return function(text, end);
}

int main() {
    const char* text = "1.0004";
    if (call(text, nullptr) != 0x3ff004189374bc6aULL) return 1;
    if (g_calls != 1 || g_text != text || g_end != nullptr) return 2;
    if (static_cast<unsigned>(g_reent) != 0x12345678u) return 3;
    char* end = nullptr;
    if (call("1.0004", &end) != 0x3ff004189374bc6aULL) return 4;
    if (g_calls != 2 || end != text + 6) return 5;
    std::printf("strtod cases passed\n");
    return 0;
}
