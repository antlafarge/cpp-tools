# cpp-tools

Set of cpp tools

## Sub-Projects

- [Base64](https://github.com/antlafarge/cpp-tools/tree/main/Base64)
- [ConcurrentResource](https://github.com/antlafarge/cpp-tools/tree/main/Concurrent)
- [UUID](https://github.com/antlafarge/cpp-tools/tree/main/UUID)
- [JSON](https://github.com/antlafarge/cpp-tools/tree/main/JSON)

## Visual studio code C++ installation

https://code.visualstudio.com/docs/cpp/config-mingw

- Download and install [MSYS2](https://www.msys2.org/)
- In MSYS2 command shell :
    - Run `pacman -Syu`
    - restart MSYS2 from `"C:\msys64\msys2.exe"`
    - Re-run `pacman -Syu`
    - Run `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`
- Add `C:\msys64\mingw64\bin` to your PATH environment variable
- Restart shells
- Test
    - `g++ --version`
    - `gdb --version`

## WSL 2

- Run `wsl --install -d Ubuntu` in powershell (elevated)
- Reboot Windows
- Fill in Ubuntu credentials
- Full update Ubuntu `sudo apt update && sudo apt full-upgrade -y && sudo apt autoremove -y`
- Install g++ `sudo apt install g++ -y`
- Install g++ (for ppc64) `sudo apt install g++-powerpc64-linux-gnu qemu-user -y`
- Test
    - `g++ --version`
    - `powerpc64-linux-gnu-g++ --version`

### Default G++ STD version

`g++ -dM -E -x c++  /dev/null | grep -F __cplusplus`
