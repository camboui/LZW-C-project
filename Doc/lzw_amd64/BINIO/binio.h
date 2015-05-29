#ifndef _BINIO_H_
#define _BINIO_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef MAX_ESBIN
#define MAX_ESBIN 64
#endif

typedef int_fast64_t binio_long_t;
typedef uint_fast64_t binio_ulong_t;

typedef enum {BINIO_OK=0, 
              BINIO_EOF=-1, 
              BINIO_BADESBIN=-2,
              BINIO_MAXESBIN=-3,
              BINIO_BADVALUE=-4,
              BINIO_IOERROR=-5} 
        binio_error_t; 

typedef void * binio_file_func_param_t;
typedef int binio_fputc_t (int,binio_file_func_param_t);    // EOF on error
typedef int binio_fgetc_t (binio_file_func_param_t);   // EOF on error
typedef int binio_eof_t (binio_file_func_param_t);       // non zero on end of file

// Debugging
// Minimal value is defined by BINIO_DEBUG_LEVEL environment variable
void set_debug_level (unsigned int value);

// bit stream descriptor type
typedef struct _binio *binio_t;

// Reverse bit order of an  integer
binio_ulong_t binio_reverse_bits (binio_ulong_t value, unsigned int bitsize);

// Init binio library
// max_open : limit on  simultaneous binio stream 
//            upperbound : MAX_ESBIN  (default value for max_open=0)
// Return values {BINIO_OK, BINIO_MAXESBIN} 

binio_error_t binio_init (unsigned int max_open);

// Open an input bit stream
//
// If pack_msb_first  (bit stream bytes filled form left to right)
//      + bytes filled most significant bits first 
//
// Case !bit_stream_write :  bit stream --> integer stream
//      + read_func  : read a byte from bit stream
// Case  bit_stream_write :  integer stream -->  bit stream
//      + write_func : write a byte to bit stream
//
// Return NULL on error

binio_t binio_read_open (binio_fgetc_t read_func,
                             binio_eof_t eof_func,
                             binio_file_func_param_t func_param,
                             bool pack_msb_first);

binio_t binio_write_open (binio_fputc_t write_func,
                             binio_eof_t eof_func,
                             binio_file_func_param_t func_param,
                             bool pack_msb_first);



// Close and free a bit stream
// On output, fill pending bits and flush last byte to bit stream
void binio_close (binio_t binio);

// Return number ([0,7]) of pending bits 
// + bits not extracted from last byte read from bit stream
// + bits written still not flushed to output stream (write
//   8-pending more bits to flush next byte to bit stream)
// Return <0 on error (BAD_ESBIN)

int binio_pending_bits (binio_t binio);


// Read/write integer from/to bit stream
// If reverse_bit_order 
//     integer bits in LSB to MSB order
// Return values : 
//   {BINIO_OK, BINIO_BADESBIN, BINIO_BADVALUE, BINIO_IOERROR, BINIO_EOF}

binio_error_t binio_read (binio_t binio,
                          binio_long_t *value,
                          unsigned int bitsize,
                          bool reverse_bit_order);

binio_error_t binio_unsigned_read (binio_t binio,
                          binio_ulong_t *value,
                          unsigned int bitsize,
                          bool reverse_bit_order);

binio_error_t binio_write (binio_t binio,
                          binio_long_t value,
                          unsigned int bitsize,
                          bool reverse_bit_order);

binio_error_t binio_unsigned_write (binio_t binio,
                          binio_ulong_t value,
                          unsigned int bitsize,
                          bool reverse_bit_order);

#endif  //_BINIO_H_
