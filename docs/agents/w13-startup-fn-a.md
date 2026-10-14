# w13-startup-fn-a: runtime support, exception/RTTI helpers, math kernels, OS and input glue

Branch `agents/w13-startup-fn-a`. Sources are under `experimental/sources/w13-startup-fn-a-*`. Every function
below is one commit, compared against the pinned `SLUS_205.65` with a private R5900 paired run (original and
authored function from identical memory snapshots, identical scripted callees, call logs and every changed byte
compared; mutations were run to show the comparison can fail). The paired-run tooling and its output stay
outside git. Several units also have a committed host contract or object check. Compiled sizes are not image
sizes and no byte match is claimed.

## Imports closed (30)

`upcast__C16__user_type_infoRC9type_infoPvPPv`, `CheckforModifierKeyEvents`, `VIBufferSwap__FiPUc`,
`GetOsdConfigParam2`, `IsT10K`, `VIEndian` (data), `__dynamic_cast_2`, `__ieee754_rem_pio2`, `__kernel_cos`,
`__kernel_cosf`, `__kernel_sin`, `__kernel_sinf`, `__tf5Skill` (with the node `__ti5Skill`), `__tfv`, `__throw`,
`addhour`, `convertfrombcd`, `converttobcd`, `generate_key_press_events`, `generate_key_release_events`,
`gsm_decode`, `gsm_encode`, `init_reg_size_table`, `sceDeci2ExRecv`, `sceDeci2ExSend`, `subhour`,
`__tf16__attr_type_info` (with `__ti16__attr_type_info`), `__tf16__func_type_info` (with `__ti16__func_type_info`),
`uncompress`.

A private full partial build (shared branch plus these 29 plan entries, `/tmp` plan outside git) compiled all
5,557 inputs, linked once, and none of the 30 names is in its `unresolved-imports.json`.

Two follow-up commits make the bit-cast helpers of `__kernel_sinf` and `__kernel_cosf` file-local: they had
been exported as global `from_bits`/`to_bits` from both units and would have been duplicate definitions in the
link.

## Notable units

* `gsm_decode` (0x002eb7e0) and `gsm_encode` (0x002ec2c0): the GSM 06.10 full-rate frame coder over 64-bit words
  (33-byte frame, 160 samples, first byte high nibble 13). Helpers stay imports; the paired run scripted them
  and compared their arguments' contents. Encoder returns 33, decoder 160 (0 for a bad header nibble).
* `__throw` (0x002fb070): the earlier `w10-startup-a-throw` and `w12-startup-rt-a-throw` were not selected because
  the handler was entered with different `$v0`/`$ra`/argument registers. This version keeps the compiler's
  exception return but routes it through a small stub in inline assembly so that at handler entry `$v0` is the
  context, `$v1` the adjustment, `$a0` the handler, the stack pointer is adjusted and the callee-saved integer and
  float registers are restored; a paired run compares all of these. Still different: `$ra` is this unit's stub
  address, `$a1..$a3` hold entry values, `$t4..$t6` hold stub inputs. **It defines the same symbol as the
  unselected w10/w12 units; select only one.**
* `__ieee754_rem_pio2`, `__kernel_sin`, `__kernel_cos`: use the image's soft-double helpers (`dpadd`, `dpmul`,
  `dpsub`, `dptoli`, `litodp`, `dpcmp`) as imports, in the image's call order.
* Keyboard passes, clock-record routines, DECI2 stubs and the type_info accessors are small, fully compared.

## Imports opened (still unresolved after the private build)

`GetRomName`, `gRomName` (IsT10K); `Gsm_RPE_Decoding`, `Gsm_Long_Term_Synthesis_Filtering`,
`Gsm_Short_Term_Synthesis_Filter`, `Gsm_LPC_Analysis`, `Gsm_Long_Term_Predictor`, `Gsm_RPE_Encoding`,
`Gsm_Short_Term_Analysis_Filter` (gsm_decode/encode); `__frame_state_for`, `throw_helper` (__throw);
`__kernel_rem_pio2`, `fabs` (__ieee754_rem_pio2); `adddate`, `subdate`, `frombcd`, `tobcd` (clock records);
`dwarf_reg_size_table` (data, extent not visible from the image); `dyncast__C16__user_type_infoiRC9type_infoPvT2T3`
(__dynamic_cast_2); `__tf9type_info`, `__ti9type_info` (attr/func accessors); `_vt$19__builtin_type_info` (__tfv);
`inflateInit_`, `inflate`, `inflateEnd` (uncompress). Resolved by existing units at that build: `Deci2Call`,
`ModifierCodes`, `__rtti_si`, `__rtti_user`, `__terminate`, the soft-double helpers, `litodp`,
`dwarf_reg_size_table_initialized`, `get_eh_context`, `memset`. (Other packages may also close some of the opened names.)

## Build-plan entries needed

`docs/agents/w13-startup-fn-a-plan-entries.json` holds all 29 entries (28 `authored_cpp`, one
`authored_entry_asm` for `GetOsdConfigParam2`; `VIEndian` is a data unit with no function), without
`input_order`, with source hashes of this branch's last commit. Compile flags are the unchanged `authored_cpp`
profile. Host checks and contracts (`host_check.cpp`, `host_contract.cpp`) are not plan inputs.

## Authored but never executed

Nothing here has run on a console or in an emulator. The only execution is the private interpreter's paired run
of single functions against the image, and host-side contracts. Specifically no game or emulator run covers:
`__throw` (builtin-based exception return; verified only in the interpreter with scripted callees), the GSM coder
(helpers scripted, never run on real audio frames), `__ieee754_rem_pio2` with other than the sampled inputs,
and `VIEndian` (data, no behaviour). `init_reg_size_table`, the type_info accessors and `uncompress` have only
scripted-callee evidence.

## Unknown

* What `GetOsdConfigParam2` (service 111) reads and returns.
* The layout of the 1,144-byte frame-state record beyond word 0 and `throw_helper`'s contract; whether any
  landing pad reads `$ra`, `$a1..$a3` or `$t4..$t6` at handler entry.
* The extent of `dwarf_reg_size_table`; the image keeps `VIEndian` in the overlay section and nothing was seen
  writing it.
* Names of GSM state fields (offsets 960, 1280, 2240, 2248, 2256, 2552 are observed; the meanings are inferred
  from the public GSM 06.10 structure).
