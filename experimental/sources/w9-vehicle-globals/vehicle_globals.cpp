// Storage for the two Vehicle globals the original names vehicleCount and
// vehicleList. Observed: vehicleCount is a 4-byte word at 0x00485e40 and
// vehicleList is 128 bytes at 0x00436fa8, directly after allVehicleSpecs; both
// are zero in the image. The destructor and constructor treat vehicleList as an
// array of object pointers indexed below vehicleCount, and keep the index in
// the word at object+2156. The array has room for 32 entries and nothing
// checks the count. Nothing here was executed on the EE. The original keeps
// these in initialised data; whether the final link places them there is not
// established by this unit.
extern "C" {
int vehicleCount = 0;
unsigned char* vehicleList[32] = {};
}
