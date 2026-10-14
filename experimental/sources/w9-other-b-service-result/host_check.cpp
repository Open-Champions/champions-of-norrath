// Host check for service_result.cpp; not an EE plan input.
#include <cstring>
extern "C" {
const char* MCA_ErrorMsg;
int MCA_Result, MCA_FileOpen, MCA_LastDirCount, MCA_Formatted[8], MCA_Present[8], cardType, queueStart, queueEnd, file;
int slotMax = 2, closed = -1;
int scePadGetSlotMax(int) { return slotMax; }
int sceMcClose(int fd) { closed = fd; return 0; }
}
#include "service_result.cpp"

static int fails;
#define CHECK(c) do { if (!(c)) { ++fails; } } while (0)

static void reset() {
    MCA_Result = 0; MCA_FileOpen = 1; MCA_LastDirCount = 9; cardType = 0; queueStart = 3; queueEnd = 8; file = 5; closed = -1;
    for (int i = 0; i < 8; ++i) { MCA_Formatted[i] = 7; MCA_Present[i] = 7; }
}

int main() {
    queueCmd c{};
    int err = 111;
    reset(); c.command = 0; MCA_Result = -4; serviceResult__FP8queueCmdPi(&c, &err);
    CHECK(MCA_FileOpen == 0 && err == 111 && queueStart == 3 && *MCA_ErrorMsg == 0);
    reset(); c.command = 0; MCA_Result = -6; serviceResult__FP8queueCmdPi(&c, &err);
    CHECK(err == -6 && queueStart == 8);
    reset(); c.command = 1; MCA_Result = -9; err = 111; serviceResult__FP8queueCmdPi(&c, &err);
    CHECK(err == 111); MCA_Result = -10; serviceResult__FP8queueCmdPi(&c, &err); CHECK(err == -10);
    reset(); c.command = 5; c.slot = 100; MCA_Result = 99; serviceResult__FP8queueCmdPi(&c, &err);
    CHECK(std::strcmp(MCA_ErrorMsg, "Save Data Is Corrupt.") == 0 && MCA_FileOpen == 0 && err == 99 && queueStart == 8);
    reset(); c.command = 6; MCA_Result = 4; serviceResult__FP8queueCmdPi(&c, &err); CHECK(MCA_LastDirCount == 4);
    reset(); c.command = 9; c.port = 1; c.slot = 2; serviceResult__FP8queueCmdPi(&c, &err); CHECK(MCA_Formatted[6] == 1);
    reset(); c.command = 8; c.port = 1; c.slot = 1; cardType = 2; MCA_Result = -1; serviceResult__FP8queueCmdPi(&c, &err);
    CHECK(MCA_Formatted[5] == 1 && MCA_Present[5] == 1);
    reset(); c.command = 8; c.port = 1; c.slot = 3; cardType = 2; serviceResult__FP8queueCmdPi(&c, &err);
    CHECK(MCA_Result == -10 && MCA_Present[7] == 0 && MCA_Formatted[7] == 0);
    reset(); c.command = 12; c.port = -3; serviceResult__FP8queueCmdPi(&c, &err);
    CHECK(closed == 5 && MCA_FileOpen == 0 && MCA_Result == -3 && err == -3 && queueStart == 8);
    reset(); c.command = 13; err = 111; serviceResult__FP8queueCmdPi(&c, &err); CHECK(err == 111 && queueStart == 3);
    return fails;
}
