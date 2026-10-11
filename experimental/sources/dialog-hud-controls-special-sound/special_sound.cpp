// Observed specialSoundHandler__FiG6Point3 at 0x001d0d80, 72 bytes.
// The second argument arrives as the address of 12 bytes. Those bytes are
// copied before the compare. When the first argument is -2, the copy is
// passed to gameFootstepSound__FG6Point3iii with the three integers 0.
// Any other code returns after the copy. gameFootstepSound is not defined
// here.
extern "C" void gameFootstepSound__FG6Point3iii(const unsigned char* point, int a, int b, int c);

extern "C" void specialSoundHandler__FiG6Point3(int code, const unsigned char* point) {
    unsigned char local[12];
    for (int i = 0; i < 12; ++i) local[i] = point[i];
    if (code == -2) gameFootstepSound__FG6Point3iii(local, 0, 0, 0);
}
