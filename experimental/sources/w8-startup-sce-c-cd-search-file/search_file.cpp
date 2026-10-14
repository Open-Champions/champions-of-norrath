// Observed sceCdSearchFile at 0x002e2cb0, 28 bytes. It forwards both arguments
// to sceCdLayerSearchFile with a third argument of zero and returns its result.
extern "C" {
int sceCdLayerSearchFile(void* file, const char* name, int layer);
int sceCdSearchFile(void* file, const char* name) {
    return sceCdLayerSearchFile(file, name, 0);
}
}
