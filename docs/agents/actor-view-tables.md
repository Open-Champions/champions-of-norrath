# Remaining actor view tables

Branch `agents/actor-view-tables`, taken over after the first agent was cut off. The
previous agent left 18 uncommitted view objects. Before committing them I compared every
one against the private executable with a separate script kept outside git: strings and
the `.lit4` word byte for byte, resource-map pointers and their strings, and every
virtual-table adjustment word and function pointer against the sized ELF function
symbols at those addresses. All matched. Each object was then compiled with the
reviewed `authored_cpp` EE profile and passed its host test, one commit each. Nothing
ran on the EE.

## Evidence

- Observed: tag strings are the characters plus the NUL; padding after is not part of
  the view. Virtual tables are 8-byte slots (adjustment word, function pointer, zero for
  a null slot). The resource map is pointer-plus-zero-word slots ending in a null slot.
- Observed: `norrath_actor_view_50bfd0` is a mis-decoded name. `objectAddToSlowRunList`
  (`0x1438a0`) builds its base with `lui 0x50` and `addiu 0xbfd0`; the sign extension
  gives `0x4fbfd0`, the `.bss` symbol `slowRunList` (size field 0). `objectGrid` starts
  240 bytes later, so the object is 240 bytes (60 heads of 4 bytes). This contradicts
  the note in `object-infra.md` that it is not that list.
- Unknown: layout of a list head beyond its first word; the base of the original
  placement of any of these objects (definitions do not place objects at their original
  addresses).

## Imports closed

- `norrath_actor_view_486878`
- `norrath_actor_view_50bfd0`
- `norrath_actors_a_view_449790`
- `norrath_actors_a_view_4498b0`
- `norrath_actors_a_view_4499d8`
- `norrath_actors_a_view_44a930`
- `norrath_actors_a_view_44a9c0`
- `norrath_actors_a_view_44c830`
- `norrath_actors_a_view_44ccb8`
- `norrath_actors_a_view_44d0a8`
- `norrath_actors_a_view_44f1f8`
- `norrath_actors_a_view_44f2a0`
- `norrath_actors_a_view_470830`
- `norrath_actors_a_view_471310`
- `norrath_actors_a_view_477758`
- `norrath_actors_a_view_481980`
- `norrath_actors_a_view_483e5c`
- `norrath_actors_a_view_485358`
- `norrath_actors_a_view_485360`
- `norrath_actors_a_view_485400`
- `norrath_actors_a_view_485408`
- `norrath_actors_a_view_4860b8`
- `norrath_actors_a_view_4860c0`

Already closed by earlier commits on this branch (virtual tables, resource map, name
tag): `norrath_actor_view_486878`, `norrath_actors_a_view_438e98`, `44d0a8`, `485358`,
`44ccb8`, `449430`.

Left to the vehicle branch, which defines them under `vehicle-actors-views`:
`norrath_actors_b_view_4816d4`, `484fd8`, `484fe0`, `484fe8`, `4850f0`, `4850f8`.

## Imports opened

The virtual tables name sized ELF functions by assembler label (for example
`msg_run__8FireBomb`, `clone__8FireBomb`, `getModel__10GameObject`). Each referenced
function that no other package defines becomes a new function import. I did not
count them against a full partial link; no merged build was run on this branch.

## Build-plan entries needed

Not added to `experimental/build-plan.json` (shared). Each unit needs an `inputs` entry
with `kind: independently_authored_source`, `profile: authored_cpp`,
`header_closure: []`, `object_name: <name>.o`, `source:
sources/actor-view-tables-views/<name>.cpp`, the sha256 below, and an `input_order`
assigned by the integrator; the same sha belongs in `files`.

| Unit | sha256 |
|---|---|
| `norrath_actor_view_486878` | `a4e7fb5582612b70602575146e1f5b5d33ecf49119a12c2a200144a7ed4d6e13` |
| `norrath_actor_view_50bfd0` | `b7414b6eac64a39782f9dd35aeb475cedf1bd8bc48f6e8ed477fea3c94640fda` |
| `norrath_actors_a_view_438e98` | `1189cde3277e6ec6b42f9f912e2b22885afcd9dd9350a43eb082d41027966444` |
| `norrath_actors_a_view_449430` | `9fb7f4748b63ee49dcdbb2ae9b4fc62c1bc3b6d846a8b6b6f4ec0c4109abfb84` |
| `norrath_actors_a_view_449790` | `b6fd080766cd3ff3e2d7d1a2ab3e5401c22187a7c436e4892efdadcb19e80335` |
| `norrath_actors_a_view_4498b0` | `3558ec27974159f000632a92c8d20a83c0e22e52d12d6dc8fcbc4bdec2459ee7` |
| `norrath_actors_a_view_4499d8` | `04c431c3f2833a61bbc5fd1f26d40b0cb8c787caff9a05d5f8f06b32c9d8f91b` |
| `norrath_actors_a_view_44a930` | `dbdbed6c7bd8eb99bb99917dde96db8ad1123dc7895412301f8594dc71af1caf` |
| `norrath_actors_a_view_44a9c0` | `85e5ae5b3aeebeb16a57b89626d139f5effe4a026f6e2ca1032c21fa65d6e006` |
| `norrath_actors_a_view_44c830` | `3412afd1f1bd7c4cf5b56b47b304d483ea7304f5500a80b5292ece290fb2f0a3` |
| `norrath_actors_a_view_44ccb8` | `f6b784afc49980307a69722e362ff0b799b8af12dd4914a4161499d7ca177994` |
| `norrath_actors_a_view_44d0a8` | `fc2c2a52f506ebab0f03e128a1982dee7f2638444370feddedf3352cc2947948` |
| `norrath_actors_a_view_44f1f8` | `bce1f82294bac041ab65e55db992035c7bb73176fa3cc300d50094cef289522f` |
| `norrath_actors_a_view_44f2a0` | `ae3ce0d2c0bcbda5f7b42cfc36ec84f60afc1d96860f9065b90786b214c2a489` |
| `norrath_actors_a_view_470830` | `44777c8c365704f680b0e4919c307468dddb6d29f30f228c9d9fa911c8def1c1` |
| `norrath_actors_a_view_471310` | `f953a36f85401df6c27ca2ca6907be821bccd09ba07e6123a1f2ab71326de264` |
| `norrath_actors_a_view_477758` | `0e3286e532fc612b5380b1705b95736328d5a9aa8acde64d496e624db4ef86d6` |
| `norrath_actors_a_view_481980` | `9ec5d82b1301dedc962b5b38c92a3dc9bd2dbcc19b7b43a3dbfbd7b8e6ee33cf` |
| `norrath_actors_a_view_483e5c` | `8d1bf67cb747cf0593ce495edc1d8c47fd53d8b6b8e4908604d9ad67aead2dfc` |
| `norrath_actors_a_view_485358` | `e1a03148b42be8198e07fe1fe370f347318e5f0dbb87fc8b2bf41a359f07cad4` |
| `norrath_actors_a_view_485360` | `cac77923bb7b226283bdd455eba2fb8a7354a1f154f9e91de78851c16434ef64` |
| `norrath_actors_a_view_485400` | `d3ae0093990abff6d9915e1636b39a24822833a83333e9132b921518b1de41a7` |
| `norrath_actors_a_view_485408` | `5457ec4c7204b02042cd96fbcc11dc494f639a161f1a068c27b6b28269c62c03` |
| `norrath_actors_a_view_4860b8` | `4dff148551f7a46f1bcc2b79ea11b1aa24b9c5bd695a32dbabdacaad993e6547` |
| `norrath_actors_a_view_4860c0` | `2600e97b6eab8f2eeabaa914c5843edbf7011945d036373c6abdf42766599f57` |

## Authored but never executed

Everything. Host tests run the data through a host compiler with stub functions; none
of it ran on the EE or as gameplay, and the full partial link was not rerun.

## Unknown

- Dispatch and overrides behind each virtual table.
- Whether `syncRandState` at `0x483e5c` is meant to be the same object as the
  object-infra and natives-a names (this unit only defines the actor-view name).
- Layout of the slow run list heads beyond the first word.
