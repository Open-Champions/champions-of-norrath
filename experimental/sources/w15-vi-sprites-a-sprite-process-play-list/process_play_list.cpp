// Observed: ProcessPlayList__9VIHSpriteiR7VIScene at 0x01061df0, 2220 bytes. Arguments (this, frame, scene). Three local poses t1, t2, t3 are kept, each
// eight floats: a quaternion (x, y, z, w) starting at (0, 0, 0, 1), a scale starting at 1 and a three-float position starting at 0. 32-bit words unless noted.
//  1. When GetTime__7VIScene(scene) equals the 64-bit value at this+184 it returns 0. Otherwise it stores a fresh GetTime result there and walks the play
//     pool embedded at this+120 (Begin, then Next, until End, which is asked afresh each step); a play record is 104 bytes at (pool word +8) + 104*position.
//  2. For each play: animation = Animation__7VIScenei(scene, record word +0); then ProcessPlayTiming__9VIHSpriteiR7VIScene(this, position, scene), and a
//     negative result returns -1. The record is then re-read: words +44 (first key) and +48 (second key), floats +32 (w) and +56 (t), the count at +80 and
//     the first node position at +84. A count of zero or less does no node work for the play.
//  3. For each of count nodes of the node pool at this+144 (the next one comes from Next__Ct6VIPool1Z17VIHSpritePlayNodei(pool, position)): the node is the
//     52-byte record at (pool word +8) + 52*position; its word +4 picks a 256-byte entry n of the table whose base is the word at (this+64)+8, with word
//     +12 of that entry compared (signed) with the frame argument.
//       a. When frame >= entry word +12: copy the 32 bytes at entry+224 (quaternion, scale, position) to entry+16 (quaternion), +32 (scale), +36 (position).
//       b. Otherwise, with k = node word +8, the pair of keys (t1, t2) is: when node word +0 is 2, both are the pose stored in the node at +12 (quaternion),
//          +28 (scale), +32 (position); else when the animation's word +8 is zero, t1 and t2 are 32-byte records number k+first and k+second in the table at
//          animation word +40; else they are 16-byte compact records number k+first and k+second in that table: four signed shorts scaled by 3.0518509e-5
//          (0x38000100) are the quaternion, then four shorts at record +8 scaled by 1/512 are the scale and the three position components. If node word
//          +0 was 1 it becomes 2 and t1 is stored into the node as above.
//       c. If the play record word +8 is zero t3 = t1, otherwise t3 = (Slerp(t1.q, t2.q, t), scale and position interpolated linearly with t).
//       d. If the play record word +12 is 0, t3 is written to the table entry's +16/+32/+36; if it is 1 it is first blended with the entry's current pose,
//          the quaternion by Slerp(entry quaternion, t3 quaternion, w) and scale and position linearly with w, and the result is written the same way;
//          any other value writes nothing.
//     It returns 0 at the end.
// Inferred: this is an animation sampler blending keyframe pairs of a hierarchical sprite into per-node poses; the name "frame" for the second argument is
// from the comparison only.
// Unknown: what the record words +8 and +12 select by name, the unit of the frame argument, and what happens in the other value cases of node word +0.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
long long GetTime__7VIScene(void* scene);
int Begin__Ct6VIPool1Z13VIHSpritePlay(void* pool);
int End__Ct6VIPool1Z13VIHSpritePlay(void* pool);
int Next__Ct6VIPool1Z13VIHSpritePlayi(void* pool, int position);
int Next__Ct6VIPool1Z17VIHSpritePlayNodei(void* pool, int position);
unsigned char* Animation__7VIScenei(void* scene, int id);
int ProcessPlayTiming__9VIHSpriteiR7VIScene(void* object, int position, void* scene);
void Slerp__6VIQuatRC6VIQuatT1f(float* out, const float* a, const float* b, float t);

static unsigned char* at(unsigned char* base, int offset) { return NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(base + offset)); }
static int word(const unsigned char* base, int offset) { return *reinterpret_cast<const int*>(base + offset); }
static float flt(const unsigned char* base, int offset) { return *reinterpret_cast<const float*>(base + offset); }

static void copyPose(float* to, const float* from) {
    for (int i = 0; i < 8; ++i) to[i] = from[i];
}
static void identity(float* pose) {
    pose[0] = 0.0f; pose[1] = 0.0f; pose[2] = 0.0f; pose[3] = 1.0f; pose[4] = 1.0f; pose[5] = 0.0f; pose[6] = 0.0f; pose[7] = 0.0f;
}

int ProcessPlayList__9VIHSpriteiR7VIScene(void* object, int frame, void* scene) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    float t1[8], t2[8], t3[8];
    identity(t1);
    identity(t2);
    identity(t3);
    if (GetTime__7VIScene(scene) == *reinterpret_cast<long long*>(bytes + 184)) return 0;
    *reinterpret_cast<long long*>(bytes + 184) = GetTime__7VIScene(scene);
    void* plays = bytes + 120;
    void* nodes = bytes + 144;
    for (int play = Begin__Ct6VIPool1Z13VIHSpritePlay(plays); play != End__Ct6VIPool1Z13VIHSpritePlay(plays); play = Next__Ct6VIPool1Z13VIHSpritePlayi(plays, play)) {
        unsigned char* animation = Animation__7VIScenei(scene, word(at(bytes, 120 + 8) + 104 * play, 0));
        if (ProcessPlayTiming__9VIHSpriteiR7VIScene(object, play, scene) < 0) return -1;
        unsigned char* record = at(bytes, 120 + 8) + 104 * play;
        int firstKey = word(record, 44);
        int secondKey = word(record, 48);
        float weight = flt(record, 32);
        float blend = flt(record, 56);
        int count = word(record, 80);
        int position = word(record, 84);
        for (; count > 0; --count, position = Next__Ct6VIPool1Z17VIHSpritePlayNodei(nodes, position)) {
            unsigned char* node = at(bytes, 144 + 8) + 52 * position;
            unsigned char* entry = at(bytes, 64 + 8) + (static_cast<unsigned>(word(node, 4)) << 8);
            if (!(frame < word(entry, 12))) {
                float* quat = reinterpret_cast<float*>(entry + 16);
                const float* source = reinterpret_cast<const float*>(entry + 224);
                for (int i = 0; i < 4; ++i) quat[i] = source[i];
                *reinterpret_cast<float*>(entry + 32) = source[4];
                for (int i = 0; i < 3; ++i) reinterpret_cast<float*>(entry + 36)[i] = source[5 + i];
                continue;
            }
            int k = word(node, 8);
            if (word(node, 0) == 2) {
                const float* q = reinterpret_cast<const float*>(node + 12);
                for (int i = 0; i < 4; ++i) t1[i] = q[i];
                t1[4] = flt(node, 28);
                for (int i = 0; i < 3; ++i) t1[5 + i] = reinterpret_cast<const float*>(node + 32)[i];
                copyPose(t2, t1);
            } else if (word(animation, 8) == 0) {
                const float* a = reinterpret_cast<const float*>(at(animation, 40) + ((k + firstKey) << 5));
                const float* b = reinterpret_cast<const float*>(at(animation, 40) + ((k + secondKey) << 5));
                copyPose(t1, a);
                copyPose(t2, b);
            } else {
                const short* a = reinterpret_cast<const short*>(at(animation, 40) + ((k + firstKey) << 4));
                const short* b = reinterpret_cast<const short*>(at(animation, 40) + ((k + secondKey) << 4));
                const float angle = 0x1.0002p-15f;
                const float length = 0x1p-9f;
                for (int i = 0; i < 4; ++i) t1[i] = static_cast<float>(a[i]) * angle;
                for (int i = 0; i < 4; ++i) t1[4 + i] = static_cast<float>(a[4 + i]) * length;
                for (int i = 0; i < 4; ++i) t2[i] = static_cast<float>(b[i]) * angle;
                for (int i = 0; i < 4; ++i) t2[4 + i] = static_cast<float>(b[4 + i]) * length;
            }
            unsigned char* again = at(bytes, 144 + 8) + 52 * position;
            if (word(again, 0) == 1) {
                *reinterpret_cast<int*>(again) = 2;
                float* q = reinterpret_cast<float*>(again + 12);
                for (int i = 0; i < 4; ++i) q[i] = t1[i];
                *reinterpret_cast<float*>(again + 28) = t1[4];
                for (int i = 0; i < 3; ++i) reinterpret_cast<float*>(again + 32)[i] = t1[5 + i];
            }
            if (word(at(bytes, 120 + 8) + 104 * play, 8) == 0) {
                copyPose(t3, t1);
            } else {
                Slerp__6VIQuatRC6VIQuatT1f(t3, t1, t2, blend);
                t3[4] = t1[4] + (t2[4] - t1[4]) * blend;
                t3[5] = t2[5];
                t3[6] = t2[6];
                t3[7] = t2[7];
                t3[5] = t3[5] - t1[5];
                t3[6] = t3[6] - t1[6];
                t3[7] = t3[7] - t1[7];
                t3[5] = t3[5] * blend;
                t3[6] = t3[6] * blend;
                t3[7] = t3[7] * blend;
                t3[5] = t3[5] + t1[5];
                t3[6] = t3[6] + t1[6];
                t3[7] = t3[7] + t1[7];
            }
            int mode = word(at(bytes, 120 + 8) + 104 * play, 12);
            float* quat = reinterpret_cast<float*>(entry + 16);
            float* pos = reinterpret_cast<float*>(entry + 36);
            if (mode == 0) {
                for (int i = 0; i < 4; ++i) quat[i] = t3[i];
                *reinterpret_cast<float*>(entry + 32) = t3[4];
                for (int i = 0; i < 3; ++i) pos[i] = t3[5 + i];
            } else if (mode == 1) {
                Slerp__6VIQuatRC6VIQuatT1f(t3, quat, t3, weight);
                float scale = *reinterpret_cast<float*>(entry + 32);
                t3[4] = scale + (t3[4] - scale) * weight;
                t3[5] = t3[5] - pos[0];
                t3[6] = t3[6] - pos[1];
                t3[7] = t3[7] - pos[2];
                t3[5] = t3[5] * weight;
                t3[6] = t3[6] * weight;
                t3[7] = t3[7] * weight;
                t3[5] = t3[5] + pos[0];
                t3[6] = t3[6] + pos[1];
                t3[7] = t3[7] + pos[2];
                for (int i = 0; i < 4; ++i) quat[i] = t3[i];
                *reinterpret_cast<float*>(entry + 32) = t3[4];
                for (int i = 0; i < 3; ++i) pos[i] = t3[5 + i];
            }
        }
    }
    return 0;
}
}
