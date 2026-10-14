// Observed doNormalCam__6Camera at 0x0026d4d0, 1496 bytes. All camera offsets
// below are bytes into the camera; "F(n)" is a float, "W(n)" a word, "B(n)" a
// byte at camera+n. Nothing here was executed on the EE.
//
// Entry: B(352) = 1 when localNmPlayersAlive (0x00484f48) is at least 2, else
// 0. W(100) = 0. Then, when localNmPlayersAlive is nonzero: take the first
// element e of localPlayersAlive (0x0042a3d8); when the word at
// *(player + 23984 * e + 21872) + 1328 (player is 0x00408600) is nonzero,
// W(100) becomes 1 if nmPlayersAlive (0x00484f44) is 1, or if it is at least
// 2 and the same quantity (taken through the elements of playersAlive,
// 0x0042a3e8, indices 0 .. nmPlayersAlive - 1) is equal for all of them.
// If gamePlayersPresent (0x00484f90) is zero: W(72) = 0 and return.
// If the signed byte B(364) is zero: obtainTarget__6CameraP6Point3(camera,
// camera+24) and a zero result returns immediately. Then a positive B(364) is
// decremented. When W(96) is nonzero, localNmPlayersAlive differs from the
// signed byte B(366) and the byte netNetworkMode (0x00483829) is zero:
// playerCountChanged__6Camera is called and B(366) = localNmPlayersAlive.
//
// Two paths follow. Path A is taken when W(72) is nonzero or, with W(72) zero,
// when the signed byte B(365) is nonzero. With W(72) zero, F(200) is first
// reduced: when it is nonzero it becomes F(200) - F(200) * 0.05 * 1.0 (the
// literals are the file words 0x3d4ccccc and 0x3f7fffff), and a result below
// 0.02 (0x3ca3d70a) is stored as 0.0. Path A copies the 12 bytes at camera+24
// to camera+36, then calls doUserZoom__6Camera.
// Path B (W(72) zero, B(365) zero): k = 0.99998295 (0x3f7ffee2) for x and y and
// k2 for z, where k2 = 0.059988 (0x3d75b5fa) when F(156) is zero, else
// 0.29994 (0x3e9991bc). The delta d = ((F(24..32) - F(36..44)) * (k, k, k2)) *
// (1.0 - F(200)) componentwise. When B(352) is nonzero, W(100) is zero and
// F(200) is zero, d is replaced by this: with M the floats of
// worldCameraMatrix (0x0037ef00), a = dot(d, (M0, M3, M6)), b = dot(d, (-M1,
// -M4, -M7)), c = dot(d, (-M2, -M5, -M8)), w = max(0, 0.9 - F(204)) *
// max(0, 0.7 - F(208)) (literals 0x3f666666 and 0x3f333333), a, b and c are
// each multiplied by w, and d = a * (M0, M3, M6) + b * (-M1, -M4, -M7) + c *
// (-M2, -M5, -M8); then enforceMaxSpeed__6CameraP6Point3(camera, &d) may
// change d. In every Path B case F(36..44) += d, W(68) = W(64), W(64) =
// determineBestController__6Camera(camera), and when W(96) is nonzero
// checkYawReset, doUserZoom, doUserYaw and doAutoPitch (all taking the
// camera) are called in that order.
//
// Both paths: BuildMatrix__6CamerafffP6Point3(camera, worldCameraPos x, y, z,
// null). Then with t = F(164): when t equals -1.0, s = F(140); otherwise s =
// min(rayHit__FP6Point3N30(camera+36, &(-M2, -M5, -M8), &(0, 0, t), &(0, 0,
// -1.0)), F(140)). Then BuildMatrix__6CamerafffP6Point3(camera, F(36) - s * M2,
// F(40) - s * M5, F(44) - s * M8, null). Then, when W(72) is nonzero,
// doAutoPitch__6Camera(camera). Finally W(72) = 0 and a nonzero signed B(365)
// is decremented. Nothing is returned. What the fields mean is not
// established; the argument roles of rayHit are taken from the call layout.
extern "C" {
int obtainTarget__6CameraP6Point3(void* camera, void* point);
void playerCountChanged__6Camera(void* camera);
void doUserZoom__6Camera(void* camera);
void doUserYaw__6Camera(void* camera);
void doAutoPitch__6Camera(void* camera);
void checkYawReset__6Camera(void* camera);
int determineBestController__6Camera(void* camera);
void enforceMaxSpeed__6CameraP6Point3(void* camera, float* point);
float rayHit__FP6Point3N30(const void* origin, const float* a, const float* b, const float* c);
void BuildMatrix__6CamerafffP6Point3(void* camera, float a, float b, float c, void* point);
extern int localNmPlayersAlive;
extern int nmPlayersAlive;
extern int gamePlayersPresent;
extern signed char netNetworkMode;
extern int localPlayersAlive[4];
extern int playersAlive[4];
extern unsigned char player[];
extern float worldCameraMatrix[12];
extern float worldCameraPos[3];
}

typedef float __attribute__((may_alias)) AliasedFloat;
typedef int __attribute__((may_alias)) AliasedInt;

#define F(n) (*reinterpret_cast<AliasedFloat*>(camera + (n)))
#define W(n) (*reinterpret_cast<AliasedInt*>(camera + (n)))
#define B(n) (*reinterpret_cast<signed char*>(camera + (n)))

static inline int playerValue(int index) {
    const unsigned char* record = player + 23984 * index;
    const unsigned char* object = *reinterpret_cast<const unsigned char* const*>(record + 21872);
    return *reinterpret_cast<const AliasedInt*>(object + 1328);
}

static inline float maxZero(float x) { return x > 0.0f ? x : 0.0f; }

extern "C" void doNormalCam__6Camera(unsigned char* camera) {
    B(352) = localNmPlayersAlive >= 2;
    W(100) = 0;
    if (localNmPlayersAlive != 0) {
        if (playerValue(localPlayersAlive[0]) != 0) {
            const int count = nmPlayersAlive;
            int i = 1;
            if (i < count) {
                const int first = playerValue(playersAlive[0]);
                while (i < count) {
                    if (playerValue(playersAlive[i]) != first) break;
                    ++i;
                }
            }
            if (i == count) W(100) = 1;
        }
    }
    if (gamePlayersPresent == 0) {
        W(72) = 0;
        return;
    }
    if (B(364) == 0) {
        if (obtainTarget__6CameraP6Point3(camera, camera + 24) == 0) return;
    }
    if (B(364) > 0) B(364) = static_cast<signed char>(B(364) - 1);
    if (W(96) != 0) {
        if (localNmPlayersAlive != B(366) && netNetworkMode == 0) {
            playerCountChanged__6Camera(camera);
            B(366) = static_cast<signed char>(localNmPlayersAlive);
        }
    }

    bool pathA = W(72) != 0;
    if (!pathA) {
        const float scale = F(200);
        if (scale != 0.0f) {
            const float reduced = scale - (scale * 0x1.999998p-5f) * 0x1.fffffep-1f;
            F(200) = reduced < 0x1.47ae14p-6f ? 0.0f : reduced;
        }
        pathA = B(365) != 0;
    }
    if (pathA) {
        F(36) = F(24);
        F(40) = F(28);
        F(44) = F(32);
        doUserZoom__6Camera(camera);
    } else {
        const float k = 0x1.fffdc4p-1f;
        const float k2 = F(156) == 0.0f ? 0x1.eb6bf4p-5f : 0x1.332378p-2f;
        const float remain = 1.0f - F(200);
        float d[3];
        d[0] = ((F(24) - F(36)) * k) * remain;
        d[1] = ((F(28) - F(40)) * k) * remain;
        d[2] = ((F(32) - F(44)) * k2) * remain;
        if (B(352) != 0 && W(100) == 0 && F(200) == 0.0f) {
            const float* M = worldCameraMatrix;
            const float a = (M[0] * d[0] + M[3] * d[1]) + M[6] * d[2];
            const float b = ((-M[1]) * d[0] + (-M[4]) * d[1]) + (-M[7]) * d[2];
            const float w = maxZero(0x1.ccccccp-1f - F(204)) * maxZero(0x1.666666p-1f - F(208));
            const float b2 = b * w;
            const float a2 = a * w;
            const float c = ((-M[2]) * d[0] + (-M[5]) * d[1]) + (-M[8]) * d[2];
            const float c2 = c * w;
            const float nx = (M[0] * a2 + (-M[1]) * b2) + (-M[2]) * c2;
            const float ny = (M[3] * a2 + (-M[4]) * b2) + (-M[5]) * c2;
            const float nz = (M[6] * a2 + (-M[7]) * b2) + (-M[8]) * c2;
            d[0] = nx;
            d[1] = ny;
            d[2] = nz;
            enforceMaxSpeed__6CameraP6Point3(camera, d);
        }
        F(36) += d[0];
        W(68) = W(64);
        F(40) += d[1];
        F(44) += d[2];
        W(64) = determineBestController__6Camera(camera);
        if (W(96) != 0) {
            checkYawReset__6Camera(camera);
            doUserZoom__6Camera(camera);
            doUserYaw__6Camera(camera);
            doAutoPitch__6Camera(camera);
        }
    }

    BuildMatrix__6CamerafffP6Point3(camera, worldCameraPos[0], worldCameraPos[1], worldCameraPos[2], nullptr);
    float s;
    if (F(164) == -1.0f) {
        s = F(140);
    } else {
        const float M2 = worldCameraMatrix[2], M5 = worldCameraMatrix[5], M8 = worldCameraMatrix[8];
        float up[3] = {0.0f, 0.0f, F(164)};
        float down[3] = {0.0f, 0.0f, -1.0f};
        float axis[3] = {-M2, -M5, -M8};
        const float hit = rayHit__FP6Point3N30(camera + 36, axis, up, down);
        const float limit = F(140);
        s = hit < limit ? hit : limit;
    }
    {
        const float* M = worldCameraMatrix;
        BuildMatrix__6CamerafffP6Point3(camera, F(36) - s * M[2], F(40) - s * M[5], F(44) - s * M[8], nullptr);
    }
    if (W(72) != 0) doAutoPitch__6Camera(camera);
    W(72) = 0;
    if (B(365) != 0) B(365) = static_cast<signed char>(B(365) - 1);
}
