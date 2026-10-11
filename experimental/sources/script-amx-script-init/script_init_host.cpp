#include <cstdio>
#include <cstring>

extern "C" {
unsigned char norrath_script_current_amx_word_view[4] = {0x11, 0x22, 0x33, 0x44};
void scriptInit__Fv();
void scriptTask__Fv() {}

static int calls = 0;
static void (*seen_callback)() = nullptr;
static int seen_second = -1;
static char* seen_third = nullptr;
static int seen_fourth = -1;
static unsigned char during_call[4];

void engineAddTask__FPFv_viPci(void (*callback)(), int second, char* third, int fourth) {
    ++calls;
    seen_callback = callback;
    seen_second = second;
    seen_third = third;
    seen_fourth = fourth;
    for (int i = 0; i < 4; ++i) during_call[i] = norrath_script_current_amx_word_view[i];
    norrath_script_current_amx_word_view[0] = 0xab;
    norrath_script_current_amx_word_view[1] = 0xcd;
    norrath_script_current_amx_word_view[2] = 0xef;
    norrath_script_current_amx_word_view[3] = 0x01;
}
}

int main() {
    scriptInit__Fv();
    if (calls != 1) return 1;
    if (seen_callback != scriptTask__Fv) return 2;
    if (seen_second != 125 || seen_fourth != 0) return 3;
    if (seen_third == nullptr || std::strcmp(seen_third, "Script") != 0) return 4;
    if (std::strlen(seen_third) != 6) return 5;
    if (during_call[0] != 0x11 || during_call[1] != 0x22 || during_call[2] != 0x33 || during_call[3] != 0x44)
        return 6;
    for (int i = 0; i < 4; ++i) {
        if (norrath_script_current_amx_word_view[i] != 0) return 7;
    }
    std::puts("script init passed");
    return 0;
}
