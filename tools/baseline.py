#!/usr/bin/env python3
"""Inspect a private PS2 ELF without copying its contents into a report.

This is a structural intake check, not a game identity or reconstruction verifier.
No third-party packages, network access, disassembly, or executable loading.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import struct
import sys


class IntakeError(ValueError):
    pass


def span(data, offset, size, label):
    if offset < 0 or size < 0 or offset > len(data) or size > len(data) - offset:
        raise IntakeError(f"{label} is outside the file")
    return data[offset:offset + size]


def address_span(address, size, label):
    if address + size > 0x100000000:
        raise IntakeError(f"{label} overflows the 32-bit address space")


def boot_metadata(text, executable_name):
    """Return only constrained boot metadata; never include arbitrary CNF text."""
    fields = {}
    for line in text.splitlines():
        if '=' not in line:
            continue
        key, value = (part.strip() for part in line.split('=', 1))
        key = key.upper()
        if key in {'BOOT2', 'VER', 'VMODE'}:
            if key in fields:
                raise IntakeError(f"Duplicate SYSTEM.CNF field: {key}")
            fields[key] = value
    match = re.fullmatch(r'cdrom0?:[\\/]+([A-Za-z0-9_.-]+);1', fields.get('BOOT2', ''), re.I)
    if not match:
        raise IntakeError("SYSTEM.CNF has no supported BOOT2 entry")
    name = match.group(1).upper()
    if name != executable_name.upper():
        raise IntakeError("SYSTEM.CNF BOOT2 does not match the executable filename")
    result = {'boot_executable': name}
    for key in ('VER', 'VMODE'):
        if key in fields:
            if not re.fullmatch(r'[A-Za-z0-9_.-]{1,32}', fields[key]):
                raise IntakeError(f"Unsupported SYSTEM.CNF value: {key}")
            result[key.lower()] = fields[key]
    return result


def inspect_elf(data):
    span(data, 0, 52, 'ELF header')
    if data[:7] != b'\x7fELF\x01\x01\x01':
        raise IntakeError('Expected ELF32 little-endian version 1')
    (kind, machine, version, entry, phoff, shoff, flags, ehsize,
     phsize, phnum, shsize, shnum, shstr) = struct.unpack_from('<HHIIIIIHHHHHH', data, 16)
    if kind != 2 or machine != 8 or version != 1 or ehsize != 52:
        raise IntakeError('Expected an executable MIPS ELF with a standard ELF32 header')
    if phnum == 0 or phnum == 0xffff or phsize != 32:
        raise IntakeError('Missing or unsupported program header table')
    if phoff < 52:
        raise IntakeError('Program header table overlaps the ELF header')
    span(data, phoff, phnum * phsize, 'Program header table')
    segments = []
    executable_entry = False
    for i in range(phnum):
        ptype, offset, vaddr, paddr, filesz, memsz, pflags, align = struct.unpack_from('<8I', data, phoff + i * phsize)
        span(data, offset, filesz, f'Segment {i}')
        address_span(vaddr, memsz, f'Segment {i}')
        if align not in (0, 1) and (align & (align - 1)):
            raise IntakeError(f'Segment {i} has a non-power-of-two alignment')
        if ptype == 1:
            if filesz > memsz:
                raise IntakeError(f'Segment {i} file size exceeds memory size')
            if align > 1 and vaddr % align != offset % align:
                raise IntakeError(f'Segment {i} has incongruent load alignment')
            if pflags & 1 and vaddr <= entry < vaddr + filesz:
                executable_entry = True
        segments.append(dict(index=i, type=ptype, offset=offset, virtual_address=vaddr,
                             physical_address=paddr, file_size=filesz, memory_size=memsz,
                             flags=pflags, alignment=align))
    if not executable_entry or entry % 4:
        raise IntakeError('Entry point is not aligned inside a file-backed executable load segment')
    sections = []
    if shoff == 0:
        if shnum != 0 or shstr != 0:
            raise IntakeError('Section counts exist without a section table')
    else:
        if shoff < 52 or shnum == 0 or shsize != 40 or shstr >= shnum:
            raise IntakeError('Unsupported section header table (extended numbering is not supported)')
        span(data, shoff, shnum * shsize, 'Section header table')
        headers = [struct.unpack_from('<10I', data, shoff + i * shsize) for i in range(shnum)]
        names = b''
        if shstr:
            table = headers[shstr]
            if table[1] != 3:
                raise IntakeError('Section name table is not a string table')
            names = span(data, table[4], table[5], 'Section name table')
        for i, (nameoff, stype, sflags, addr, offset, size, link, info, align, entsize) in enumerate(headers):
            if stype not in (0, 8):  # SHT_NULL and SHT_NOBITS have no file payload.
                span(data, offset, size, f'Section {i}')
            if sflags & 2:
                address_span(addr, size, f'Section {i}')
            if align not in (0, 1) and (align & (align - 1)):
                raise IntakeError(f'Section {i} has invalid alignment')
            name = None
            if nameoff:
                if nameoff >= len(names):
                    raise IntakeError(f'Section {i} has an invalid name offset')
                end = names.find(b'\0', nameoff)
                if end < 0:
                    raise IntakeError(f'Section {i} has an unterminated name')
                raw_name = names[nameoff:end]
                # Do not emit arbitrary embedded strings or filesystem paths.
                if re.fullmatch(rb'[A-Za-z0-9_.-]{1,64}', raw_name):
                    name = raw_name.decode('ascii')
            sections.append(dict(index=i, name=name, type=stype, flags=sflags,
                                 address=addr, offset=offset, size=size, link=link,
                                 info=info, alignment=align, entry_size=entsize))
    return {
        'schema_version': 1,
        'status': 'structurally_validated_candidate',
        'executable': {'sha256': hashlib.sha256(data).hexdigest(), 'size': len(data),
                       'machine': 'MIPS', 'class': 32, 'endianness': 'little',
                       'entry_point': entry, 'flags': flags},
        'segments': segments, 'sections': sections,
        'unknown': ['Game identity and exact disc edition require independent evidence.',
                    'Compiler, ABI and function boundaries are not established.',
                    'No reconstructed-function count or runtime compatibility is established.'],
    }


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('executable', type=Path)
    parser.add_argument('--system-cnf', type=Path)
    parser.add_argument('--output', type=Path, help='New local report; existing files are never overwritten')
    args = parser.parse_args(argv)
    try:
        if args.executable.name.lower().endswith(('.crdownload', '.part', '.download')):
            raise IntakeError('Download is incomplete')
        with args.executable.open('rb') as source:
            before = os.fstat(source.fileno())
            if before.st_size > 128 * 1024 * 1024:
                raise IntakeError('Executable exceeds the 128 MiB intake limit; pass the extracted ELF, not a disc image')
            data = source.read()
            after = os.fstat(source.fileno())
        if (before.st_size, before.st_mtime_ns) != (after.st_size, after.st_mtime_ns) or len(data) != after.st_size:
            raise IntakeError('Executable changed during intake')
        report = inspect_elf(data)
        if args.system_cnf:
            report['boot'] = boot_metadata(args.system_cnf.read_text(encoding='ascii'), args.executable.name)
        rendered = json.dumps(report, indent=2) + '\n'
        if args.output:
            with args.output.open('x', encoding='utf-8') as output:
                output.write(rendered)
        else:
            sys.stdout.write(rendered)
    except (OSError, ValueError) as error:
        print(f'Intake failed: {error}', file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
