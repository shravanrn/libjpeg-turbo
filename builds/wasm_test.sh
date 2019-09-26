#!/bin/bash
# set -x
wasiclang_SOURCE_DIR="/home/shr/Code/LibrarySandboxing/rlbox_lucet_sandbox/build/_deps/wasiclang-src"
LUCET_WASI_DIR="/home/shr/Code/LibrarySandboxing/lucet_sandbox_compiler/lucet-wasi"
LUCET_DIR="/home/shr/Code/LibrarySandboxing/lucet_sandbox_compiler/target/debug/"

CONFIG_DIR="$(realpath ./native_wasm_test/configs_native)" \
cmake -DBUILD_WASM=OFF -S ./ -B ./native_wasm_test/native
cmake --build ./native_wasm_test/native -- -j8

CONFIG_DIR="$(realpath ./native_wasm_test/configs_wasm)" \
wasiclang_SOURCE_DIR=$wasiclang_SOURCE_DIR \
LUCET_WASI_DIR=$LUCET_WASI_DIR \
LUCET_DIR=$LUCET_DIR \
cmake -DBUILD_WASM=ON -S ./ -B ./native_wasm_test/wasm
cmake --build ./native_wasm_test/wasm -- -j8

echo "!!!!!NATIVE!!!!!!!"
./native_wasm_test/native/example_native

echo "!!!!!WASM!!!!!!!"
$LUCET_DIR/lucet-wasi ./native_wasm_test/wasm/example_wasm_sbx.so