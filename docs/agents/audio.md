# IOP sound client

Package notes for the published `norrath_sound_*` imports. Original names and
addresses below were read from the pinned SLUS-20565 symbol table and the
corresponding instruction bytes. Kernel effects beyond those bytes are unknown.
Host checks are not EE plan inputs.

## DMA handler registration

Observed `AddDmacHandler` at 0x002d4df0 is sixteen bytes: service 18, then
return. Arguments stay in the incoming registers and the kernel result stays
in `v0`. The published symbol is `norrath_sound_add_dma_handler`.

Reviewed EE assembly profile `authored_entry_asm`. Host check:

```sh
c++ -std=c++17 -Wall -Wextra -o /tmp/audio-add-dma-handler-check \
  experimental/sources/audio-add-dma-handler/host_check.cpp
/tmp/audio-add-dma-handler-check work/private/SLUS_205.65 0x002d4df0 \
  work/audio-add-dma-handler.o norrath_sound_add_dma_handler
```

The check compares the complete compiled body with the pinned executable. It
fails if the bytes differ.

### Build-plan entry

Append after the current last input. Profile `authored_entry_asm`. Header
closure empty. Object name `add_dma_handler.o`. Source
`sources/audio-add-dma-handler/add_dma_handler.S`. SHA-256
`64739e1a20d2ee7356b080cbc6be2ebbde719f0234b54d8d11e39fe7fb9daef3`.
Do not add `host_check.cpp`
as a plan input; it uses host headers. `snapshot.json` must record both files
before `tools/check_source_snapshot.py` will accept the tree. This note does
not edit that snapshot.

## DMA chain request

Observed `sceSifSetDChain` at 0x002d54c0 is sixteen bytes: service 120, then
return. It writes no argument. `isceSifSetDChain` is service -120 and is not
this import. The published symbol is `norrath_sound_set_dma_chain`.

Same host checker shape, address `0x002d54c0`, object symbol
`norrath_sound_set_dma_chain`. The compiled body matched the pinned
executable.

### Build-plan entry

Profile `authored_entry_asm`. Header closure empty. Object name
`set_dma_chain.o`. Source `sources/audio-set-dma-chain/set_dma_chain.S`.
SHA-256 `488f61cbb18e9fa87e7711ba2fe07f79502da0eeb5d4bacb7fa46afd6b4d884e`.
Do not plan `host_check.cpp`.

## DMA enable

Observed `EnableDmac` at 0x002d5cf8 is 104 bytes. It masks COP0 Status with
`0x10000`, calls `DIntr` only when that bit is set, requests service 22
(`_EnableDmac` at 0x002d4e40) with the incoming channel, executes `sync`, and
calls `EIntr` on the same bit. The service result is returned. This source
issues service 22 directly and calls `norrath_sound_disable_interrupts` and
`norrath_sound_enable_interrupts`. It is not an original-byte match. The host
contract fails unless the disabled path is service-then-sync, the enabled path
is disable, service 22, sync, enable, and bit 0 alone does not take the
interrupt path.

### Build-plan entry

Profile `authored_cpp`. Header closure empty. Object name `enable_dma.o`.
Source `sources/audio-enable-dma/enable_dma.cpp`. SHA-256
`295e36b9d1eb95ad5e2d8f32b37cdfd5a89a15e35bf84b923fb807a36f2180a6`.
Do not plan `host_contract.cpp`.
