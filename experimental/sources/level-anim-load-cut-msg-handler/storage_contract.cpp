#include "storage.hpp"
int main() {
    if (sizeof(cutMsgHandler) != 4) return 1;
    if (cutMsgHandler != 0) return 2;
    cutMsgHandler = 7;
    return cutMsgHandler == 7 ? 0 : 3;
}
