#include <cstring>
#include <string>

extern "C" void msg_run__4Gold(void* object);
extern "C" int frameNm = 100;

static std::string log_text;
static std::string lump, name;
static void* sequence_seen;
static void* sound_object;
static float sound_volume;
static void* anim_state;
static void* anim_point;
static void* anim_second;
static void* anim_third;
static void* removed;
static unsigned char byte76_at_remove;
static char sequence_token;

extern "C" char* lumpFindResource__FPcT0(const char* l, const char* n) {
    lump = l; name = n;
    log_text += "L";
    return &sequence_token;
}
extern "C" void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sequence, void* object, float volume) {
    log_text += "S";
    sequence_seen = sequence; sound_object = object; sound_volume = volume;
}
extern "C" void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* s, void* p, void* b, void* c) {
    log_text += "A";
    anim_state = s; anim_point = p; anim_second = b; anim_third = c;
}
extern "C" void objectRemoveFromRunList__FP10GameObject(void* object) {
    log_text += "R";
    removed = object;
    byte76_at_remove = static_cast<unsigned char*>(object)[76];
}

int main() {
    unsigned char object[192];
    std::memset(object, 0, sizeof object);
    unsigned stamp = 82;                       // frameNm - 18
    std::memcpy(object + 80, &stamp, 4);
    msg_run__4Gold(object);
    if (log_text != "LSA") return 1;
    if (lump != "fx.lmp" || name != "inv_drop_coin.anm") return 2;
    if (sequence_seen != &sequence_token || sound_object != object || sound_volume != 1.0f) return 3;
    if (anim_state != object + 88 || anim_point != object + 16 || anim_second || anim_third) return 4;
    // Another stamp: animation only. The halfword flag removes the object, byte first.
    log_text.clear();
    stamp = 83;
    std::memcpy(object + 80, &stamp, 4);
    msg_run__4Gold(object);
    if (log_text != "A") return 5;
    object[94] = 0x80;
    log_text.clear();
    msg_run__4Gold(object);
    if (log_text != "AR" || removed != object || byte76_at_remove != 1 || object[76] != 1) return 6;
    // Other bits of the halfword do not count; bit 7 of the high byte is bit 15.
    object[76] = 0; object[94] = 0x7f; object[95] = 0xff;
    log_text.clear();
    msg_run__4Gold(object);
    if (log_text != "A" || object[76] != 0) return 7;
    // A non-zero byte at +76 (negative too) stops everything.
    object[76] = 0xff; object[94] = 0x80;
    stamp = 82;
    std::memcpy(object + 80, &stamp, 4);
    log_text.clear();
    msg_run__4Gold(object);
    if (!log_text.empty()) return 8;
    // The compare is with frameNm - 18 at the time of the call.
    object[76] = 0; object[94] = 0; frameNm = 118;
    stamp = 100;
    std::memcpy(object + 80, &stamp, 4);
    msg_run__4Gold(object);
    if (log_text != "LSA") return 9;
    return 0;
}
