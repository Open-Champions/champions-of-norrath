static void* g_arg;
static int g_calls;
extern "C" void* __9VICollRay(void* self) {
    g_arg = self;
    ++g_calls;
    return reinterpret_cast<char*>(self) + 5;
}
#include "ctor.cpp"

int main() {
    char object[16];
    void* r = __12VICollSphere(object);
    if (g_calls != 1 || g_arg != object) return 1;
    if (r != object + 5) return 2;
    return 0;
}
