# Original executable layout requirements

The [sanitized inventory](../targets/SLUS-20565/link-layout.json) records declarations
from the exact hash-pinned original. No executable payload is included. All 45
section records and six program records agree with an independent LLVM 20.1.8
inspection. A second inventory run produced identical output. Synthetic fixtures
cover distinct file/zero-fill mappings, overlaps, gaps and malformed inputs.

| Observed scope | Bytes | Meaning |
|---|---:|---|
| Six load segments, file union | 8,354,434 | Distinct original file bytes referenced by loads |
| Six load segments, memory union | 12,966,784 | Distinct declared memory bytes, including zero-fill |
| Allocated executable sections | 5,735,156 | `.text`, `.vutext` and mixed-content `.srs`; not a pure code denominator |
| Allocated initialized data | 2,619,150 | Declared data sections, not reconstructed data |
| Allocated NOBITS sections | 4,612,276 | Declared memory without section payload |
| Nonallocated executable overlays | 33,432 | 18 DVP sections; no disposal or runtime-use inference |
| Nonallocated platform metadata | 911 | DVP overlay tables and strings |
| Nonallocated symbol/string metadata | 844,874 | Retained original metadata, not source |

The allocated categories are sums of nonoverlapping section extents. They are
distinct from load unions: some load ranges have no section, and `.reginfo` has
file bytes outside its containing load's file-backed range.

## Mapping issues to resolve before linking

- `.reginfo` occupies 24 bytes at `0x0031f420`, file offset `0x00220420`.
  Its address lies exactly in load 0's zero-fill tail. Section file payload exists,
  but that load does not declare it file-backed.
- Loads 2 and 3 overlap over 4,036 memory bytes. The former's zero-fill tail
  intersects 28 file-backed `.srsdtors` bytes and 4,008 zero-fill bytes in the latter.
  Allocated sections themselves do not overlap. Loader ordering therefore needs
  separate investigation; this tool does not simulate a loader.
- Loads 0 and 1 have respectively 8 and 144 file-backed bytes without section
  coverage. These can represent padding or unidentified content; they are not
  missing-source measurements.
- Startup tables `.ctors`, `.dtors`, `.srsctors` and `.srsdtors`, GP-relative
  sections, exception records, VU code and DVP overlay handling need an explicit
  build model. Nonallocation alone does not justify removing a section.
- `.loadScreenSection` declares 1,147,392 writable file-backed bytes at
  `0x01e00000`. Its semantic ownership remains unresolved.

Original linker/script identity, initialization order, general ABI, and the role
of nonallocated overlays remain unknown. No linker script, linked reconstruction
or startup success is implied by this inventory. See the [completion criteria](COMPLETION.md).

## Reproduce privately

```sh
python3 tools/link_inventory.py work/private/SLUS_205.65 --expected-sha256 9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0 --output work/new-link-layout.json
python3 -m unittest discover -s tools -p 'test_link_inventory.py' -v
```

The command requires the exact hash, uses half-open ranges, and refuses to replace
an existing output. The public report contains metadata only; original binaries,
raw tool output and independent comparison records remain private.
