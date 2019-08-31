
@echo off

pushd dist

clang --target=wasm32 -Os -nostdlib -Wl,-z,stack-size=2097152 -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -o main.wasm ../code/main.c

popd