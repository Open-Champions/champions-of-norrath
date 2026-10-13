// Host check for runPrediction__Fi. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
// analCompress/analDecompress are replaced by an inverse pair that stores each
// byte in 8 bits, so the frame handed to padProcessData shows what the button,
// column, yaw and yaw-bit stages carry through the bit buffer. The word at
// player record + 21872 stays zero here (that slot holds a 32-bit EE address;
// the copy at the end is covered by the private paired run against the image).
#include "run_prediction.cpp"
#include <cstring>
#include <string>
#include <vector>

extern "C" {
int localNetFrameNm;
int frameNm;
int syncRandState;
int objectIgnoreFlags;
unsigned char* localPredictedPlayer;
signed char netPrediction;
signed char netLogDisable;
signed char netPredictionFastForward;
alignas(8) unsigned char partners[4 * 17632];
alignas(8) unsigned char player[4 * 23984];
alignas(8) unsigned char pad[4 * 88];
unsigned char localPlayerPadState[8];
unsigned char netCameraYaw[8];
alignas(8) unsigned char compImmediateCompressor[80];
alignas(8) unsigned char compImmediateDecompressor[80];
Camera* gameCam = reinterpret_cast<Camera*>(0x44);
}
static std::string events;
static int steps_value = 1;
static unsigned char seen_frame[12];
static int seen_pad;
static int object_calls;
static unsigned char object_storage[256];
static unsigned char table_storage[64];
extern "C" int netGetLocalPlayerNm__Fi(int) { return 1; }
extern "C" int engineGetAIStepCount__Fv() { return steps_value; }
extern "C" void Update__6Camera(Camera*) { events += "U;"; }
extern "C" void padProcessData__FP3padP18padUnprocessedDataP12padStateData(void* pad_record, void* data, void*) {
    seen_pad = (int)((unsigned char*)pad_record - pad);
    memcpy(seen_frame, data, 12);
    events += "P;";
}
extern "C" void analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(void*, void* writer_ptr, void*,
                                                                                    unsigned char* bytes, int count) {
    BitWriter* writer = static_cast<BitWriter*>(writer_ptr);
    for (int i = 0; i < count; ++i) put(writer, bytes[i], 8);
}
extern "C" void analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(void*, void* reader_ptr, void*,
                                                                                               unsigned char* bytes, int count, int) {
    BitReader* reader = static_cast<BitReader*>(reader_ptr);
    for (int i = 0; i < count; ++i) bytes[i] = (unsigned char)get(reader, 8);
}
static void step_method(void* self) {
    ++object_calls;
    events += self == object_storage ? "S;" : "S?;";
}
int main() {
    // No predicted player: one camera update per AI step.
    steps_value = 3;
    localPredictedPlayer = 0;
    runPrediction__Fi(0);
    if (events != "U;U;U;") return 1;
    // Predicted player present, nothing to replay: nothing but the tail runs.
    events.clear();
    localPredictedPlayer = object_storage;
    frameNm = 10; localNetFrameNm = 10;
    runPrediction__Fi(0);
    if (!events.empty() || objectIgnoreFlags != 0x200000 || netPrediction != 0) return 2;
    // Replay two frames (10 and 11) of local player 1's ring.
    memset(object_storage, 0, sizeof object_storage);
    void* table_pointer = table_storage;
    memcpy(object_storage + 72, &table_pointer, sizeof(void*));
    void (*fn)(void*) = step_method;
    unsigned char* entry_at = table_storage + 32;
    memset(table_storage, 0, sizeof table_storage);
    memcpy(entry_at + 4, &fn, sizeof fn);
    *(unsigned*)(object_storage + 12) = 0;
    steps_value = 1;
    syncRandState = 0x1234;
    localNetFrameNm = 12; frameNm = 10;
    unsigned char* ring10 = partners + 17632 + 172 + 24 * 10;
    unsigned char* ring11 = partners + 17632 + 172 + 24 * 11;
    // frame 10: buttons 0x0f0f, analog 1 2 3 4, yaw word 0x1234 with bit 0 set
    ring10[4] = 0x0f; ring10[5] = 0x0f; ring10[6] = 1; ring10[7] = 2; ring10[8] = 3; ring10[9] = 4;
    *(int*)(ring10 + 12) = 0x1235;
    // frame 11: buttons 0xaaaa, analog 9 8 7 6, yaw 0x1236
    ring11[4] = 0xaa; ring11[5] = 0xaa; ring11[6] = 9; ring11[7] = 8; ring11[8] = 7; ring11[9] = 6;
    *(int*)(ring11 + 12) = 0x1236;
    float* floats = (float*)(player + 23984 + 172);
    *floats = 1.5f; *(float*)(player + 23984 + 196) = 2.5f;
    events.clear();
    runPrediction__Fi(0);
    // Each frame: pad step, object step, camera update unless fast forward. localNetFrameNm - steps = 11:
    // frame 10 is below it (fast forward, no camera), frame 11 is not (camera update).
    if (events != "P;S;P;S;U;") return 3;
    if (seen_pad != 88) return 4;
    // The second frame passed through the bit buffer: buttons 0xaaaa (the 16-bit field reads back
    // low byte first as stored), analog bytes and a yaw word whose bit 0 carries the original bit.
    if (seen_frame[0] != 0xaa || seen_frame[1] != 0xaa || seen_frame[2] != 9 || seen_frame[3] != 8 || seen_frame[4] != 7 || seen_frame[5] != 6) return 5;
    // Yaw 0x1236 follows 0x1234 within the smoothing window with a zero step, so the coder repeats
    // 0x1234 and the yaw bit (clear) is applied.
    if (*(int*)(seen_frame + 8) != 0x1234) return 6;
    if (frameNm != 12 || netPrediction != 0 || netLogDisable != 0) return 7;
    if (syncRandState != 0x1234) return 8;
    if (*floats != 1.5f || *(float*)(player + 23984 + 196) != 2.5f) return 9;
    if ((*(unsigned*)(object_storage + 12) & 0x04000000) != 0) return 10;
    if (*(unsigned short*)(netCameraYaw + 2) != (unsigned short)*(int*)(seen_frame + 8)) return 11;
    // The compression counters count one frame each.
    if (*(int*)(compImmediateCompressor + 72) != 2 || *(int*)(compImmediateDecompressor + 72) != 2) return 12;
    return 0;
}
