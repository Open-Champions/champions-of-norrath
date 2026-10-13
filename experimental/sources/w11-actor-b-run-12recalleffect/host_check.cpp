// Host contract for msg_run__12RecallEffect; not an EE input. Includes the source.
#include "msg_run.cpp"
#include <cstring>
int main() {
    unsigned char object[64];
    std::memset(object, 0x3c, sizeof object);
    msg_run__12RecallEffect(object);
    for (unsigned char b : object) if (b != 0x3c) return 1;
    return 0;
}
