// Observed SetTime__7VIScenel at 0x011449f8, 12 bytes. It stores the 64-bit
// argument at offset 1152 of the scene and returns zero. The meaning of the
// stored value and the layout of VIScene are not established here.
extern "C" {
struct VIScene;
int SetTime__7VIScenel(VIScene* scene, long long time) {
    *reinterpret_cast<long long*>(reinterpret_cast<char*>(scene) + 1152) = time;
    return 0;
}
}
