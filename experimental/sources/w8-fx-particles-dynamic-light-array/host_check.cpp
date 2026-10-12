// Host contract for DynamicLightArray. Not an EE plan input.
extern "C" unsigned char DynamicLightArray[16 * 68];
int main() {
    if (sizeof DynamicLightArray != 1088u) return 1;
    for (unsigned i = 0; i < sizeof DynamicLightArray; ++i) {
        if (DynamicLightArray[i] != 0) return 2;
    }
    return 0;
}
