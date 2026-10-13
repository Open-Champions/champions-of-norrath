# w13-m-startup-data-a: startup, runtime-support and Sony library data words (part 1)

Scope: the 31 unresolved imports listed in `wave13.json` for this package. Nothing here
was executed on the EE, and nothing is a byte match against the pinned image. Addresses
come from the pinned `SLUS_205.65` symbol table and section contents, read locally from the
private reference; no bytes, disassembly or decompiler output are committed.

## Status

| | |
|---|---|
| Closed by this package's units (new authored source) | `_sceSDC`, `eh_context_static`, `gConfig`, `iRandState`, `localRandState`, `norrath_ee_exception_frame` |
| Closed by committed sources not yet in the shared plan | `__EH_FRAME_BEGIN__`, three `norrath_archive_vblank_*` views, twelve libpad/libmtap views, `norrath_loader_failure_word_view` |
| Still open | six linker-layout markers, two `norrath_ctors_ant_lantern_view_*` names (see the last two sections) |

Closed: **23 of 31**. Opened: **none**.

## Measurements

Each figure comes from a command run in this worktree.

- Baseline partial build, shared plan at this commit's parent, driver defaults, output in
  the git-ignored `work/w13-base`: 5,528 selected authored inputs; **1,168 unresolved
  imports** in `unresolved-imports.json`. All 31 names of this package were present.
- Closure check, `ld -r` of the baseline partial object plus this package's objects and the
  four reused objects, using the reviewed `mips64r5900el-ps2-elf-ld -m elf32lr5900 -r
  --warn-common`: **1,145 unresolved** (1,168 − 23). No undefined symbol appeared that was
  not already undefined in the baseline. This is a link of objects, not the driver's
  per-unit build.
- Private plan check: a private copy of the plan and snapshot (outside git, under
  `work/w13-root` and `work/w13-check`) with the eight entries below added, run through
  `build_partial.py --check-plan`: `source_plan_valid`, 5,536 selected inputs, zero target
  tools executed.
- Full driver build with the private plan: **not run**. The closure check above covers the
  link question; the driver's per-unit compile and function-inventory checks still need one
  run by the integrator.

## Checks performed

- `_sceSDC`: host contract compiled with `c++ -std=c++17 -Wall -Wextra -DNORRATH_HOST_CONTRACT`;
  passes. A mutated copy with an exclusive upper bound fails with exit 2. Unit compiled with the
  reviewed EE g++ under the `authored_cpp` flags; exports `_sceSDC`, no undefined symbols.
- `eh_context_static`: host contract passes (layout offsets 224 and 232 asserted, first-call
  effects, no re-initialize, re-initialize after a cleared flag). EE compile passes.
- `gConfig`, `iRandState`, `localRandState`: host check compares the observed initial bytes
  and derived values (halfword 540 at offset 0, byte 0 at offset 6, `localRandState` 0x000e18cd).
  EE compile passes.
- `norrath_ee_exception_frame`: host check (size 0x230, 16-byte alignment, zero contents).
  EE compile passes.
- `tools` suite: `python3 -m unittest discover -s tools -p 'test_*.py'` ran 214 tests, all OK.

## Imports closed

Each name maps to the file that defines it. "New" means a source this package wrote;
"reused" means a committed source from an earlier wave that defines the same name at the
same observed address, which this package did not copy.

| Name | Defined by | Kind |
|---|---|---|
| `_sceSDC` | new: `w13-m-startup-data-a-sce-dcache-sweep/sce_dcache.cpp` | function, observed at 0x002d5930 |
| `eh_context_static` | new: `w13-m-startup-data-a-eh-context-static/eh_context_static.cpp` | function, observed at 0x002fa3b8 |
| `gConfig` | new: `w13-m-startup-data-a-config-rand-words/config_rand_words.cpp` | 8-byte data, 0x00444850 |
| `iRandState` | new: same file | 4-byte data, 0x00486ab4 |
| `localRandState` | new: same file | 4-byte data, 0x00484918 |
| `norrath_ee_exception_frame` | new: `w13-m-startup-data-a-ee-exception-frame/ee_exception_frame.cpp` | 0x230-byte block, 0x008e5780 |
| `__EH_FRAME_BEGIN__` | reused: `w12-startup-rt-a-eh-frame-begin/eh_frame_begin.cpp` | zero-length marker, 0x00453970 |
| `norrath_archive_vblank_count_view`, `..._field_view`, `..._semaphore_view` | reused: `sony-cd-vblank-views/views.cpp` | 0x00484438, 0x0048443c, 0x00486a60 |
| `norrath_libpad_is_init_word`, `norrath_libpad_is_warning_word`, `norrath_libpad_rpc_clients_view`, `norrath_libpad_port_info_view`, `norrath_libpad_dma_info_view`, `norrath_libpad_rpc_buffer_view` | reused: `simloop-services-pad-mtap-state/pad_mtap_state.cpp` | 0x004448f8, 0x004448fc, 0x008dd880, 0x008dd8d0, 0x008dd9c0, 0x008ddac0 |
| `norrath_libmtap_port_open_client_view`, `..._port_close_client_view`, `..._get_connection_client_view`, `..._change_priority_client_view`, `..._module_version_client_view`, `..._rpc_buffer_view` | reused: same file | 0x008ddb40, 0x008ddb68, 0x008ddb90, 0x008ddbb8, 0x008ddbe0, 0x008ddc40 |
| `norrath_loader_failure_word_view` | reused: `level-anim-load-loader-failure-word/failure_word.cpp` | absolute symbol, value 0 |

The reused views were checked against the ELF addresses above. The reused sources are not
copies of this package's work and carry their own review notes. Their owners must add them to
the shared plan, or the integrator can take the entries below.

## Imports opened

None. The closure check reports no undefined symbol that the baseline did not already have.

## Build-plan entries needed

These eight entries are appended after the last shared input (`input_order` 5529). The lead
must renumber them if the shared plan changes. Each `files` entry is the same `sha256` as its
`sha256` field, and `header_closure` is empty for every entry. `object_name` values are
proposals. A `files` table entry is required for each `source`.

```json
{"emitted_functions": [{"compiled_body_bytes": 160, "name": "_sceSDC"}], "header_closure": [], "input_order": 5530, "kind": "independently_authored_source", "object_name": "w13msd_sce_dcache.o", "profile": "authored_cpp", "sha256": "5d2c45bbb7884a074c9933c59632700911ba63954b21188aca7026e258da5082", "source": "sources/w13-m-startup-data-a-sce-dcache-sweep/sce_dcache.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 76, "name": "eh_context_static"}], "header_closure": [], "input_order": 5531, "kind": "independently_authored_source", "object_name": "w13msd_eh_context_static.o", "profile": "authored_cpp", "sha256": "c0e1cc1d47d1eb47663ac41793b257d78d8d585f85e55330c5a3ee9d7fdafcb7", "source": "sources/w13-m-startup-data-a-eh-context-static/eh_context_static.cpp"}
{"header_closure": [], "input_order": 5532, "kind": "independently_authored_source", "object_name": "w13msd_config_rand_words.o", "profile": "authored_cpp", "sha256": "94692d2ccde24c7a1852d0f99b86a4faaf627833dca8c4b1b6b6383d3c74c81b", "source": "sources/w13-m-startup-data-a-config-rand-words/config_rand_words.cpp"}
{"header_closure": [], "input_order": 5533, "kind": "independently_authored_source", "object_name": "w13msd_ee_exception_frame.o", "profile": "authored_cpp", "sha256": "59b1e832084ea49d11a5767fd4dee10216ce7e15551b8ddb340a7bd30e290647", "source": "sources/w13-m-startup-data-a-ee-exception-frame/ee_exception_frame.cpp"}
{"header_closure": [], "input_order": 5534, "kind": "independently_authored_source", "object_name": "w13msd_sony_cd_vblank_views.o", "profile": "authored_cpp", "sha256": "2735a9c09cb5afcffd3881673c54a30b2da2862a2278c3da24b7ef2325bc30d7", "source": "sources/sony-cd-vblank-views/views.cpp"}
{"header_closure": [], "input_order": 5535, "kind": "independently_authored_source", "object_name": "w13msd_pad_mtap_state.o", "profile": "authored_cpp", "sha256": "cd4cc30389e9190b46da94d0d284631c1242bd84a22fbb94ca8b37b17cb74bab", "source": "sources/simloop-services-pad-mtap-state/pad_mtap_state.cpp"}
{"header_closure": [], "input_order": 5536, "kind": "independently_authored_source", "object_name": "w13msd_loader_failure_word.o", "profile": "authored_cpp", "sha256": "f8f47f4028b1ca99e6bceb0a9f9b8fca00dff4554b2b1ffde7c9a18896ad4791", "source": "sources/level-anim-load-loader-failure-word/failure_word.cpp"}
{"header_closure": [], "input_order": 5537, "kind": "independently_authored_source", "object_name": "w13msd_eh_frame_begin.o", "profile": "authored_cpp", "sha256": "aaeea69fa811ccc1982c4d1636ff91c4e9b4ba962257c0e28b3da9316c09d707", "source": "sources/w12-startup-rt-a-eh-frame-begin/eh_frame_begin.cpp"}
```

Placement caveat for the integrator. `__EH_FRAME_BEGIN__` is a zero-length object in
`.norrath_eh_frame_begin`, not in `.eh_frame`. The final link has to put that input section
first, as `docs/agents/w12-startup-rt-a.md` describes. Without that ordering the symbol does not
equal the start of `.eh_frame`.

Also the shared snapshot's `files` table needs the eight `sha256` values above, which the
private snapshot used.

## Authored but never executed

- `_sceSDC`: the cache sweep has never run on the EE. Its host contract replaces the cache
  operations and TagLo read with scripted hooks, so the inline assembly path is compiled but
  not exercised. The authored body is 160 bytes against the image's 164; this is not a byte match.
- `eh_context_static`: its first-call path was checked only by the host contract, which uses
  the real struct layout on the host (pointer width 8, not 4). The EE pointer width of 4 is not
  exercised.
- `gConfig`, `iRandState`, `localRandState`, `norrath_ee_exception_frame`: data definitions only.
  Nothing has read them on the EE.
- All reused sources listed above: their own notes cover their execution status; this package
  did not run them.
- `__EH_FRAME_BEGIN__`: placement is unverified in a final link.

## Unknown

- The six linker-layout markers, `_gp`, `_fbss`, `_bssend`, `_end`, `_srsctorsstart` and
  `_srsctorsend`. The pinned image values are `_gp` 0x004879f0, `_fbss` 0x00486800, `_bssend`
  and `_srsctorsstart` 0x008eab1c, `_srsctorsend` 0x008eab58, `_end` 0x008ec91c. These
  are defined by the link script, not by a relocatable object. They stay open until the
  integrator's layout exists (`docs/LINK_LAYOUT.md`). Nothing in this package defines them.
- `norrath_ctors_ant_lantern_view_34b474`. It is a 4-byte entry in a pointer table in `.data`
  whose initial value is an absolute address (0x00602de8) of an object this build does not
  have. Defining it with that number would not reproduce the original's behavior, and defining
  it as zero changes behavior. The ctors-ant-lantern owner needs to decide what the entry
  points to in the reconstruction.
- `norrath_ctors_ant_lantern_view_34b6b1`. It is a character buffer inside `.data`. Its
  extent is not established: the ELF has no symbol for it, and the neighbouring words are
  unlabelled. The innoruuk contract writes a level name into it and the badger and chest
  constructors read it as a string. A size chosen without evidence would be a guess.
- Whether `_sceSDC`'s 0x10 index operation is the one the hardware documents for TagLo. The
  cache operation numbers are the observed ones.
- The writers of `gConfig`, and the other readers of `localRandState` and `iRandState` beyond
  the shop grid.
- The meaning of the 8 bytes at offset 232 of the static context block, and whether anything
  reads the block (`eh_context_static`).
- What fills `norrath_ee_exception_frame` on the EE, and which exception paths reach
  `_kExitTLBHandler` at run time.
