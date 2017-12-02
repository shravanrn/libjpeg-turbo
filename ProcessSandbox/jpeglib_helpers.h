#pragma once

#include <stdio.h>  // FILE*
#include "jpeglib.h"

// Callback signatures
typedef void (*t_my_error_exit) (j_common_ptr cinfo);
typedef void (*t_init_source) (j_decompress_ptr jd);
typedef void (*t_skip_input_data) (j_decompress_ptr jd, long num_bytes);
typedef boolean (*t_fill_input_buffer) (j_decompress_ptr jd);
typedef void (*t_term_source) (j_decompress_ptr jd);
typedef boolean (*t_jpeg_resync_to_restart) (j_decompress_ptr cinfo, int desired);

// All types used by library functions must be a single preprocessor token, so that
//   they play nice with macros
// This means that we can't have two-word types like "struct x", nor types that contain *
typedef struct jpeg_error_mgr* JPEG_ERROR_MGR;
typedef FILE* FILESTAR;
typedef char* CHARSTAR;
typedef void* VOIDSTAR;
typedef unsigned char* UCHARSTAR;
typedef unsigned char** UCHARSTARSTAR;
typedef unsigned long ULONG;
typedef unsigned long* ULONGSTAR;

// Expects a macro that itself expects:
//   - function "number" (unique ID for the function, assigned by us)
//   - function name
//   - return type
//   - additional arguments: type of each parameter
#define FOR_EACH_LIBRARY_FUNCTION(macro) \
  FOR_EACH_1ARG_LIBRARY_FUNCTION(macro) \
  FOR_EACH_2ARG_LIBRARY_FUNCTION(macro) \
  FOR_EACH_3ARG_LIBRARY_FUNCTION(macro) \
  FOR_EACH_4ARG_LIBRARY_FUNCTION(macro) \
  FOR_EACH_5ARG_LIBRARY_FUNCTION(macro)

// Expects a macro that itself expects:
//   - function "number" (unique ID for the function, assigned by us)
//   - function name
//   - return type
//   - argument type
#define FOR_EACH_1ARG_LIBRARY_FUNCTION(macro) \
  FOR_EACH_1ARG_VOID_LIBRARY_FUNCTION(macro) \
  FOR_EACH_1ARG_NONVOID_LIBRARY_FUNCTION(macro)

// Expects a macro that itself expects:
//   - function "number" (unique ID for the function, assigned by us)
//   - function name
//   - return type
//   - argument 1 type
//   - argument 2 type
#define FOR_EACH_2ARG_LIBRARY_FUNCTION(macro) \
  FOR_EACH_2ARG_VOID_LIBRARY_FUNCTION(macro) \
  FOR_EACH_2ARG_NONVOID_LIBRARY_FUNCTION(macro)

// Expects a macro that itself expects:
//   - function "number" (unique ID for the function, assigned by us)
//   - function name
//   - return type
//   - argument 1 type
//   - argument 2 type
//   - argument 3 type
#define FOR_EACH_3ARG_LIBRARY_FUNCTION(macro) \
  FOR_EACH_3ARG_VOID_LIBRARY_FUNCTION(macro) \
  FOR_EACH_3ARG_NONVOID_LIBRARY_FUNCTION(macro)

// Expects a macro that itself expects:
//   - function "number" (unique ID for the function, assigned by us)
//   - function name
//   - return type
//   - argument 1 type
//   - argument 2 type
//   - argument 3 type
//   - argument 4 type
#define FOR_EACH_4ARG_LIBRARY_FUNCTION(macro) \
  FOR_EACH_4ARG_VOID_LIBRARY_FUNCTION(macro) \
  FOR_EACH_4ARG_NONVOID_LIBRARY_FUNCTION(macro)

// Expects a macro that itself expects:
//   - function "number" (unique ID for the function, assigned by us)
//   - function name
//   - return type
//   - argument 1 type
//   - argument 2 type
//   - argument 3 type
//   - argument 4 type
//   - argument 5 type
#define FOR_EACH_5ARG_LIBRARY_FUNCTION(macro) \
  FOR_EACH_5ARG_VOID_LIBRARY_FUNCTION(macro) \
  FOR_EACH_5ARG_NONVOID_LIBRARY_FUNCTION(macro)

// Like FOR_EACH_1ARG_LIBRARY_FUNCTION, but only void functions
#define FOR_EACH_1ARG_VOID_LIBRARY_FUNCTION(macro) \
  macro(4, jpeg_set_defaults, void, j_compress_ptr) \
  macro(8, jpeg_finish_compress, void, j_compress_ptr) \
  macro(9, jpeg_destroy_compress, void, j_compress_ptr) \
  macro(16, jpeg_destroy_decompress, void, j_decompress_ptr) \
  macro(19, jpeg_calc_output_dimensions, void, j_decompress_ptr)

// Like FOR_EACH_1ARG_LIBRARY_FUNCTION, but only nonvoid functions
#define FOR_EACH_1ARG_NONVOID_LIBRARY_FUNCTION(macro) \
  macro(1, jpeg_std_error, JPEG_ERROR_MGR, JPEG_ERROR_MGR) \
  macro(13, jpeg_start_decompress, boolean, j_decompress_ptr) \
  macro(15, jpeg_finish_decompress, boolean, j_decompress_ptr) \
  macro(18, jpeg_has_multiple_scans, boolean, j_decompress_ptr) \
  macro(21, jpeg_finish_output, boolean, j_decompress_ptr) \
  macro(22, jpeg_input_complete, boolean, j_decompress_ptr) \
  macro(23, jpeg_consume_input, int, j_decompress_ptr) \
  /*macro(26, my_error_exit, t_my_error_exit, j_common_ptr)*/ \
  /*macro(27, init_source, t_init_source, j_decompress_ptr)*/ \
  /*macro(29, fill_input_buffer, t_fill_input_buffer, j_decompress_ptr)*/ \
  /*macro(30, term_source, t_term_source, j_decompress_ptr)*/

// Like FOR_EACH_2ARG_LIBRARY_FUNCTION, but only void functions
#define FOR_EACH_2ARG_VOID_LIBRARY_FUNCTION(macro) \
  macro(3, jpeg_stdio_dest, void, j_compress_ptr, FILESTAR) \
  macro(6, jpeg_start_compress, void, j_compress_ptr, boolean) \
  macro(11, jpeg_stdio_src, void, j_decompress_ptr, FILESTAR)

// Like FOR_EACH_2ARG_LIBRARY_FUNCTION, but only nonvoid functions
#define FOR_EACH_2ARG_NONVOID_LIBRARY_FUNCTION(macro) \
  macro(12, jpeg_read_header, int, j_decompress_ptr, boolean) \
  macro(20, jpeg_start_output, boolean, j_decompress_ptr, int) \
  /*macro(28, skip_input_data, t_skip_input_data, j_decompress_ptr, long)*/ \
  /*macro(31, jpeg_resync_to_restart, t_jpeg_resync_to_restart, j_decompress_ptr, int)*/

// Like FOR_EACH_3ARG_LIBRARY_FUNCTION, but only void functions
#define FOR_EACH_3ARG_VOID_LIBRARY_FUNCTION(macro) \
  macro(2, jpeg_CreateCompress, void, j_compress_ptr, int, size_t) \
  macro(5, jpeg_set_quality, void, j_compress_ptr, int, boolean) \
  macro(10, jpeg_CreateDecompress, void, j_decompress_ptr, int, size_t) \
  macro(17, jpeg_save_markers, void, j_decompress_ptr, int, unsigned) \
  /*macro(25, format_message, void, VOIDSTAR, j_common_ptr, CHARSTAR)*/ \
	macro(32, jpeg_mem_dest, void, j_compress_ptr, UCHARSTARSTAR, ULONGSTAR) \
	macro(33, jpeg_mem_src, void, j_decompress_ptr, UCHARSTAR, ULONG)

// Like FOR_EACH_3ARG_LIBRARY_FUNCTION, but only nonvoid functions
#define FOR_EACH_3ARG_NONVOID_LIBRARY_FUNCTION(macro) \
  macro(7, jpeg_write_scanlines, JDIMENSION, j_compress_ptr, JSAMPARRAY, JDIMENSION) \
  macro(14, jpeg_read_scanlines, JDIMENSION, j_decompress_ptr, JSAMPARRAY, JDIMENSION)

// Like FOR_EACH_4ARG_LIBRARY_FUNCTION, but only void functions
#define FOR_EACH_4ARG_VOID_LIBRARY_FUNCTION(macro) \

// Like FOR_EACH_4ARG_LIBRARY_FUNCTION, but only nonvoid functions
#define FOR_EACH_4ARG_NONVOID_LIBRARY_FUNCTION(macro) \

// Like FOR_EACH_5ARG_LIBRARY_FUNCTION, but only void functions
#define FOR_EACH_5ARG_VOID_LIBRARY_FUNCTION(macro) \

// Like FOR_EACH_5ARG_LIBRARY_FUNCTION, but only nonvoid functions
#define FOR_EACH_5ARG_NONVOID_LIBRARY_FUNCTION(macro) \
  macro(24, alloc_sarray, JSAMPARRAY, VOIDSTAR, j_common_ptr, int, JDIMENSION, JDIMENSION)

#define DECLARE_STRUCTARGS_1ARG(fnum, fname, rettype, arg1type) \
  struct fname##_args { \
    arg1type arg1; \
  };

#define DECLARE_STRUCTARGS_2ARG(fnum, fname, rettype, arg1type, arg2type) \
  struct fname##_args { \
    arg1type arg1; \
    arg2type arg2; \
  };

#define DECLARE_STRUCTARGS_3ARG(fnum, fname, rettype, arg1type, arg2type, arg3type) \
  struct fname##_args { \
    arg1type arg1; \
    arg2type arg2; \
    arg3type arg3; \
  };

#define DECLARE_STRUCTARGS_4ARG(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type) \
  struct fname##_args { \
    arg1type arg1; \
    arg2type arg2; \
    arg3type arg3; \
    arg4type arg4; \
  };

#define DECLARE_STRUCTARGS_5ARG(fnum, fname, rettype, arg1type, arg2type, arg3type, arg4type, arg5type) \
  struct fname##_args { \
    arg1type arg1; \
    arg2type arg2; \
    arg3type arg3; \
    arg4type arg4; \
    arg5type arg5; \
  };

FOR_EACH_1ARG_LIBRARY_FUNCTION(DECLARE_STRUCTARGS_1ARG)
FOR_EACH_2ARG_LIBRARY_FUNCTION(DECLARE_STRUCTARGS_2ARG)
FOR_EACH_3ARG_LIBRARY_FUNCTION(DECLARE_STRUCTARGS_3ARG)
FOR_EACH_4ARG_LIBRARY_FUNCTION(DECLARE_STRUCTARGS_4ARG)
FOR_EACH_5ARG_LIBRARY_FUNCTION(DECLARE_STRUCTARGS_5ARG)

#undef DECLARE_STRUCTARGS_1ARG
#undef DECLARE_STRUCTARGS_2ARG
#undef DECLARE_STRUCTARGS_3ARG
#undef DECLARE_STRUCTARGS_4ARG
#undef DECLARE_STRUCTARGS_5ARG

typedef union {
  // All possible return-value types from the library
  JPEG_ERROR_MGR as_JPEG_ERROR_MGR;
  JDIMENSION as_JDIMENSION;
  int as_int;
  boolean as_boolean;
  JSAMPARRAY as_JSAMPARRAY;
  /*
  t_my_error_exit as_t_my_error_exit;
  t_init_source as_t_init_source;
  t_skip_input_data as_t_skip_input_data;
  t_fill_input_buffer as_t_fill_input_buffer;
  t_term_source as_t_term_source;
  t_jpeg_resync_to_restart as_t_jpeg_resync_to_restart;
  */
} library_retval_union;

// alloc_sarray is not a normal function in libjpeg
//   but we want to call it like one
//   so we declare this wrapper at global scope
inline JSAMPARRAY alloc_sarray(void* unused, j_common_ptr cinfo, int pool_id, JDIMENSION samplesperrow, JDIMENSION numrows) {
	return cinfo->mem->alloc_sarray(cinfo, pool_id, samplesperrow, numrows);
}

/* As backup: manual declarations of all of the structs

struct jpeg_std_error_args {
  struct jpeg_error_mgr* err;
};

struct jpeg_CreateCompress_args {
  j_compress_ptr cinfo;
  int version;
  size_t structsize;
};

struct jpeg_stdio_dest_args {
  j_compress_ptr cinfo;
  FILE * outfile;
};

struct jpeg_set_defaults_args {
  j_compress_ptr cinfo;
};

struct jpeg_set_quality_args {
  j_compress_ptr cinfo;
  int quality;
  boolean force_baseline;
};

struct jpeg_start_compress_args {
  j_compress_ptr cinfo;
  boolean write_all_tables;
};

struct jpeg_write_scanlines_args {
  j_compress_ptr cinfo;
  JSAMPARRAY scanlines;
  JDIMENSION num_lines;
};

struct jpeg_finish_compress_args {
  j_compress_ptr cinfo;
};

struct jpeg_destroy_compress_args {
  j_compress_ptr cinfo;
};

struct jpeg_CreateDecompress_args {
  j_decompress_ptr cinfo;
  int version;
  size_t structsize;
};

struct jpeg_stdio_src_args {
  j_decompress_ptr cinfo;
  FILE * infile;
};

struct jpeg_read_header_args {
  j_decompress_ptr cinfo;
  boolean require_image;
};

struct jpeg_start_decompress_args {
  j_decompress_ptr cinfo;
};

struct jpeg_read_scanlines_args {
  j_decompress_ptr cinfo;
  JSAMPARRAY scanlines;
  JDIMENSION max_lines;
};

struct jpeg_finish_decompress_args {
  j_decompress_ptr cinfo;
};

struct jpeg_destroy_decompress_args {
  j_decompress_ptr cinfo;
};

struct jpeg_save_markers_args {
  j_decompress_ptr cinfo;
  int marker_code;
  unsigned int length_limit;
};

struct jpeg_has_multiple_scans_args {
  j_decompress_ptr cinfo;
};

struct jpeg_calc_output_dimensions_args {
  j_decompress_ptr cinfo;
};

struct jpeg_start_output_args {
  j_decompress_ptr cinfo;
  int scan_number;
};

struct jpeg_finish_output_args {
  j_decompress_ptr cinfo;
};

struct jpeg_input_complete_args {
  j_decompress_ptr cinfo;
};

struct jpeg_consume_input_args {
  j_decompress_ptr cinfo;
};

struct alloc_sarray_args {
  void* alloc_sarray;
  j_common_ptr cinfo;
  int pool_id;
  JDIMENSION samplesperrow;
  JDIMENSION numrows;
};

struct format_message_args {
  void* format_message;
  j_common_ptr cinfo;
  char* buffer;
};

struct my_error_exit_args {
  t_my_error_exit callback;
};

struct init_source_args {
  t_init_source callback;
};

struct skip_input_data_args {
  t_skip_input_data callback;
};

struct fill_input_buffer_args {
  t_fill_input_buffer callback;
};

struct term_source_args {
  t_term_source callback;
};

struct jpeg_resync_to_restart_args {
  t_jpeg_resync_to_restart callback;
};

*/

