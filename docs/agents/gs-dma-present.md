# GS DMA frame present

Work stays on `agents/gs-dma-present`. Takeover found a clean worktree at
`e49d0b4`, with 24 committed package definitions and no handoff note. No
uncommitted predecessor edits needed retention or removal. The pinned image
SHA-256 was rechecked: `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`.
Original edition remains unresolved. All addresses below refer to that image.

The inherited 23 C++ host contracts passed again with ASan/UBSan; all 24
source units compiled with the reviewed EE profiles. This is not execution
on an EE. Source comments retain per-symbol observed addresses and behavior.

## frameStartPacket__FUibi

Observed at `0x0018fac0`, 832 bytes. Constructs DMA/GIF headers and the two
frame/depth, scissor, offset and control contexts. The old engineYOffset is
packed before its new value is stored. Color zero skips the clear sprites;
nonzero emits 20 pairs, including the final strip. Full mode skips the last
two scissor writes. The packet is written through the uncached alias and
sent using the ordinary frameStatePacket address, followed by sync-path.
The fixed construction makes the original alternate end-tag branch unreachable.
Host cases cover both modes, both fields, both color paths, every output word,
the first untouched word, and call order. Fresh EE compilation passed.

Inferred: the register-number pairs are GS commands. Unknown: hardware timing,
valid screen-slide range, interrupts, and actual frame presentation.

## lightVU0StoppedProcessing__Fv

Observed at `0x0013ce70`, 648 bytes. Calls `lightResetOptionalParams__Fv`
on entry and exit. Stores 8388608.0 to the four VU words at `0x1100ffd0`
through `0x1100ffdc`, the unnamed `.lit4` 8388736.0 to `0x1100ffcc`, the
float of the scene offset-52 word biased by `0x80000000` to the three words
at `0x1100ffc0` through `0x1100ffc8`, and `worldCameraPos[2] * 0.25` to
`0x1100ffac`, in that store order. Four `worldCharViewMatrix` column lengths
are square-rooted into dead stack slots. The eight scene bytes at offsets
44..51 are scaled by 1.0, or by the `globalCharLightScale` word when
`interfaceMode` is nonzero, rounded half-to-even, clamped to 0..255, and
stored at offsets 36..43. Host cases cover both scale paths, half-even ties,
both clamp edges, both bias wraps, all nine VU stores with order, and the
double reset call. Fresh EE compilation passed with no new imports.

Inferred: round-to-nearest-even matches `cvt.w.s` under the default FCSR
mode. Unknown: FCSR state, VU/converter side effects, the scene word
meanings, and why the matrix lengths are discarded.

## doSuperSample__Fiib

Observed at `0x0018fe00`, 992 bytes. A changed mode word rebuilds the
sample packet with `(mode != 0)` and updates `madePacket.769`, defined here
with its `-1` file value. Gray 128 is used unless `useComposite` selects the
composite words. A nonzero `trippleBufferMode` with alpha below 128 emits
the fixed 12-quadword `frameStatePacket` (tag `0x70000005`, DMA/GIF header,
one buffer word mixing both display bases, and the constant tail) and sends
it before the sync-path wait; the alternate tag arm is dead. `copyScreenAlpha`
takes alpha clamped to 0..128, `copyScreenColor` takes the doubled scaled
color packed R/G/B with round-half-even and 0..255 clamps, and the three
composite words step toward the 124..133 band (`>= 133` drops 4, 124..132
becomes 128, below grows 4). Brightness at or above 0.2 (NaN included) bumps
`compositeAlpha` by 3 and pins the scale to 0.2. `copyScreenFrame` takes the
shifted selected base with `0xa0000`, then `FlushCache(0)`, the `copyScreen`
send, and a final sync-path wait. Host cases cover mode caching, both buffer
selects with exact packet words, all three skip paths, call order, both
brightness paths plus NaN, alpha clamps, saturation via out-of-band words,
and every stepper edge. Fresh EE compilation passed with no new imports.

Inferred: the packet words are DMA/GIF descriptors. Unknown: hardware
packet effects, FCSR mode, caller argument meanings, and valid color ranges.

## reloadDMA__Fi

Observed at `0x0015ed18`, 2488 bytes. Always returns 1. An uninitialized
engine with any cause but -1 runs sync/ei and returns. Cause -2 bumps
`donesGot.753` when `channelDone.747` has bit 1 (else clears
`flushing.755`) and stores 2 at `0x12001000`. Cause -1 flips
`readBuffer.745`, marks initialized, clears the bucket, `texTypes.758`,
`sentDone.752`, both done counters, points all four texture queues at the
draw-record head, and sets `channelDone.747` to 0 (2 with an empty head).
A 2-valued `channelDone.747` with matching counters scans bucket slots for
a new head, locking and skipping nodes. The drain masks `channelBusy.746`
with `~(1u << (cause & 31))`, walks the tex/vif queues against DMA
counters, sweeps on cause 1, repoints on cause 2, and honors `flushing.755`.
Uploads build GIF tags at computed tex/vif list addresses (lock/cacheIn
selected by halfwords, s3 budget vs `drawSizeOverEnable`), close with
`0x70000000`, kick lengths at `0x1000a000`/`0x10009000`, and flip the lists.
The tail signals `endDrawSemaphore` (EIntr/SignalSema/DIntr for cause -1,
iSignalSema otherwise) once the bucket reaches 8 with idle channels. All
21 state words are defined here with file values (zeros, `drawSizeOverEnable`
1); the zero-initialized ones land in BSS rather than `.sdata` under the
reviewed profile. Host cases cover entry gating, both -2 arms, reframe,
tex/vif drains, the cause-1 sweep, cause-2 repoint, flushing return, both
semaphore tails, upload/cachein/small paths with exact store words, the s3
budget trip, the vif 6-node batch close, and the refill lock walk. Fresh EE
compilation passed with no new imports.

Inferred: computed addresses are uncached DMA-list aliases; the dead tag
accumulator and dead stack slots carry no behavior. Unknown: hardware DMA
effects, texture heap layout, valid counter ranges, and the texture
halfword meanings.

## makeSamplePacket__Fb

Observed at `0x0018f1f8`, 1492 bytes. Builds 577 quadwords, including two
70-tile passes and the end controls. Full mode changes vertical destination
scale and scissor height. The texture base is reloaded between passes.
Publishes copyScreenFrame at byte 48, copyScreenColor at 144, and
copyScreenAlpha at 180, then FlushCache(0). Every packet word and the first
untouched word pass the two-mode host contract; reviewed EE compilation passed.
No packet was submitted to hardware. Original alternate end-tag path is
unreachable for these fixed loop bounds.
