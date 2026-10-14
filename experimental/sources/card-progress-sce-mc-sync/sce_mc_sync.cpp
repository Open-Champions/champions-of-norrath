// Observed: sceMcSync returns -1 when mcRunCmdNo is zero, without calling a
// semaphore or writing either pointer. Otherwise it polls semaid_timerfunc.
// A zero mode waits on that same semaphore when the poll is negative, and
// ignores the wait result. A nonzero mode does not wait. A non-null command
// pointer receives mcRunCmdNo. A negative poll on a nonzero mode returns 0
// and leaves mcRunCmdNo unchanged. A completed call stores zero into
// mcRunCmdNo, stores retval through a non-null result pointer, signals
// semaidRegFunc, and returns 1.
// Inferred: none.
// Unknown: which IOP command publishes retval.

extern "C" int mcRunCmdNo;
extern "C" int semaidRegFunc;
extern "C" int semaid_timerfunc;
extern "C" int retval;

extern "C" int PollSema(int id);
extern "C" int WaitSema(int id);
extern "C" int SignalSema(int id);

extern "C" int sceMcSync(int mode, int* command, int* result) {
    if (mcRunCmdNo == 0) {
        return -1;
    }
    int polled = PollSema(semaid_timerfunc);
    int ready = polled >= 0 ? 1 : 0;
    if (mode == 0 && ready == 0) {
        WaitSema(semaid_timerfunc);
        ready = 1;
    }
    if (command != 0) {
        *command = mcRunCmdNo;
    }
    if (ready == 0) {
        return 0;
    }
    mcRunCmdNo = 0;
    if (result != 0) {
        *result = retval;
    }
    SignalSema(semaidRegFunc);
    return 1;
}
