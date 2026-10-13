// Observed: Init__t6VIList1Z7VIActor at 0x01224830, 44 bytes. Calls Clear__t6VIList1Z7VIActor with the same list
// pointer and returns -1 when that result is negative, else 0.
extern "C" {
int Clear__t6VIList1Z7VIActor(void* list);

int Init__t6VIList1Z7VIActor(void* list) {
    return Clear__t6VIList1Z7VIActor(list) < 0 ? -1 : 0;
}
}
