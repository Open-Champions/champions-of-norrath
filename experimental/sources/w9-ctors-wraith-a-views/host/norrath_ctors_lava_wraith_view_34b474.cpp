// Host comparison only. Not an EE build input.
extern "C" unsigned char skillFireWeapons[] asm("skillFireWeapons");
unsigned char skillFireWeapons[0x68];
extern "C" void* norrath_ctors_lava_wraith_view_34b474;
int main() { return norrath_ctors_lava_wraith_view_34b474 == (void*)skillFireWeapons ? 0 : 1; }
