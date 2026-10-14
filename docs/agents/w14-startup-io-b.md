# Startup file, load thread, memory card, pad and net service symbols, part b (w14-startup-io-b)

Branch `agents/w14-startup-io-b`. Checks on this machine: each new function compiled with the reviewed EE toolchain; host
contracts built with the system C++ compiler and passed; `python3 -m unittest discover -s tools -p 'test_*.py'` passed (214 tests).
Nothing ran on the EE or as gameplay. No partial build was run; I linked the seven new objects with `ld -r` to check for duplicate
definitions and listed their undefined symbols.

## Starting point

Observed: of the 20 names in the package, 15 already had authored source in the tree but no build-plan entry, so they stayed unresolved.
The integration log gives the reasons they were left out: `simloop-services-pad-mtap-state` had no host check, the absolute-symbol
loader word is not accepted by the recipe, and the vblank views were left out earlier. Only four names had no source at all.

## Functions (each its own commit, paired against the original image in a private R5900 interpreter harness)

| Symbol | Address | Bytes | Unit | Check |
|---|---|---|---|---|
| `padChangeIndex__Fii` | 0x00168258 | 32 | `w14-startup-io-b-pad-change-index` | 48 paired runs (3 map contents, all 16 index pairs), memory changes and no halts; one mutant killed |
| `unet_startup_async__Fv` | 0x0101c870 | 168 | `w14-startup-io-b-unet-startup-async` | 9 paired runs (net_ready 0/1/7, CreateThread -1/0/5), call log with thread-record words and memory; two mutants killed |
| `unet_startup_async_status__Fv` | 0x0101c918 | 116 | `w14-startup-io-b-unet-startup-async-status` | 15 paired runs (id -1/3/0, status 16/1/0/32/-1), call log with net_ready and id at each call; one mutant killed |
| `receiveControlPacket__FPiib` (revision) | 0x00132d40 | 2596 | `w14-startup-io-b-receive-control-packet` | the earlier 700-run paired script re-run unchanged against the revision: 0 mismatches, 640 of 649 words covered; a one-byte index mutant gives 62 mismatches |

Compiled sizes include trailing alignment nops and are not original-byte matches (the original reaches `padIndexMap` and the net
words through gp; the fresh code does not).

## Observed

* `padChangeIndex__Fii` swaps the bytes at `padIndexMap[a0]` and `padIndexMap[a1]` (0x00484078, gp-14712), no range check.
* `unet_startup_async__Fv`: returns when `net_ready` (0x0118e6b8) is nonzero. Otherwise it fills a thread record (entry
  `unet_startup_thread__FPv` at 0x0101c448, stack `unet_startup_thread_stack` rounded up to 16, size 8192, `_gp`, priority 10) and
  calls `CreateThread`. The result is stored in `unet_startup_thread_id` (0x0118e6bc) even when it is -1; otherwise it calls
  `StartThread(id, 0)` and `ChangeThreadPriority(id, 1)`.
* `unet_startup_async_status__Fv`: id -1 returns 0; status other than 16 returns 1; status 16 sets `net_ready` to 1, terminates and
  deletes the thread, stores -1 in the id and returns 0.
* 0x0048681c is `kickState` (0x00486818, 8 bytes) plus 4, as `w12-startup-rt-b` also found. `receiveControlPacket__FPiib` indexes
  it with the packet's tag byte.
* Pad/multitap objects: spans checked again against the symbol table. The gap between `module_version`'s 40 bytes and `sif_buffer`
  is 56 bytes (the earlier unit's comment said 16). `isInit` bytes are zero and `isWarning` is 1 in the image.

## Inferred

* `net_ready` and `unet_startup_thread_id` are the only state the two async functions share; the pair looks like a start/poll
  interface for a background network start-up thread. The meaning of thread status 16 beyond the comparison is not established.

## Unknown

* What `unet_startup_thread__FPv` does here (not authored in this package) and the size of the thread stack area.
* Whether anything reads the loader failure word after the stores; whether the original code ever reads address 0.
* What lies past `kickState`'s 8 bytes when the tag byte exceeds 3.

## Data units

* `w14-startup-io-b-pad-mtap-state`: the twelve pad and multitap objects of `simloop-services-pad-mtap-state`, republished with a
  host contract (sizes, initial values, alignment, zeroed storage, no overlap) and the corrected gap size. Plan only one of the two.
* `w14-startup-io-b-loader-failure-word`: real 4-byte storage under the import name, replacing the absolute symbol of
  `level-anim-load-loader-failure-word`. This is a deliberate difference: stores land in this object, not at address 0. It needs the
  owner's decision; plan only one of the two.
* `sony-cd-vblank-views` (not mine, already has a host contract) defines the three archive vblank views; no other plan source defines
  them, so it only needs its plan entry. Included below so the three names close.

## Imports closed (when the entries below are planned)

`norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`, `norrath_archive_vblank_semaphore_view`,
`norrath_libmtap_change_priority_client_view`, `norrath_libmtap_get_connection_client_view`,
`norrath_libmtap_module_version_client_view`, `norrath_libmtap_port_close_client_view`, `norrath_libmtap_port_open_client_view`,
`norrath_libmtap_rpc_buffer_view`, `norrath_libpad_dma_info_view`, `norrath_libpad_is_init_word`, `norrath_libpad_is_warning_word`,
`norrath_libpad_port_info_view`, `norrath_libpad_rpc_buffer_view`, `norrath_libpad_rpc_clients_view`,
`norrath_loader_failure_word_view`, `norrath_net_flag_bytes_48681c` (its only user is replaced by the revision, which uses
`kickState`), `padChangeIndex__Fii`, `unet_startup_async__Fv`, `unet_startup_async_status__Fv`.

`norrath_net_flag_bytes_48681c` closes only if `w11-game-a-receive-control-packet` is swapped for the revision; do not plan both
(both define `receiveControlPacket__FPiib`).

## Imports opened

`net_ready`, `unet_startup_thread_id`, `unet_startup_thread_stack`, `unet_startup_thread__FPv` (data and thread entry of the net
start-up thread, addresses 0x0118e6b8, 0x0118e6bc, 0x011e31d8, 0x0101c448). `kickState` is already defined by
`w10-net-a-frame-state-data`; `padIndexMap` by `simloop-services-pad-index-map`. `CreateThread`, `StartThread`,
`ChangeThreadPriority`, `ReferThreadStatus`, `TerminateThread`, `DeleteThread` are the existing kernel call names.

## Build-plan entries needed

Append in this order. Remove the `w11-game-a-receive-control-packet` entry when adding the revision. Each source and host file must
also be recorded in `snapshot.json`; host contracts are not plan inputs. Assign `input_order` on the integration tip.

```json
[
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 36,
        "name": "padChangeIndex__Fii"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14-startup-io-b-pad-change-index.o",
    "profile": "authored_cpp",
    "sha256": "c52bf3fcc389d92d741731369b332cfd44ecd7d4327cc51b669158f6e6a7a992",
    "source": "sources/w14-startup-io-b-pad-change-index/pad_change_index.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 160,
        "name": "unet_startup_async__Fv"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14-startup-io-b-unet-startup-async.o",
    "profile": "authored_cpp",
    "sha256": "e00640a17030b40e72f60380af223b4e3f76b82a38aafe58b4512678dd480b01",
    "source": "sources/w14-startup-io-b-unet-startup-async/unet_startup_async.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 128,
        "name": "unet_startup_async_status__Fv"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14-startup-io-b-unet-startup-async-status.o",
    "profile": "authored_cpp",
    "sha256": "4f93ef03b99f9ed87f539898f4af1c20ce070dfe08c52f7292ac7fa13a32156f",
    "source": "sources/w14-startup-io-b-unet-startup-async-status/unet_startup_async_status.cpp"
  },
  {
    "emitted_functions": [
      {
        "compiled_body_bytes": 2796,
        "name": "receiveControlPacket__FPiib"
      }
    ],
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14-startup-io-b-receive-control-packet.o",
    "profile": "authored_cpp",
    "sha256": "83bb2905c392be1568dd1a0b48224fe3898677fe3bdb23bfba63d679fd9f75ca",
    "source": "sources/w14-startup-io-b-receive-control-packet/receive_control_packet.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14-startup-io-b-pad-mtap-state.o",
    "profile": "authored_cpp",
    "sha256": "f9bda289f30badc491935e6349a9d5f4223eaeba00e816285923d5ad1a45246f",
    "source": "sources/w14-startup-io-b-pad-mtap-state/pad_mtap_state.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w14-startup-io-b-loader-failure-word.o",
    "profile": "authored_cpp",
    "sha256": "ba7ecffd1799968d4f3b21c1a70154192ba536e4aecb512ef332ece22f7ab8d7",
    "source": "sources/w14-startup-io-b-loader-failure-word/failure_word.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "sony-cd-vblank-views.o",
    "profile": "authored_cpp",
    "sha256": "2735a9c09cb5afcffd3881673c54a30b2da2862a2278c3da24b7ef2325bc30d7",
    "source": "sources/sony-cd-vblank-views/views.cpp"
  }
]
```

## Authored but never executed

All of it: the three new functions and the revision ran only in the private host interpreter against the original image, never on
the EE. The data units have host contracts only.

## Not done

Nothing in the package scope is left without a source. Blocked on the owner: the choice between real storage and an absolute symbol
for the loader failure word, and who swaps the `receiveControlPacket__FPiib` plan entry.
