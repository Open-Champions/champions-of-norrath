#include "close.cpp"

static int calls;
static int seen_port = -1;
static int seen_slot = -1;
static int flag_at_call = -1;
static int* watched;
extern "C" int scePadPortClose(int port, int slot) {
    ++calls;
    seen_port = port;
    seen_slot = slot;
    flag_at_call = watched[13];
    return 1;
}

int main() {
    int object[16];
    for (int& word : object) word = 0x1234;
    watched = object;
    object[0] = 1;
    object[1] = 2;
    object[13] = 0;
    Close__5VIPad(object);
    if (calls != 0) return 1;
    object[13] = 1;
    Close__5VIPad(object);
    if (calls != 1 || seen_port != 1 || seen_slot != 2) return 2;
    if (flag_at_call != 0 || object[13] != 0) return 3;
    Close__5VIPad(object);
    if (calls != 1) return 4;
    object[13] = 7;
    Close__5VIPad(object);
    if (calls != 2 || object[13] != 0) return 5;
    for (int index = 2; index < 13; ++index)
        if (object[index] != 0x1234) return 6;
    return 0;
}
