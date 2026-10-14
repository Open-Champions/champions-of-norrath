// Observed: UpdateAllActors__7VIScene at 0x01145df8 is 132 bytes. With the actor list
// at scene + 972 it takes Begin__Ct6VIList1Z7VIActor(list) and, while that value
// differs from End__Ct6VIList1Z7VIActor(list) (called again every test), calls
// UpdateActor__7VIScenei(scene, value). A negative result returns -1 at once;
// otherwise the value is replaced by Next__Ct6VIList1Z7VIActori(list, value). It
// returns 0 when the walk finishes.
extern "C" {
int Begin__Ct6VIList1Z7VIActor(void* list);
int End__Ct6VIList1Z7VIActor(const void* list);
int Next__Ct6VIList1Z7VIActori(void* list, int item);
int UpdateActor__7VIScenei(unsigned char* scene, int actor);

int UpdateAllActors__7VIScene(unsigned char* scene) {
    void* list = scene + 972;
    int item = Begin__Ct6VIList1Z7VIActor(list);
    while (item != End__Ct6VIList1Z7VIActor(list)) {
        if (UpdateActor__7VIScenei(scene, item) < 0)
            return -1;
        item = Next__Ct6VIList1Z7VIActori(list, item);
    }
    return 0;
}
}
