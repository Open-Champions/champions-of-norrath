// Observed worldPlotRouteProcess__Fi at 0x0017d500, 184 bytes. The argument is unused.
// It does nothing on odd values of frameNm. Otherwise, with rqHead and rqTail (ints),
// it returns when they are equal. The queue is routeQueue, 128 entries of 48 bytes.
// When the entry at rqTail has a zero word at offset 40, the tail is advanced (mod 128)
// to the next entry with a nonzero word at offset 40; reaching rqHead stores that
// index to rqTail and returns without plotting. Whenever the scan moved the tail,
// the new tail is stored in rqTail. If an entry with a nonzero word was current or
// found, routePlot__FR10RouteQueue is called with that entry's address (rqTail is not
// advanced past it here). Nothing is returned.
// Inferred: the offset-40 word marks a pending route. Unknown: the queue entry layout
// beyond that word, and who clears the word and advances the tail after plotting.
extern int frameNm;
extern int rqHead;
extern int rqTail;
extern unsigned char routeQueue[];
extern "C" void routePlot__FR10RouteQueue(void* entry);

extern "C" void worldPlotRouteProcess__Fi(int) {
    if (frameNm & 1) return;
    int tail = rqTail;
    const int head = rqHead;
    if (head == tail) return;
    if (*reinterpret_cast<int*>(routeQueue + tail * 48 + 40) == 0) {
        for (;;) {
            tail = (tail + 1) & 0x7f;
            if (head == tail) {
                rqTail = tail;
                return;
            }
            if (*reinterpret_cast<int*>(routeQueue + tail * 48 + 40) != 0) break;
        }
        rqTail = tail;
    }
    if (head == tail) return;
    routePlot__FR10RouteQueue(routeQueue + tail * 48);
}
