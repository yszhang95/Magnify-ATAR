# Magnify-tracking

```
./magnify [file]
```
## Build
Please supply path to `cmake` directory for `ROOT`. Files,
such as `ROOTConfig.cmake`, are hosted under this directory.
Another way to avoid `ROOT_DIR` is to sourcing `thisroot.[c]sh`.
```shell
cmake -S . -B build/ -DROOT_DIR=/path/to/root/cmake/  -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake --build build
build/magnify-aTAR
```
