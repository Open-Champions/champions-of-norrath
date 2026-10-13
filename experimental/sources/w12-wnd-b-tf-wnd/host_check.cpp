#include <cstring>
extern "C" {
extern unsigned char ti_wnd[8] __asm__("__ti5VIWnd");
void* wnd_typeinfo() __asm__("__tf5VIWnd");
}
static int calls;
static void* seen_node;
static const char* seen_name;
extern "C" void host_rtti_user(void*, const char*) __asm__("__rtti_user");
extern "C" void host_rtti_user(void* node, const char* name) {
    ++calls; seen_node = node; seen_name = name;
    static_cast<unsigned char*>(node)[0] = 1;
}
int main() {
    std::memset(ti_wnd, 0, 8);
    if (wnd_typeinfo() != ti_wnd || calls != 1 || seen_node != ti_wnd ||
        std::strcmp(seen_name, "5VIWnd") != 0) return 1;
    if (wnd_typeinfo() != ti_wnd || calls != 1) return 2;
    std::memset(ti_wnd, 0, 8); ti_wnd[3] = 1; calls = 0;
    if (wnd_typeinfo() != ti_wnd || calls != 0) return 3;
    return 0;
}
