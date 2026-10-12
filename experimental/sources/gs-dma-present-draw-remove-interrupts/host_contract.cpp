#include "draw_remove_interrupts.cpp"

#include <cstdio>
#include <string>

extern "C" int vif0HID = 0;
extern "C" int vif1HID = 0;
extern "C" int gifHID = 0;

static std::string log_text;

extern "C" void RemoveDmacHandler(int channel, int identifier) {
    log_text += "R" + std::to_string(channel) + ":" + std::to_string(identifier) + ";";
}
extern "C" void DisableDmac(int channel) {
    log_text += "D" + std::to_string(channel) + ";";
}
extern "C" void RemoveIntcHandler(int cause, int identifier) {
    log_text += "I" + std::to_string(cause) + ":" + std::to_string(identifier) + ";";
}
extern "C" void DisableIntc(int cause) {
    log_text += "C" + std::to_string(cause) + ";";
}

int main() {
    vif0HID = 10;
    vif1HID = 11;
    gifHID = 12;
    gsHID = 0;
    log_text.clear();
    drawRemoveInterrupts__Fv();
    if (log_text != "R0:10;D0;R1:11;D1;R2:12;D2;I0:0;C0;") return 1;
    gsHID = 4;
    vif0HID = -1;
    log_text.clear();
    drawRemoveInterrupts__Fv();
    if (log_text != "R0:-1;D0;R1:11;D1;R2:12;D2;I0:4;C0;") return 2;
    std::puts("draw remove interrupts contract passed");
    return 0;
}
