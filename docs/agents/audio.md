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

## Command, channel, sequence, stream, and dialog storage

Sizes below are either an ELF `st_size` or the span to the next symbol in the
same section, narrowed where a caller shows the used prefix. Initial bytes
were read from the pinned executable. Host contracts are not plan inputs.
Every data unit uses profile `authored_cpp`, an empty header closure, and
the object name of its `.cpp` stem.

| Plan source | SHA-256 | Symbols |
|---|---|---|
| `sources/audio-command-storage/command_storage.cpp` | `aa53a4be46ed1f5cb5a59e3291dac7fb7be70d112d2accdc08f693ddfeee082d` | packet 128, send 64, init packet 20, DMA handle 4, state 40, handlers 384, registers 128, init word 4 |
| `sources/audio-channel-records/channel_records.cpp` | `0d0cd87af4a41e6fca0153e53cd5018a3316d7fe1070eec6339d10c921bb4092` | 43 records of 20 bytes |
| `sources/audio-channel-voice/voice_map.cpp` | `1b4b700d3ca565b14952af8207709b864d0c08ccf84f7028e5e4ce4ce0247074` | 86-byte voice map |
| `sources/audio-key-words/key_words.cpp` | `11785d495e631ec70f267d14f6621086d23ceeecd19ce7f4475b1465d5e829c1` | key-on and key-off, 8 bytes each |
| `sources/audio-group-limits/group_limits.cpp` | `6f3aa3127fa38a210071db230c1173d3012ed69d7a8f1f39f222d487a1212b25` | 300 group-limit bytes |
| `sources/audio-sequence-records/sequence_records.cpp` | `204b6b8acd702319d6c92b9c31343a5b23b34654bad981140e6973b8812a2c4c` | 32 records of 76 bytes |
| `sources/audio-stream-storage/stream_storage.cpp` | `aa50a8b67685697051e9ca80984d01c4f634f72a0ef2ea143e4bea31bb44e0ab` | three size words, three volume words, three 452-byte records |
| `sources/audio-master-volume/master_volume.cpp` | `d5d756d0e3b8bf4968728590d8d1ebfe5417c9374cf8f6f73745a2fb79e55f0c` | volume word and scale word `0x467ffc00` |
| `sources/audio-delay-thread-name/delay_thread_name.cpp` | `fca61b494483fcf3bd36f2718bf45cf4b6e01705a6a6acfbe6ca541115b77a9b` | `DelayThread` and NUL |
| `sources/audio-dialog-maps/dialog_maps.cpp` | `bccb408e0824d35c473c4e4be0b3c3bc33504c804b1c24c792089638cea73549` | 12-byte map, `mouth.lmp`, `dialog.bin` |
| `sources/audio-iop-buffers/iop_buffers.cpp` | `14f9beab640d0188b7d1d1244f6d369731f7656f9019522d1ab0895ef8d78dcc` | batch and IOP words, 4 bytes each |
| `sources/audio-startup-strings/startup_strings.cpp` | `162d51c4ba625a48af1fb4966e540ae1abda7573e15e3d9ff4646513556d0b40` | five format slots, `sound.cc`, two assert conditions |

`sceSifInitCmd` at 0x002d71a8 uses `_pckt_buffer`, `_send_buffer`, `_csdata`,
`sif0_handleid`, `_data_table` at 0x008dea18, `_sys_buffer`, `soft_reg`, and
`_cmd_init_check`. Two other `_data_table` symbols are different objects and
are not defined here. `soundStopSound` at 0x0024cf80 scans `channelInfo` and
reads `channelVoiceInfo`, `kons`, and `koffs`. `soundPlaySequence` at
0x0024d308 reads `soundGroupLimit` and the 2432-byte `sequence` object.
`soundStartStream` and `soundResetStream` use `stream` at 0x007dd290; a
caller advances that pointer by 452. `soundInitHardware` loads the scale from
`_gp-22992` (0x00482020) and the volume from `_gp-3612` (`masterVolume`).
`DelayThread` stores 0x0047b688. The dialog initializer writes 12 bytes at
`globalDialogDir` and points at `mouth.lmp` and `dialog.bin`.

## Original names used by main and launchSRS

`soundRestore__Fb` at 0x0024bc38 is 984 bytes. Its reset path stores
`0x00032000` and calls `soundInitHardware`, then allocates stream buffers
and issues the same RPC sequence already written as `norrath_sound_restore`.
The original name is an alias of that symbol. Both symbols in the compiled
object have the same value, size, and section. The compiled body is 1632
bytes and is not an original-byte match. The host check fails if the two
symbols diverge. No second plan entry: the existing input
`sources/game-sound-restore-update-build/restore.cpp` changed. Its SHA-256
is now `f628db76921a5ad131df2eb6e2063ec150e758be5e67b68a42522396ae799cad`
(was `a0c8f39bd07f535e79080b52766b70ef771f59225fce3353d34cc3fed31a09c4`).

`soundSleep__Fv` at 0x0024b9e8 is 180 bytes and is not that function. It
frees the three words at `streamAllocation` (0x007dd280) through
`sceSifFreeIopHeap`, clears offsets 44 and 48 in each of the three stream
records, and calls `sceSifCallRpc`. That RPC is the existing
`norrath_sound_call_rpc`: command 160, mode 0, send length 0, receive
length 64, null end function and parameter. The host contract fails if a
zero allocation is freed, a live word is cleared before the free, a
neighboring stream byte changes, or the RPC arguments change.

Profile `authored_cpp`. Header closure empty. Object name `sound_sleep.o`.
Source `sources/audio-sound-sleep/sound_sleep.cpp`. SHA-256
`b8970067048a1cdd7bfca8ae92418c4bfa5ddf1034a27b8096203663daf42d7e`.
Do not plan either host check.

## Imports closed

`soundRestore__Fb`, `soundSleep__Fv`,
`norrath_sound_add_dma_handler`, `norrath_sound_enable_dma`,
`norrath_sound_set_dma_chain`,
`norrath_sound_command_packet_buffer_view`,
`norrath_sound_command_send_buffer_view`,
`norrath_sound_command_initialization_packet_view`,
`norrath_sound_command_dma_handle_view`,
`norrath_sound_command_state_view`,
`norrath_sound_command_system_handlers_view`,
`norrath_sound_system_register_words_view`,
`norrath_sound_command_initialized_word_view`,
`norrath_sound_channel_records_view`,
`norrath_sound_channel_voice_map_view`,
`norrath_sound_key_on_words_view`,
`norrath_sound_key_off_words_view`,
`norrath_sound_group_limits_view`,
`norrath_sound_sequence_records_view`,
`norrath_sound_stream_buffer_sizes_view`,
`norrath_sound_stream_volumes_view`,
`norrath_sound_stream_records_view`,
`norrath_sound_master_volume_view`,
`norrath_sound_master_scale_view`,
`norrath_sound_delay_thread_name_view`,
`norrath_sound_global_dialog_map_view`,
`norrath_sound_global_dialog_name_view`,
`norrath_sound_global_dialog_extension_view`,
`norrath_sound_batch_iop_buffer_word_view`,
`norrath_sound_iop_buffer_word_view`,
`norrath_sound_startup_formats_view`,
`norrath_sound_startup_assert_file_view`,
`norrath_sound_startup_assert_conditions_view`.

## Imports opened

`sceSifFreeIopHeap`. `soundSleep__Fv` also uses the existing unresolved
views `norrath_sound_stream_heap_words_view`, `norrath_sound_rpc_client_view`,
`norrath_sound_rpc_send_view`, and `norrath_sound_rpc_receive_view`. It calls
the existing `norrath_sound_call_rpc` and the existing stream-record view.
`norrath_sound_enable_dma` calls the existing interrupt functions. Service 22
is issued directly, so `_EnableDmac` is not a new import. `soundRestore__Fb`
adds no import.

## Build-plan entries needed

Append the 15 plan sources in this note, plus
`sources/audio-sound-sleep/sound_sleep.cpp`. Refresh the existing
`restore.cpp` hash recorded above. Do not add `host_check.cpp` or
`host_contract.cpp` as inputs. `snapshot.json` must record every new file,
including those host checks and the changed `restore.cpp`, before
`tools/check_source_snapshot.py` accepts the tree. This branch does not edit
the snapshot or the shared plan.

## Authored but never executed

Every function and data object above, including `soundRestore__Fb` and
`soundSleep__Fv`. The host contracts and the DMA body comparisons ran on
this machine. Nothing was executed on the EE. No syscall, DMA, SPU, or game
path has run. `sceSifFreeIopHeap` has not been reconstructed.

## Unknown

Services 18, 22, and 120 have unknown kernel effects. `norrath_sound_enable_dma`,
`soundRestore__Fb`, and `soundSleep__Fv` are not original-byte matches.
`sceSifFreeIopHeap` at 0x002dc2a0 is 28 bytes and only calls
`sceSifFreeSysMemory`; its effect is unknown. The fourth byte of the
16-byte `streamAllocation` object is not written by `soundSleep__Fv`. Four bytes between the 43 channel records and
`soundList`, four bytes between the three stream records and `gCd`, and four
bytes between the dialog map and `delaySoundList` are not owned. Bytes after
each C string are other rodata. `IOPbuffer`'s next symbol starts 8 bytes
later; only the first word is defined. `gCd` at 0x007dd7e0 spans 96 bytes
while the startup review touched 40; `norrath_sound_rpc_client_view` is not
defined. Still unresolved: `norrath_sound_command_dma_destination_word_view`,
the IOP heap views, the remote interrupt and RPC views, the restore format
and condition views, the RPC packet and buffer views,
`norrath_sound_sequence_id_spinner_view`,
`norrath_sound_stereo_enable_word_view`,
`norrath_sound_stream_heap_words_view`, and the wake ring, semaphore, and
top-id views. Their layouts were not established. Hardware timing, interrupts,
and gameplay remain unverified.
