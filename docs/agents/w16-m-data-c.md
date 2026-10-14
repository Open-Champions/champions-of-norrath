# w16-m-data-c: plain data and type-info/vtable tables (part 3)

Worktree `agents/w16-m-data-c`. Sources are under `experimental/sources/w16-m-data-c-*`, one directory per symbol, each with a
`host_check.cpp`. Scope: the 32 unresolved imports named in the wave brief, taken from
`work/plan16-build/unresolved-imports.json` (841 unresolved symbols; all 32 names present). No function other than `pf` was in
the brief's list.

## Result

- **Closed 12 of 32** names in source: one function (`pf`) and eleven data symbols. Each is its own directory and its own commit,
  pushed as it was committed.
- **Left 20 open** on purpose. Fourteen are already defined by authored units in other packages that are not in the build plan, so
  a copy here would collide at link time. Six have no layout or owner that the evidence settles. The reasons are under "Imports not closed".
- Closing a data import here means the link name, extent, and type exist. No field value is established. Every data unit is zero-filled
  except where the name is a function.
- No symbol here matches the reference byte for byte. `pf` compiles to a 64-byte body; the reference body is 96 bytes. Nothing in this
  package is a byte-match claim.

## Evidence

Observed (private reference, `work/private/SLUS_205.65`, identity checked against the `target_sha256` pinned in `experimental/snapshot.json`):
- Reviewed EE `nm -S`, `nm`, and `objdump -d` were run on that file locally. Nothing from the output is committed; only addresses, sizes,
  and the observed behaviour are written here.
- `pf` is a global function of 0x60 bytes at 0x010464f8. It passes its first argument unchanged, passes the address of its spilled
  variadic area as the second argument to the libc function at 0x00309178, and returns no value.
- `osdsrc` is 0x7a8 (1960) bytes at 0x00444a38, `tlbsrc` is 0x330 (816) bytes at 0x004459d0, both initialised data.
- `statsDrawDelay` is a 4-byte initialised word at 0x00484a34. `showNoNamePrompt` is a bss byte at 0x00486d2f (no size in the table).
- `sRainParticleDist` is a 0x258 (600) byte read-only symbol at 0x0118eec0.
- The six spell views have bss symbols at their addresses with no size in the table. Their 80-byte extent comes from the callers'
  declarations, as in the w15-m-data-b siblings.

Observed in the repository (not from the binary):
- Callers: `w15-startup-a-unet-startup-thread` (pf, in an `extern "C"` block); `runtime-initsys/init_exec_ps2.cpp` and
  `init_tlb.cpp` (osdsrc, tlbsrc); `w15-gameloop-a-stats-frame` (statsDrawDelay, animState alias); `w15-gameloop-a-do-name-pad` and
  `switch-to-name-pad` (showNoNamePrompt); `w11-vi-render-a-atmosphere-init` (sRainParticleDist); the w13/w14 msg-run files (views).
- The w15-m-data-b unit `sRainParticleDist.cpp` defines the name with a file-scope `const` inside `extern "C" { }`. Compiled with the
  plan flags it emits **no symbol** at all (only a file symbol). It satisfies nothing, which is why this package defines the name.
  The w15 owner should fix or remove that unit (see the plan notes below).

Inferred:
- Layouts come from the callers' declared types and extents, and from the reference symbol sizes where the table gives them.

## Checks performed

- Read AGENTS.md, CONTRIBUTING.md, docs/PORTABLE_BUILD.md, experimental/README.md, docs/ROADMAP.md, docs/STARTUP_PROGRESS.md and the
  sibling notes for the same package shape (`docs/agents/w15-m-data-b.md`).
- Compiled each unit with the `authored_cpp` flags from `experimental/build-plan.json` using the reviewed EE g++ (in `work/toolchains/ee`).
  Each object was checked with `nm`: `pf` exports `T pf` and needs `U vprintf`; each data unit exports one global symbol of the expected
  name (`B` for zeroed storage, `R` for `sRainParticleDist`).
- Each `host_check.cpp` was compiled with the host `c++ -std=c++17 -Wall -Wextra -Werror` and run; all returned 0.
- `pf` mutation check: a copy with the `__builtin_va_start` line removed fails its host check with return code 3.
- Compiled the w15-m-data-b `sRainParticleDist.cpp` unit with the plan flags: its object has no `sRainParticleDist` symbol.
- Compiled the sibling units listed below privately (not committed) to confirm each defines the names it claims: each emits the named
  globals.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.
- Not run: a full `tools/build_partial.py` build and any link. Only single-unit compiles were run, as the brief asked. The plan
  hash used by the plan16 build matches the current `experimental/build-plan.json` (`7e9ac625…`), so the 32-name list is current.

## Imports closed (12)

| name | unit | commit |
| --- | --- | --- |
| `pf` | `w16-m-data-c-pf` | `16170cf8` |
| `osdsrc` | `w16-m-data-c-osdsrc` | `9713ae8f` |
| `tlbsrc` | `w16-m-data-c-tlbsrc` | `d398c95b` |
| `showNoNamePrompt` | `w16-m-data-c-show-no-name-prompt` | `de7da570` |
| `statsDrawDelay` | `w16-m-data-c-stats-draw-delay` | `b7ceb6ca` |
| `sRainParticleDist` | `w16-m-data-c-s-rain-particle-dist` | `691836b2` |
| `norrath_w13_spell_msg_a_view_7d6970` | `w16-m-data-c-w13-spell-msg-a-view-7d6970` | `af41aaa9` |
| `norrath_w13_spell_msg_a_view_7d6a10` | `w16-m-data-c-w13-spell-msg-a-view-7d6a10` | `2fb6b370` |
| `norrath_w13_spell_msg_a_view_7d6a60` | `w16-m-data-c-w13-spell-msg-a-view-7d6a60` | `033ef6e3` |
| `norrath_w13_spell_msg_a_view_7d6b00` | `w16-m-data-c-w13-spell-msg-a-view-7d6b00` | `2c91b4a9` |
| `norrath_w13_spell_msg_a_view_7d6fc0` | `w16-m-data-c-w13-spell-msg-a-view-7d6fc0` | `49c0d862` |
| `norrath_w13_spell_msg_a_view_7d7010` | `w16-m-data-c-w13-spell-msg-a-view-7d7010` | `efbfe5c8` |

Notes on the closed set:
- `pf` is the only function. It is a variadic wrapper over `vprintf`; the caller's one-argument call uses the same ABI.
- `osdsrc` and `tlbsrc` are the image bytes the startup code copies to fixed PS2 addresses (`0x80074000`, `0x80075000`). They are
  zeros here, so that copy writes zeros until the owner supplies the image bytes. Do not run the startup path with these units.
- `statsDrawDelay` is zero, so the first frames are not delayed; the image's initial value is not established.

## Imports not closed (20)

Defined by authored units in other packages that are **not in the build plan**. Adding those units closes these names; this package
does not duplicate them, because a second definition would collide at link time. Each unit was compiled privately and emits its names.

- `norrath_archive_vblank_field_view`, `norrath_archive_vblank_semaphore_view`: `sony-cd-vblank-views/views.cpp` (emits both as `B`).
- `norrath_inventory_exit_sound_resource_view`: `w9-hud-inventory-items-b-view-exit-sound-resource/norrath_inventory_exit_sound_resource_view.cpp`
  (emits one `R` symbol; the 21-byte initialiser is the owner's).
- `norrath_rtti_entities_a_view_4818ac`: only `w12-particle-data-b-views/views.cpp` defines it (`R`, 19 words).
- The other ten RTTI names (`480cb4`, `480cb8`, `480cbc`, `480cc0`, `4810b4`, `4810b8`, `481710`, `482424`, `482428`, `48242c`) are
  defined **twice**: once in `w12-particle-data-b-views/views.cpp` and once in the `w9-rtti-vtables-entities-a-view-*` units
  (`480cb4`, `4810b4`, `4810b8`, `481710`, `482424`). The values match in the source text. The integrator must choose one set; adding both
  is a duplicate-definition link error.

Not closeable from evidence in this package:
- `norrath_loader_failure_word_view`: the owner is `level-anim-load-loader-failure-word/failure_word.cpp` (not in the plan). It satisfies
  the name with an absolute symbol at value 0. That is a link-level alias, which the build does not permit, so it belongs to that owner.
- `norrath_ctors_ant_lantern_view_34b474` and `norrath_ctors_ant_lantern_view_34b6b1`: these are interior views. 34b474 is at offset 0x34
  of a 0xfc-byte reference object starting at 0x0034b440, and 34b6b1 is at offset 0x51 of a 0x142-byte object at 0x0034b660. A separate
  storage would duplicate those objects. The host contracts disagree on the second one's size (`[32]` and `[16]`). The owner of the
  containing objects needs to provide the offset symbols.
- `norrath_net_flag_bytes_48681c`: the caller indexes it by a received byte (up to 255), but 0x48681c sits between two neighbouring
  reference objects at 0x486818 and 0x486820. A 256-byte array would overlap them, so the extent is unknown. The owner of the receive
  path needs to establish it.
- `norrath_stats_screen_view_7d0440`: the caller binds it with an `__asm__` label, which is a symbol alias that the build does not apply.
  Its extent is not established (the caller says the stride is 40 bytes; the count is unknown).
- `norrath_inventory_message_handler_word_view`: the caller reads a signed 32-bit word from it, so four bytes is the established layout. No
  address for it appears in any source, so I cannot rule out that it is a view into a larger object. The owner of the inventory-actions
  unit needs to supply the address.

## Build-plan entries needed

The shared `experimental/build-plan.json` and `snapshot.json` are not edited here. Each unit below is one `independently_authored_source`
input with `profile: authored_cpp` and an empty `header_closure` (no includes). `input_order` is for the integrator to assign after the
current last input (7328). Host checks are not plan inputs.

| symbol | source (relative to `experimental/`) | object_name | sha256 |
| --- | --- | --- | --- |
| `pf` | `sources/w16-m-data-c-pf/pf.cpp` | `w16_m_data_c_pf.o` | `34a2d498de3ac3608792fd9854ca62f13737bbe6da9b77091e8e99bcb206085b` |
| `osdsrc` | `sources/w16-m-data-c-osdsrc/osdsrc.cpp` | `w16_m_data_c_osdsrc.o` | `e0aa5df8154eba164b67ff55eb4c79680b21a731e3c30df73abac9c0fceff767` |
| `tlbsrc` | `sources/w16-m-data-c-tlbsrc/tlbsrc.cpp` | `w16_m_data_c_tlbsrc.o` | `30a9f7ae2a7ff8efd9db3f863846882600d169dafaac0c07b56d4c4366636842` |
| `showNoNamePrompt` | `sources/w16-m-data-c-show-no-name-prompt/showNoNamePrompt.cpp` | `w16_m_data_c_show_no_name_prompt.o` | `d22e4dfee06cc2ddd714762a15c3b85dd2004b1dc9599af5a3520cdd1eb917a5` |
| `statsDrawDelay` | `sources/w16-m-data-c-stats-draw-delay/statsDrawDelay.cpp` | `w16_m_data_c_stats_draw_delay.o` | `36f380dbd761a1177996cd17c374077a07a4e9701869034fcaab41897b7a3880` |
| `sRainParticleDist` | `sources/w16-m-data-c-s-rain-particle-dist/sRainParticleDist.cpp` | `w16_m_data_c_s_rain_particle_dist.o` | `fd52130cd5659bb4e0159cab02758b51c66f9f728c5689c7f4b87749ee69a741` |
| `norrath_w13_spell_msg_a_view_7d6970` | `sources/w16-m-data-c-w13-spell-msg-a-view-7d6970/norrath_w13_spell_msg_a_view_7d6970.cpp` | `w16_m_data_c_w13_spell_msg_a_view_7d6970.o` | `224d1781c67744255949706334e3b7ccd46d3f27b32e4d449f4382127f547ea3` |
| `norrath_w13_spell_msg_a_view_7d6a10` | `sources/w16-m-data-c-w13-spell-msg-a-view-7d6a10/norrath_w13_spell_msg_a_view_7d6a10.cpp` | `w16_m_data_c_w13_spell_msg_a_view_7d6a10.o` | `8f5943f69e841e37deefdd7dd59d7ca794a27cde4599c02cfba1ffdcbf921da0` |
| `norrath_w13_spell_msg_a_view_7d6a60` | `sources/w16-m-data-c-w13-spell-msg-a-view-7d6a60/norrath_w13_spell_msg_a_view_7d6a60.cpp` | `w16_m_data_c_w13_spell_msg_a_view_7d6a60.o` | `18035a10971aed47fb609de2100e2f4468e009ae5e620fe06fae0007507debbb` |
| `norrath_w13_spell_msg_a_view_7d6b00` | `sources/w16-m-data-c-w13-spell-msg-a-view-7d6b00/norrath_w13_spell_msg_a_view_7d6b00.cpp` | `w16_m_data_c_w13_spell_msg_a_view_7d6b00.o` | `54b2a7258e54c70344291735f8cd2ae61a8f53769c0090e802959ef383a38ddd` |
| `norrath_w13_spell_msg_a_view_7d6fc0` | `sources/w16-m-data-c-w13-spell-msg-a-view-7d6fc0/norrath_w13_spell_msg_a_view_7d6fc0.cpp` | `w16_m_data_c_w13_spell_msg_a_view_7d6fc0.o` | `5fab334b2ceb575b79e61ff423c9c374872314afce6985dd98501761d19e51b3` |
| `norrath_w13_spell_msg_a_view_7d7010` | `sources/w16-m-data-c-w13-spell-msg-a-view-7d7010/norrath_w13_spell_msg_a_view_7d7010.cpp` | `w16_m_data_c_w13_spell_msg_a_view_7d7010.o` | `07fbe565a2864e169ba9eaf6fbbf363a7792707d4ea93aa294d6dc5966a488fd` |

For `pf`, the snapshot input's `emitted_functions` should record `pf` with `compiled_body_bytes: 64` (the size of this build's object,
not the reference). The data units emit no functions.

Entries the integrator must decide on (already in the plan or listed above):
- `sources/w15-m-data-b-s-rain-particle-dist/sRainParticleDist.cpp` is in the plan (order 6914) but emits no symbol. Either remove it or
  let it stay: with the w15 unit as it is, it adds no conflict, and the name is supplied by this package's unit. If the w15 owner changes it
  to an external definition, drop one of the two definitions.
- The sibling definers to add when their owners approve: `sources/sony-cd-vblank-views/views.cpp`,
  `sources/w9-hud-inventory-items-b-view-exit-sound-resource/norrath_inventory_exit_sound_resource_view.cpp`,
  `sources/w12-particle-data-b-views/views.cpp`, and one of each duplicated RTTI set (see above).
- `vprintf` must be supplied by the libc owner. It is not defined in any authored unit (the reference's `vprintf` is at 0x00309178).

## Authored but never executed

All twelve closed units. Nothing here was linked into an image, run on an EE, or compared with the original. `pf` was not executed on
hardware; its host test uses a recording stub for `vprintf`. The zeroed storage was never read or written by a running game. The
`osdsrc`/`tlbsrc` copy path has not been run with any value.

## Unknown

- Every field value in the data units: the 80-byte particle views, `osdsrc` and `tlbsrc` image bytes, the `sRainParticleDist` table
  values, and the initial value of `statsDrawDelay`.
- The output destination of `vprintf` and the libc behaviour on this target. The host stub does not model either.
- Whether the compiled `pf` (64-byte body) behaves identically to the reference (96-byte body) in every register and stack effect. It
  was not compared; a byte or behavioural comparison would be the next step.
- Whether `osdsrc`, `tlbsrc`, or any view is written by the game after start-up.
- The six undecided names above: the offsets, extents, and addresses that would settle them.
- Which sibling definer of each duplicated RTTI name the integrator will keep.
