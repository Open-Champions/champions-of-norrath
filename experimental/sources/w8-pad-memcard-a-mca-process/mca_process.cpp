// Observed: MCA_Process__FPi at 0x0025c038 is 788 bytes. It polls
// sceMcSync(1, 0, &MCA_Result); a zero return means a command is still running.
// When none is running and checkResults is nonzero it calls
// serviceResult__FP8queueCmdPi(&cq, error) and zeroes checkResults. If
// commandQueueEmpty__Fv is zero and no command is running it stores 1 in checkResults,
// copies the 16-byte queue entry commandQueue[queueStart] into the local static cq, and
// jumps on the unsigned command word of cq (a 13-entry table):
//   0 file = sceMcOpen(MCA_Port, MCA_Slot, text, third), after storing 1 in MCA_FileOpen
//   1 sceMcClose(file), after storing 0 in MCA_FileOpen
//   2 sceMcSeek(file, text, third)
//   3 and 4 sceMcRead(file, text, third)
//   5 write: a nonzero text word calls sceMcWrite(file, text, third). A zero text word
//     writes zeros from nullBuff in chunks of at most 16384: when clearBytes is -1 it
//     takes third as the total, the chunk is the smaller of the total and 16384, that
//     chunk replaces third in commandQueue[queueStart] and in cq, sceMcWrite(file,
//     nullBuff, chunk) is called, and clearBytes becomes total minus chunk, or -1 when
//     that is not positive.
//   6 sceMcGetDir(MCA_Port, MCA_Slot, text, 0, 64, MCA_DirContents)
//   7 sceMcDelete(MCA_Port, MCA_Slot, text)
//   8 sceMcGetInfo(text, third, &cardType, fourth, 0), the first two words passed as stored
//   9 sceMcFormat(text, third), the first two words passed as stored
//   10 sceMcChdir(MCA_Port, MCA_Slot, text, 0)
//   11 sceMcMkdir(MCA_Port, MCA_Slot, text)
//   12 and any larger value issue nothing.
// After the dispatch, when clearBytes is -1, queueStart advances by one and wraps to 0 at 256.
// The result is 1 when commandQueueEmpty__Fv is zero or a command is still running, else 0.
// Inferred: queue entries are four words: command, two arguments, a fourth word.
// Unknown: the sceMc call signatures beyond their register use; the meaning of the
// fourth word for command 8; what serviceResult does with cq.

struct queueCmd {
    int command;
    const void* text;
    int third;
    int fourth;
};

extern "C" int sceMcSync(int mode, int* cmd, int* result);
extern "C" int sceMcOpen(int port, int slot, const void* name, int mode);
extern "C" int sceMcClose(int fd);
extern "C" int sceMcSeek(int fd, const void* offset, int origin);
extern "C" int sceMcRead(int fd, const void* buffer, int size);
extern "C" int sceMcWrite(int fd, const void* buffer, int size);
extern "C" int sceMcGetDir(int port, int slot, const void* name, int mode, int maxent, void* table);
extern "C" int sceMcDelete(int port, int slot, const void* name);
extern "C" int sceMcGetInfo(const void* port, int slot, int* type, int free, int format);
extern "C" int sceMcFormat(const void* port, int slot);
extern "C" int sceMcChdir(int port, int slot, const void* newdir, int current);
extern "C" int sceMcMkdir(int port, int slot, const void* name);
extern "C" void serviceResult__FP8queueCmdPi(queueCmd* command, int* error);
extern "C" int commandQueueEmpty__Fv();
extern "C" queueCmd commandQueue[256];
extern "C" unsigned char nullBuff[16384];
extern "C" unsigned char MCA_DirContents[4096];
extern "C" int MCA_Port;
extern "C" int MCA_Slot;
extern "C" int MCA_FileOpen;
extern "C" int MCA_Result;
extern "C" int cardType;
extern "C" int queueStart;
extern "C" int checkResults;
extern "C" int clearBytes;

namespace {
int file;
}

extern "C" int MCA_Process__FPi(int* error) {
    static queueCmd cq;
    int running = sceMcSync(1, 0, &MCA_Result) == 0;
    if (!running && checkResults != 0) {
        serviceResult__FP8queueCmdPi(&cq, error);
        checkResults = 0;
    }
    if (commandQueueEmpty__Fv() == 0 && !running) {
        checkResults = 1;
        cq = commandQueue[queueStart];
        switch (static_cast<unsigned>(cq.command)) {
        case 0:
            MCA_FileOpen = 1;
            file = sceMcOpen(MCA_Port, MCA_Slot, cq.text, cq.third);
            break;
        case 1:
            MCA_FileOpen = 0;
            sceMcClose(file);
            break;
        case 2:
            sceMcSeek(file, cq.text, cq.third);
            break;
        case 3:
        case 4:
            sceMcRead(file, cq.text, cq.third);
            break;
        case 5:
            if (cq.text == 0) {
                int total = clearBytes;
                if (total == -1) {
                    total = cq.third;
                    clearBytes = total;
                }
                int chunk = total < 16384 ? total : 16384;
                commandQueue[queueStart].third = chunk;
                cq.third = chunk;
                sceMcWrite(file, nullBuff, chunk);
                int left = clearBytes - chunk;
                clearBytes = left > 0 ? left : -1;
            } else {
                sceMcWrite(file, cq.text, cq.third);
            }
            break;
        case 6:
            sceMcGetDir(MCA_Port, MCA_Slot, cq.text, 0, 64, MCA_DirContents);
            break;
        case 7:
            sceMcDelete(MCA_Port, MCA_Slot, cq.text);
            break;
        case 8:
            sceMcGetInfo(cq.text, cq.third, &cardType, cq.fourth, 0);
            break;
        case 9:
            sceMcFormat(cq.text, cq.third);
            break;
        case 10:
            sceMcChdir(MCA_Port, MCA_Slot, cq.text, 0);
            break;
        case 11:
            sceMcMkdir(MCA_Port, MCA_Slot, cq.text);
            break;
        default:
            break;
        }
        if (clearBytes == -1) {
            int next = queueStart + 1;
            queueStart = next == 256 ? 0 : next;
        }
    }
    return commandQueueEmpty__Fv() == 0 || running;
}
