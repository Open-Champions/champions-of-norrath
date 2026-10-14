#include "mca_init.cpp"

#include <cstdio>

static int clear_calls;
static int memset_calls;
static void* memset_destination;
static int memset_value;
static unsigned memset_size;
static int file_open_at_clear;

extern "C" int MCA_Port = 0;
extern "C" int MCA_Slot = 0;
extern "C" int MCA_FileOpen = 0;

extern "C" void MCA_ClearInfo__Fv() {
    ++clear_calls;
    file_open_at_clear = MCA_FileOpen;
}

extern "C" void* memset(void* destination, int value, unsigned size) {
    ++memset_calls;
    memset_destination = destination;
    memset_value = value;
    memset_size = size;
    auto* bytes = static_cast<unsigned char*>(destination);
    for (unsigned index = 0; index < size; ++index) bytes[index] = static_cast<unsigned char>(value);
    return destination;
}

int main() {
    clearBytes = 4;
    queueEnd = 9;
    queueStart = 8;
    checkResults = 7;
    lastDirCount = 6;
    MCA_Port = 3;
    MCA_Slot = 2;
    MCA_FileOpen = 1;
    for (unsigned index = 0; index < sizeof nullBuff; ++index) nullBuff[index] = 0xa5;
    MCA_Init__Fv();
    if (clearBytes != -1 || queueEnd != 0 || queueStart != 0 || checkResults != 0 ||
        lastDirCount != 0) return 1;
    if (MCA_Port != 0 || MCA_Slot != 0 || MCA_FileOpen != 0) return 2;
    if (clear_calls != 1 || file_open_at_clear != 0) return 3;
    if (memset_calls != 1 || memset_destination != nullBuff || memset_value != 0 ||
        memset_size != 16384u) return 4;
    for (unsigned index = 0; index < sizeof nullBuff; ++index) {
        if (nullBuff[index] != 0) return 5;
    }
    std::puts("mca init passed");
    return 0;
}
