Maldev
---
This repository will contain code about my malware development learning process

---
## Compilation
You will need Mingw to cross-compile from Linux:


**Debian / Ubuntu / Mint:**
```bash
sudo apt update
sudo apt install mingw-w64
```

**Arch Linux / Manjaro:**
```bash
sudo pacman -S mingw-w64-gcc
```

**Fedora / RHEL:**
```bash
sudo dnf install mingw64-gcc mingw64-gcc-c++
```

### Compiling the PoCs
Example with `process_injection.c`
```bash
x86_64-w64-mingw32-gcc process_injection.c -o pi.exe -lkernel32

```
