#pragma once

#include "synch.h"
#include <stdint.h>
#include "jpeglib_helpers.h"

static const char* othersidepath = "../ProcessSandbox/ProcessSandbox_otherside";
static const char* shmempath = "sharedmem";

#define KBYTE 1024
#define MBYTE (KBYTE*KBYTE)
#define GBYTE (MBYTE*KBYTE)

#define SHAREDMEM_VADDR ((void*)0xA0000000)

#ifdef __amd64__  // 64-bit
#define SHAREDMEM_SIZE ((uintptr_t)2*GBYTE)  /* allocated/paged-in on demand */
#else
#define SHAREDMEM_SIZE ((uintptr_t)256*MBYTE)  /* allocated/paged-in on demand */
#endif

typedef struct {
    Invoker invoker;
    unsigned funcnum;
    union {
      // All possible args-structs
#define ARGS(fnum, fname, ...) struct fname##_args fname##_args;
      FOR_EACH_LIBRARY_FUNCTION(ARGS)
#undef ARGS
      /* Backup: Manual declaration of all of these structs
      struct jpeg_std_error_args jpeg_std_error_args;
      struct jpeg_CreateCompress_args jpeg_CreateCompress_args;
      struct jpeg_stdio_dest_args jpeg_stdio_dest_args;
      struct jpeg_set_defaults_args jpeg_set_defaults_args;
      struct jpeg_set_quality_args jpeg_set_quality_args;
      struct jpeg_start_compress_args jpeg_start_compress_args;
      struct jpeg_write_scanlines_args jpeg_write_scanlines_args;
      struct jpeg_finish_compress_args jpeg_finish_compress_args;
      struct jpeg_destroy_compress_args jpeg_destroy_compress_args;
      struct jpeg_CreateDecompress_args jpeg_CreateDecompress_args;
      struct jpeg_stdio_src_args jpeg_stdio_src_args;
      struct jpeg_read_header_args jpeg_read_header_args;
      struct jpeg_start_decompress_args jpeg_start_decompress_args;
      struct jpeg_read_scanlines_args jpeg_read_scanlines_args;
      struct jpeg_finish_decompress_args jpeg_finish_decompress_args;
      struct jpeg_destroy_decompress_args jpeg_destroy_decompress_args;
      struct jpeg_save_markers_args jpeg_save_markers_args;
      struct jpeg_has_multiple_scans_args jpeg_has_multiple_scans_args;
      struct jpeg_calc_output_dimensions_args jpeg_calc_output_dimensions_args;
      struct jpeg_start_output_args jpeg_start_output_args;
      struct jpeg_finish_output_args jpeg_finish_output_args;
      struct jpeg_input_complete_args jpeg_input_complete_args;
      struct jpeg_consume_input_args jpeg_consume_input_args;
      struct alloc_sarray_args alloc_sarray_args;
      struct format_message_args format_message_args;
      struct my_error_exit_args my_error_exit_args;
      struct init_source_args init_source_args;
      struct skip_input_data_args skip_input_data_args;
      struct fill_input_buffer_args fill_input_buffer_args;
      struct term_source_args term_source_args;
      struct jpeg_resync_to_restart_args jpeg_resync_to_restart_args;
      */
    };
    library_retval_union retval;
    uint8_t extraSpace[0];  // space to use for mallocInSandbox()
} sharedmem_t;
