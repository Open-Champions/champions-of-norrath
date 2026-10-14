// Host check for msg_run__7Weather. Not an EE plan input. The source lives in
// w12-actor-spell-c-msg-run-7weather; this unit only adds the check it lacked.
// Observed: the original is `jr ra` with a nop, so it must leave memory alone and take no calls.
#include "../w12-actor-spell-c-msg-run-7weather/msg_run.cpp"
int main() {
    unsigned char object[16];
    for (int i = 0; i < 16; ++i) object[i] = static_cast<unsigned char>(i * 7 + 1);
    msg_run__7Weather(object);
    msg_run__7Weather(0);
    for (int i = 0; i < 16; ++i)
        if (object[i] != static_cast<unsigned char>(i * 7 + 1)) return 1;
    return 0;
}
