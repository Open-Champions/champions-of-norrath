#include <cstdio>
#include <cstring>
extern "C" unsigned char norrath_script_post_action_view[200];
int main() {
    if (sizeof norrath_script_post_action_view != 200) return 1;
    if (norrath_script_post_action_view[0] != 0 || norrath_script_post_action_view[199] != 0) return 2;
    std::strcpy(reinterpret_cast<char*>(norrath_script_post_action_view), "gate");
    if (std::strcmp(reinterpret_cast<char*>(norrath_script_post_action_view), "gate") != 0) return 3;
    std::puts("post action passed");
}
