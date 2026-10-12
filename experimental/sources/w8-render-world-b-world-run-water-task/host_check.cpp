extern "C" {
void* world;
void worldRunWaterTask__Fv();
}
static int calls;
static void* seen;
extern "C" void worldRunWater__FP12_worldHeader(void* header) { ++calls; seen = header; }
int main() {
    world = nullptr;
    worldRunWaterTask__Fv();
    if (calls != 0) return 1;
    int marker;
    world = &marker;
    worldRunWaterTask__Fv();
    if (calls != 1 || seen != &marker) return 2;
    worldRunWaterTask__Fv();
    if (calls != 2) return 3;
    return 0;
}
