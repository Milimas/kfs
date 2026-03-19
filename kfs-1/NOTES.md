# Linker

how to check segments permissions

```bash
readelf -l kernel.elf

# More detail per section
readelf -s kernel.elf

# Or
objdump -h kernel.elf
```

