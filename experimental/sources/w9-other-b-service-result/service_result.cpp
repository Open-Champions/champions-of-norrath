// Observed: serviceResult__FP8queueCmdPi at 0x0025c350 is 740 bytes. Its only caller is
// MCA_Process__FPi, which passes the copy of the queue entry it has just completed and its
// own error pointer. It first stores a pointer to an empty string (the zero word at
// 0x00485bf8) in MCA_ErrorMsg, then switches on the unsigned command word; values above 12
// do nothing more. MCA_Result is the result of the finished memory card call. "Fail" below
// means: store the result through the error pointer and set queueStart to queueEnd.
//   0  a result of zero ends. Otherwise MCA_FileOpen becomes 0; results -5 and -4 end
//      without failing, any other nonzero result fails.
//   1  a nonzero result fails when below -9 or equal to -3.
//   2, 4, 7  results >= 0 end. Negative results fail when below -9 or equal to -3.
//   3, 5  a result different from the third word sets MCA_FileOpen to 0, points
//      MCA_ErrorMsg at the string "Save Data Is Corrupt." and fails. For command 5 an equal
//      result ends; for command 3 an equal result is handled like command 4.
//   6  a negative result is handled like command 4; otherwise it is stored in MCA_LastDirCount.
//   8  Present[second*4+third] is cleared first. When cardType is 2 and the third word is
//      below scePadGetSlotMax(second) the stored result is used: -1 sets Formatted for that
//      entry, -2 clears it, and any result of -9 or above (including those) sets Present.
//      A third word not below the slot maximum stores -10 in MCA_Result; so does any other
//      card type. Finally, if Present for the entry is still zero, Formatted is cleared.
//   9  a zero result sets Formatted[second*4+third] to 1.
//   10, 11  nothing.
//   12  queueStart becomes queueEnd, an open file is closed and MCA_FileOpen cleared, then the
//       second word becomes both MCA_Result and the error value and queueStart is set again.
// Inferred: the entry is the four-word queue entry of MCA_Process (command, port-like second
// word, slot-like third word, fourth word); Formatted and Present are 8-entry tables
// indexed port*4+slot. The error pointer receives a status that the caller's caller reads.
// Unknown: the names of the sceMc result codes, who reads MCA_ErrorMsg, and the exact meaning
// of the file word (the original's `file` at 0x00486ce4, file-scope in MCA_Process's unit)
// beyond being passed to sceMcClose. It is imported here under its symbol name.

struct queueCmd {
    int command;
    int port;
    int slot;
    int fourth;
};

extern "C" int scePadGetSlotMax(int port);
extern "C" int sceMcClose(int fd);
extern "C" const char* MCA_ErrorMsg;
extern "C" int MCA_Result;
extern "C" int MCA_FileOpen;
extern "C" int MCA_LastDirCount;
extern "C" int MCA_Formatted[8];
extern "C" int MCA_Present[8];
extern "C" int cardType;
extern "C" int queueStart;
extern "C" int queueEnd;
extern "C" int file;

namespace {
inline void fail(int* error, int result) {
    *error = result;
    queueStart = queueEnd;
}

inline void checkNegative(int* error, int result) {
    if (result < -9 || result == -3) fail(error, result);
}

inline void checkSigned(int* error, int result) {
    if (result < 0) checkNegative(error, result);
}

inline void corrupt(int* error, int result) {
    MCA_FileOpen = 0;
    MCA_ErrorMsg = "Save Data Is Corrupt.";
    fail(error, result);
}
}

extern "C" void serviceResult__FP8queueCmdPi(queueCmd* command, int* error) {
    MCA_ErrorMsg = "";
    switch (static_cast<unsigned>(command->command)) {
    case 0: {
        int result = MCA_Result;
        if (result == 0) return;
        MCA_FileOpen = 0;
        if (result == -5 || result == -4) return;
        fail(error, result);
        return;
    }
    case 1: {
        int result = MCA_Result;
        if (result != 0) checkNegative(error, result);
        return;
    }
    case 2:
    case 4:
    case 7:
        checkSigned(error, MCA_Result);
        return;
    case 3: {
        int result = MCA_Result;
        if (result != command->slot) {
            corrupt(error, result);
            return;
        }
        checkSigned(error, result);
        return;
    }
    case 5: {
        int result = MCA_Result;
        if (result != command->slot) corrupt(error, result);
        return;
    }
    case 6: {
        int result = MCA_Result;
        if (result < 0) {
            checkNegative(error, result);
            return;
        }
        MCA_LastDirCount = result;
        return;
    }
    case 8: {
        int index = command->port * 4 + command->slot;
        MCA_Present[index] = 0;
        if (cardType == 2) {
            if (!(command->slot < scePadGetSlotMax(command->port))) {
                MCA_Result = -10;
            } else {
                int result = MCA_Result;
                if (result == -1) MCA_Formatted[index] = 1;
                else if (result == -2) MCA_Formatted[index] = 0;
                if (result >= -9) MCA_Present[index] = 1;
            }
        } else {
            MCA_Result = -10;
        }
        if (MCA_Present[index] == 0) MCA_Formatted[index] = 0;
        return;
    }
    case 9:
        if (MCA_Result == 0) MCA_Formatted[command->port * 4 + command->slot] = 1;
        return;
    case 12: {
        queueStart = queueEnd;
        if (MCA_FileOpen != 0) {
            sceMcClose(file);
            MCA_FileOpen = 0;
        }
        int value = command->port;
        MCA_Result = value;
        fail(error, value);
        return;
    }
    default:
        return;
    }
}
