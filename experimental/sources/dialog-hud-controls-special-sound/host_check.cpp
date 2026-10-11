extern "C" void specialSoundHandler__FiG6Point3(int code, const unsigned char* point);

static int calls;
static const unsigned char* got_point;
static unsigned char got_bytes[12];
static int got_a, got_b, got_c;

extern "C" void gameFootstepSound__FG6Point3iii(const unsigned char* point, int a, int b, int c) {
    calls += 1;
    got_point = point;
    for (int i = 0; i < 12; ++i) got_bytes[i] = point[i];
    got_a = a;
    got_b = b;
    got_c = c;
}

int main() {
    unsigned char point[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    specialSoundHandler__FiG6Point3(0, point);
    specialSoundHandler__FiG6Point3(-1, point);
    specialSoundHandler__FiG6Point3(-3, point);
    if (calls != 0) return 1;
    specialSoundHandler__FiG6Point3(-2, point);
    if (calls != 1 || got_a != 0 || got_b != 0 || got_c != 0) return 2;
    if (got_point == point) return 3;
    for (int i = 0; i < 12; ++i) {
        if (got_bytes[i] != point[i]) return 4;
    }
    point[0] = 99;
    if (got_bytes[0] != 1) return 5;
    return 0;
}
