# Establishing a baseline

The experimental SLUS-20565 target is identified by its exact executable SHA-256;
its retail revision remains unresolved. See [baseline evidence](../docs/BASELINE.md)
and the [reconstruction workflow](../docs/REBUILD.md). Every target must independently record:

- Disc region, release/version and executable filename from the contributor's copy.
- Executable SHA-256, ELF machine, entry point, section bounds and load layout.
- Reproducible function inventory and its limits; validate the first candidate's boundary manually.
- Compiler, ABI, relocation and linker evidence, including unresolved questions.
- Private input paths and acquisition steps in local notes, never published original files.
- A sanitized verification receipt for the first complete source-derived function.

Keep edition-specific targets separate. Do not put placeholder zeros or the Return to
Arms executable hash into a manifest to make a verifier run. A missing baseline means
progress is unmeasured; it does not mean 0 of Return to Arms' 7,732 functions.

The [Return to Arms workflow](https://github.com/Open-Champions/champions-return-to-arms/blob/main/docs/REBUILD.md)
is a method reference. Its addresses, formats and compiler assumptions must be verified
independently for this game before reuse.

## Local executable intake

With Python 3.10 or newer, inspect the extracted executable and its corresponding
disc boot configuration. Store both under ignored `work/private/` or outside this
repository. Never pass a compressed archive, partial download or entire ISO as the
executable.

```sh
python3 tools/baseline.py work/private/EXECUTABLE --system-cnf work/private/SYSTEM.CNF --output work/intake.json
python3 -m unittest discover -s tools -p 'test_*.py' -v
```

Replace `EXECUTABLE` with the filename from that disc's `BOOT2` entry. The command
checks ELF32 little-endian MIPS structure, file bounds, load alignment, entry-point
mapping, section metadata and the boot filename. It records a SHA-256 and layout;
it does not run the executable or publish its bytes. Existing reports are never
overwritten. Extended ELF numbering is deliberately unsupported. Sectionless
executables are supported but cannot supply section-based evidence.

A passing report is only a **structurally validated candidate**. `SYSTEM.CNF`'s
`VER` field is recorded as boot metadata, not assumed to identify a retail revision.
Confirm the game and disc edition independently before claiming a retail release.
An experimental hash-pinned target can identify exact bytes while preserving the
retail-edition uncertainty explicitly, as this target does. The intake tool
does not infer compiler identity, function boundaries, match percentages or native
compatibility. Review the generated metadata before committing any baseline.
