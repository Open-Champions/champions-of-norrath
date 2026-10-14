// Host contract for the dialog and HUD message handles. Not an EE plan input.
#include "msg_handles.cpp"

int main() {
    if (dialogMsgHandle != 0 || hudMsgHandle != 0) return 1;
    dialogMsgHandle = 19;
    hudMsgHandle = 42;
    if (dialogMsgHandle != 19 || hudMsgHandle != 42) return 2;
    return 0;
}
