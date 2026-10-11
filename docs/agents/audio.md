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
