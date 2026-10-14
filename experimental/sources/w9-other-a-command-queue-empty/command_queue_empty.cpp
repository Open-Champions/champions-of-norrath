// Observed commandQueueEmpty__Fv at 0x0025c8d0, 20 bytes. Compares the words
// queueStart (0x00486cd0) and queueEnd (0x00486cd4) and returns 1 when equal,
// otherwise 0. Inferred: a ring or list with these two cursors. Unknown: the
// queue element type and the storage owner of both words.
extern "C" {
extern int queueStart;
extern int queueEnd;
int commandQueueEmpty__Fv() {
    return queueStart == queueEnd;
}
}
