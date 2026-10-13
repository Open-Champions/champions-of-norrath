#include "begin.cpp"
static const void* seen;
static int calls;
extern "C" int End__Ct6VIList1ZP9VIGUIPage(const void* list) { seen = list; ++calls; return 4242; }
int main() {
    int list[4] = {7, 0x1234, 0, 0};
    if (Begin__Ct6VIList1ZP9VIGUIPage(list) != 0x1234 || calls) return 1;
    list[1] = -1;
    if (Begin__Ct6VIList1ZP9VIGUIPage(list) != -1 || calls) return 2;
    list[1] = 0;
    if (Begin__Ct6VIList1ZP9VIGUIPage(list) != 4242 || calls != 1 || seen != list) return 3;
    return 0;
}
