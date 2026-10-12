// Observed Kick__7Vehicle at 0x00277b88, 108 bytes.
// Compares the C string at levelName+81 (levelName is the data symbol at
// 0x0034b660, so this address is 0x0034b6b1) with the literal "lava2" using
// strcmp. When they are equal it calls removePassenger__7Vehiclei with this
// vehicle and the slots 0, 1, 2 and 3, in that order. Otherwise it does
// nothing. Nothing is returned. Nothing here was executed on the EE.
extern "C" {
extern char levelName[];
int strcmp(const char* a, const char* b);
void removePassenger__7Vehiclei(void* vehicle, int slot);
}

extern "C" void Kick__7Vehicle(void* vehicle) {
    if (strcmp(levelName + 81, "lava2") == 0) {
        removePassenger__7Vehiclei(vehicle, 0);
        removePassenger__7Vehiclei(vehicle, 1);
        removePassenger__7Vehiclei(vehicle, 2);
        removePassenger__7Vehiclei(vehicle, 3);
    }
}
