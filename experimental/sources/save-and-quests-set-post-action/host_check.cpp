#include <cstdio>
#include <cstring>
extern "C" {
unsigned char norrath_script_post_action_view[200];
}
#define strcpy norrath_host_strcpy
extern "C" char* norrath_host_strcpy(char*, const char*);
#include "set_post_action.cpp"
#undef strcpy
extern "C" char* norrath_host_strcpy(char* d, const char* s) {
    return std::strcpy(d, s);
}
int main() {
    std::memset(norrath_script_post_action_view, 0x7e, 200);
    char text[] = "newWorld castle";
    scriptSetPostAction__FPc(text);
    if (std::strcmp(reinterpret_cast<char*>(norrath_script_post_action_view), "newWorld castle") != 0) return 1;
    if (norrath_script_post_action_view[16] != 0x7e) return 2;
    char empty[] = "";
    scriptSetPostAction__FPc(empty);
    if (norrath_script_post_action_view[0] != 0 || norrath_script_post_action_view[1] != 'e') return 3;
    std::puts("ok");
    return 0;
}
