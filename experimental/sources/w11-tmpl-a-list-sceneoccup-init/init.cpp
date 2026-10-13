// Observed: Init__t6VIList1ZQ27VIScene12VISceneOccup at 0x01224860, 44 bytes. Calls Clear__t6VIList1ZQ27VIScene12VISceneOccup with the same list
// pointer and returns -1 when that result is negative, else 0.
extern "C" {
int Clear__t6VIList1ZQ27VIScene12VISceneOccup(void* list);

int Init__t6VIList1ZQ27VIScene12VISceneOccup(void* list) {
    return Clear__t6VIList1ZQ27VIScene12VISceneOccup(list) < 0 ? -1 : 0;
}
}
