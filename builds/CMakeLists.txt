cmake_minimum_required(VERSION 3.11)

set(CMAKE_BUILD_TYPE Release)

option(BUILD_WASM "Build WASM" OFF)

if(NOT DEFINED ENV{CONFIG_DIR})
  message(FATAL_ERROR "Set CONFIG_DIR environment variable")
endif()

set(CONFIG_DIR $ENV{CONFIG_DIR})

if (BUILD_WASM)
        if(NOT DEFINED ENV{wasiclang_SOURCE_DIR})
                message(FATAL_ERROR "Set wasiclang_SOURCE_DIR environment variable")
        endif()

        if(NOT DEFINED ENV{LUCET_WASI_DIR})
                message(FATAL_ERROR "Set LUCET_WASI_DIR environment variable")
        endif()

        if(NOT DEFINED ENV{LUCET_DIR})
                message(FATAL_ERROR "Set LUCET_DIR environment variable")
        endif()

        set(wasiclang_SOURCE_DIR $ENV{wasiclang_SOURCE_DIR})
        set(LUCET_WASI_DIR $ENV{LUCET_WASI_DIR})
        set(LUCET_DIR $ENV{LUCET_DIR})

        set(CMAKE_C_COMPILER ${wasiclang_SOURCE_DIR}/opt/wasi-sdk/bin/clang)
        set(CMAKE_CXX_COMPILER ${wasiclang_SOURCE_DIR}/opt/wasi-sdk/bin/clang++)
        # Apply settings suitable for wasm module compilation
        set(CMAKE_C_FLAGS "--sysroot ${wasiclang_SOURCE_DIR}/opt/wasi-sdk/share/wasi-sysroot/")
        set(CMAKE_CXX_FLAGS "--sysroot ${wasiclang_SOURCE_DIR}/opt/wasi-sdk/share/wasi-sysroot/")
        # set(CMAKE_EXE_LINKER_FLAGS "-Wl,--export-all")

        set(OUTPUT_EXEC_PATH "example_wasm_sbx.wasm")
else()
        set(OUTPUT_EXEC_PATH "example_native")
endif()

add_executable("${OUTPUT_EXEC_PATH}"
               ${CMAKE_SOURCE_DIR}/../example_wasm.cpp
               ${CMAKE_SOURCE_DIR}/../jcapimin.c
               ${CMAKE_SOURCE_DIR}/../jcapistd.c
               ${CMAKE_SOURCE_DIR}/../jccoefct.c
               ${CMAKE_SOURCE_DIR}/../jccolor.c
               ${CMAKE_SOURCE_DIR}/../jcdctmgr.c
               ${CMAKE_SOURCE_DIR}/../jchuff.c
               ${CMAKE_SOURCE_DIR}/../jcinit.c
               ${CMAKE_SOURCE_DIR}/../jcmainct.c
               ${CMAKE_SOURCE_DIR}/../jcmarker.c
               ${CMAKE_SOURCE_DIR}/../jcmaster.c
               ${CMAKE_SOURCE_DIR}/../jcomapi.c
               ${CMAKE_SOURCE_DIR}/../jcparam.c
               ${CMAKE_SOURCE_DIR}/../jcphuff.c
               ${CMAKE_SOURCE_DIR}/../jcprepct.c
               ${CMAKE_SOURCE_DIR}/../jcsample.c
               ${CMAKE_SOURCE_DIR}/../jctrans.c
               ${CMAKE_SOURCE_DIR}/../jdapimin.c
               ${CMAKE_SOURCE_DIR}/../jdapistd.c
               ${CMAKE_SOURCE_DIR}/../jdatadst.c
               ${CMAKE_SOURCE_DIR}/../jdatasrc.c
               ${CMAKE_SOURCE_DIR}/../jdcoefct.c
               ${CMAKE_SOURCE_DIR}/../jdcolor.c
               ${CMAKE_SOURCE_DIR}/../jddctmgr.c
               ${CMAKE_SOURCE_DIR}/../jdhuff.c
               ${CMAKE_SOURCE_DIR}/../jdinput.c
               ${CMAKE_SOURCE_DIR}/../jdmainct.c
               ${CMAKE_SOURCE_DIR}/../jdmarker.c
               ${CMAKE_SOURCE_DIR}/../jdmaster.c
               ${CMAKE_SOURCE_DIR}/../jdmerge.c
               ${CMAKE_SOURCE_DIR}/../jdphuff.c
               ${CMAKE_SOURCE_DIR}/../jdpostct.c
               ${CMAKE_SOURCE_DIR}/../jdsample.c
               ${CMAKE_SOURCE_DIR}/../jdtrans.c
               ${CMAKE_SOURCE_DIR}/../jerror.c
               ${CMAKE_SOURCE_DIR}/../jfdctflt.c
               ${CMAKE_SOURCE_DIR}/../jfdctfst.c
               ${CMAKE_SOURCE_DIR}/../jfdctint.c
               ${CMAKE_SOURCE_DIR}/../jidctflt.c
               ${CMAKE_SOURCE_DIR}/../jidctfst.c
               ${CMAKE_SOURCE_DIR}/../jidctint.c
               ${CMAKE_SOURCE_DIR}/../jidctred.c
               ${CMAKE_SOURCE_DIR}/../jquant1.c
               ${CMAKE_SOURCE_DIR}/../jquant2.c
               ${CMAKE_SOURCE_DIR}/../jutils.c
               ${CMAKE_SOURCE_DIR}/../jmemmgr.c
               ${CMAKE_SOURCE_DIR}/../jmemnobs.c
               ${CMAKE_SOURCE_DIR}/../jsimd_none.c
               ${CMAKE_SOURCE_DIR}/../jaricom.c
               ${CMAKE_SOURCE_DIR}/../jcarith.c
               ${CMAKE_SOURCE_DIR}/../jdarith.c
               )

target_include_directories("${OUTPUT_EXEC_PATH}" PUBLIC ${CMAKE_SOURCE_DIR}/../
                                                        ${CONFIG_DIR})

if (BUILD_WASM)
        set(OUTPUT_SO_PATH "example_wasm_sbx.so")
        add_custom_command(OUTPUT ${OUTPUT_SO_PATH}
                        DEPENDS example_wasm_sbx.wasm
                        COMMAND ${LUCET_DIR}/lucetc
                                --bindings
                                ${LUCET_WASI_DIR}/bindings.json
                                --guard-size
                                "4GiB"
                                --min-reserved-size
                                "4GiB"
                                --max-reserved-size
                                "4GiB"
                                example_wasm_sbx.wasm
                                -o
                                ${OUTPUT_SO_PATH}
                        COMMENT "Compiling wasm file to native")

        add_custom_target(glue_lib_so ALL DEPENDS ${OUTPUT_SO_PATH})
endif()

