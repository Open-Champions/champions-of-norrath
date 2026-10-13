# w10-startup-c: startup data views (part 3)

Branch `agents/w10-startup-c`. Every name in this package's scope is a data view, not a function. Addresses are
in the private reference executable (`SLUS_205.65`); no game bytes or disassembly are committed. Nothing ran
on an EE or as gameplay. I did not run a full partial build (the machine was shared); each new unit was compiled
alone with the reviewed `authored_cpp` flags and its host check run on the host.

## Observed

- Every string and the six floats below were read from the image and compared byte for byte (including the
  terminating NUL) with what the sources define: 18 of 18 literals matched; the floats are the image words.
- `_data_table` at 0x008e8c00 is a .bss object of 64 bytes (next symbol `param.0` at 0x008e8c40); the multi-client
  bind routine passes its address to its packet getter.
- The pad/multitap storage already authored in `simloop-services-pad-mtap-state` matches the image symbol extents
  I re-checked: `isInit` 0x004448f8 and `isWarning` 0x004448fc (4 bytes each), `padsif` 0x008dd880 (80),
  `PadInfo` 0x008dd8d0 (240), `dma_iop_data` 0x008dd9c0 (256), `buffer` 0x008ddac0 (128), the five 40-byte clients
  from 0x008ddb40, and `sif_buffer` 0x008ddc40 (128 to the next symbol).

## Inferred

- Roles of the literals come from how the Player, PhProp, Sparks and VIStation constructors use them
  (lump names, sprintf formats, idle animation names, a debug print), not from running them.

## Unknown

- Field layout of the packet pool. Whether the linker merged any literal with another copy. What the six float
  constants mean beyond their use as stored values. The 16 bytes between `module_version` and `sif_buffer`.

## Imports closed

- By my new units: `norrath_sif_m_rpc_packet_pool_view`, `norrath_w9_startup_crt_b_view_11c3ae8`, `_462260`,
  `_4644c0`, `_464ab8`, `_464ac8`, `_464ad8`, `_464ae8`, `_464af8`, `_464b10`, `_464b20`, `_464b30`, `_464b48`,
  `_464b58`, `_464b68`, `_464b78`, `_464b88`, `_464b98`, `_4708d0`, `_4801f8`.
- Already authored elsewhere, closed once the owner's unit is in the plan (I did not define them a second time):
  `norrath_libmtap_change_priority_client_view`, `norrath_libmtap_get_connection_client_view`,
  `norrath_libmtap_module_version_client_view`, `norrath_libmtap_port_close_client_view`,
  `norrath_libmtap_port_open_client_view`, `norrath_libmtap_rpc_buffer_view`, `norrath_libpad_dma_info_view`,
  `norrath_libpad_is_init_word`, `norrath_libpad_is_warning_word`, `norrath_libpad_port_info_view`,
  `norrath_libpad_rpc_buffer_view`, `norrath_libpad_rpc_clients_view`
  (all from `simloop-services-pad-mtap-state`).

## Imports opened

None. The three new units import nothing.

## Build-plan entries needed

Profile `authored_cpp`, empty `header_closure`, paths relative to `experimental/`. Do not add `host_check.cpp`.
The snapshot needs an empty `emitted_functions` record for each (data only).

| source | sha256 | data |
|---|---|---|
| `sources/w10-startup-c-sif-m-packet-pool/packet_pool.cpp` | `61438e0df54b2c5258c9b538b4395c3d738114a4e3533a98d678c4f88e1ed404` | norrath_sif_m_rpc_packet_pool_view (64 bytes, .bss) |
| `sources/w10-startup-c-player-ctor-data/player_ctor_data.cpp` | `ff9747ff9d44e6f3a7b35154890bb2e48507a3c12e2b314d66deabdf3a756191` | 15 strings and the 24-byte float table, .rodata |
| `sources/w10-startup-c-name-literals/name_literals.cpp` | `25a4a15acecd42824ab876d626ab19e9636a5073b5893ab84e4ccfde0042735e` | three strings, .rodata |
| `sources/simloop-services-pad-mtap-state/pad_mtap_state.cpp` (not mine) | `cd4cc30389e9190b46da94d0d284631c1242bd84a22fbb94ca8b37b17cb74bab` | the 12 libpad/libmtap names |

## Authored but never executed

All four-byte-exact data above is only compiled and host checked. No unit ran on the EE, and the constructors
that read these literals were not re-run with them linked.
