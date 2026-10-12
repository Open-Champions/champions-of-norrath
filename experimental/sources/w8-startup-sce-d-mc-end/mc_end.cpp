/* Observed sceMcEnd at 0x002e0d60, 76 bytes. When semaidRegFunc (0x00446074)
 * is nonnegative: DeleteSema on it, DeleteSema on semaid_timerfunc
 * (0x00446078), and semaidRegFunc is set to -1 between those two calls.
 * semaid_timerfunc is left as it was. Returns 1 on every path. Both words are
 * named for their other users; what they hold here is not established.
 */
extern "C" int semaidRegFunc;
extern "C" int semaid_timerfunc;
extern "C" int DeleteSema(int id);

extern "C" int sceMcEnd() {
    if (semaidRegFunc >= 0) {
        DeleteSema(semaidRegFunc);
        int second = semaid_timerfunc;
        semaidRegFunc = -1;
        DeleteSema(second);
    }
    return 1;
}
