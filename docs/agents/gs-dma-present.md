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

## makeSamplePacket__Fb

Observed at `0x0018f1f8`, 1492 bytes. Builds 577 quadwords, including two
70-tile passes and the end controls. Full mode changes vertical destination
scale and scissor height. The texture base is reloaded between passes.
Publishes copyScreenFrame at byte 48, copyScreenColor at 144, and
copyScreenAlpha at 180, then FlushCache(0). Every packet word and the first
untouched word pass the two-mode host contract; reviewed EE compilation passed.
No packet was submitted to hardware. Original alternate end-tag path is
unreachable for these fixed loop bounds.
