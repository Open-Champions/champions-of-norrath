/* Sound state read by the frame, music, stream, and sequence code.
 *
 * Observed in the pinned SLUS-20565 symbol table and file image. Each symbol
 * is a file-scope object of the sound unit unless noted. Sizes are st_size or
 * the span to the next symbol in the same section. Initial values were read
 * from the image; .bss and .sbss objects start as zero.
 *
 *   address     size  section  symbol                initial bytes
 *   0x0042de48    12  .data    streamVolume          three words 1.0f
 *   0x00483ffd     1  .sdata   streaming             0
 *   0x00484008     4  .sdata   streamOutput          0
 *   0x0048400c     4  .sdata   streamSizeLeft        0
 *   0x00484bf8     4  .sdata   musicPlaying          0
 *   0x00484bfc     4  .sdata   musicStart            0
 *   0x00484c00     4  .sdata   emergencyMusicWaitAbort 0
 *   0x0048593c     4  .sdata   musicDuckAmount       1.0f
 *   0x00485944     4  .sdata   soundFrameNm          0
 *   0x0048595c     4  .sdata   sequenceInProgress    -1
 *   0x00485960     4  .sdata   sequenceVolume        1.0f
 *   0x004866c4     4  .sdata   musicTimer            14400
 *   0x00486bd0     4  .sbss    disableSoundPlaying   0 (span to masterVolume)
 *   0x007d8110  2432  .bss     sequence              32 records of 76
 *   0x007d8aa0  1152  .bss     delaySoundList        32 records of 36
 *   0x007dd290  1356  .bss     stream                3 records of 452
 *
 * Two symbols named musicTimer exist (0x00484f0c and 0x004866c4). Both start
 * at 14400. The game loop, runMusic, and coreSetMusicTimer address the second;
 * gameSetMusicTimer addresses the first, which this file does not define.
 * streaming at 0x00483ffd is not word aligned; the three bytes before it and
 * the four after it are other objects. The stream pointer is a 32-bit
 * address on the target; streamOutput is declared as the target pointer
 * type by its caller.
 *
 * Record layouts are owned by the code that reads them; this file only fixes
 * sizes and initial values. The static initializer in the sound unit, not this
 * file, writes byte 5 = 1 and word 40 = 0 into every sequence record.
 */
extern "C" {
float streamVolume[3] = {1.0f, 1.0f, 1.0f};
unsigned char streaming = 0;
unsigned char* streamOutput = nullptr;
int streamSizeLeft = 0;
int musicPlaying = 0;
int musicStart = 0;
int emergencyMusicWaitAbort = 0;
float musicDuckAmount = 1.0f;
int soundFrameNm = 0;
int sequenceInProgress = -1;
float sequenceVolume = 1.0f;
int musicTimer = 14400;
int disableSoundPlaying = 0;
alignas(16) unsigned char sequence[2432];
alignas(16) unsigned char delaySoundList[1152];
alignas(16) unsigned char stream[1356];
}
