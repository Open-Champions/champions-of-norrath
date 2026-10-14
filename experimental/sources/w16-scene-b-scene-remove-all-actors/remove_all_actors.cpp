// Observed: RemoveAllActors__7VIScene at 0x01145d80 is 116 bytes. With the actor list
// at scene + 972 it takes Begin__Ct6VIList1Z7VIActor(list) and, while that value
// differs from End__Ct6VIList1Z7VIActor(list) (called again every test), calls
// RemoveActor__7VIScenei(scene, value) and then replaces value with
// Next__Ct6VIList1Z7VIActori(list, value). It returns 0.
// Unknown: whether Next after RemoveActor reads the just-removed node; the image does
// exactly this and it is reproduced as is.
extern "C" {
int Begin__Ct6VIList1Z7VIActor(void* list);
int End__Ct6VIList1Z7VIActor(const void* list);
int Next__Ct6VIList1Z7VIActori(void* list, int item);
int RemoveActor__7VIScenei(unsigned char* scene, int actor);

int RemoveAllActors__7VIScene(unsigned char* scene) {
    void* list = scene + 972;
    int item = Begin__Ct6VIList1Z7VIActor(list);
    while (item != End__Ct6VIList1Z7VIActor(list)) {
        RemoveActor__7VIScenei(scene, item);
        item = Next__Ct6VIList1Z7VIActori(list, item);
    }
    return 0;
}
}
