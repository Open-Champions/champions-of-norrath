extern "C" short lerpShortRot__Fssfff(short, short, float, float, float);

static int check(short from, short to, float r, float lo, float hi, short want) {
    return lerpShortRot__Fssfff(from, to, r, lo, hi) == want ? 0 : 1;
}

int main() {
    int bad = 0;
    bad += check(100, 0, 0.5f, -1.f, -1.f, 50);    // moves down by half the gap
    bad += check(0, 100, 0.5f, -1.f, -1.f, 50);    // moves up by half the gap
    bad += check(100, 0, 0.5f, 80.f, -1.f, 20);    // lo raises the step to 80
    bad += check(100, 0, 0.5f, -1.f, 10.f, 90);    // hi lowers the step to 10
    bad += check(100, 0, 0.5f, 5.f, 10.f, 90);     // step 50 within lo, capped by hi
    bad += check(100, 0, 0.5f, 60.f, 10.f, 90);    // lo then hi: hi wins
    bad += check(7, 7, 1.0f, -1.f, -1.f, 7);      // equal: no movement
    bad += check(7, 7, 1.0f, 3.f, -1.f, 10);      // equal with lo: sign is +, so +3
    bad += check(100, 0, 1.0f, -1.f, -1.f, 0);     // full step lands on target
    bad += check(100, 0, 2.0f, -1.f, -1.f, -100);  // overshoot is not clamped
    bad += check(100, 0, 0.99f, -1.f, -1.f, 1);    // truncation: 99.0 -> 99
    bad += check(-32768, 32767, 0.5f, -1.f, -1.f, -32768); // wrapped diff = 1, step 0.5 truncates to 0
    bad += check(32767, -32768, 1.0f, -1.f, -1.f, -32768); // diff wraps to -1, so +1 and the sum wraps
    bad += check(0, -32768, 1.0f, -1.f, -1.f, -32768); // diff -32768, magnitude 32768, sign +
    return bad;
}
