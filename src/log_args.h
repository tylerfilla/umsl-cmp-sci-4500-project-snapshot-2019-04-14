/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef _char
#define _char(x) {.as_char=(x),.kind=log_record_arg_kind_char}
#endif

#ifndef _signed_char
#define _signed_char(x) {.as_signed_char=(x),.kind=log_record_arg_kind_signed_char}
#endif

#ifndef _unsigned_char
#define _unsigned_char(x) {.as_unsigned_char=(x),.kind=log_record_arg_kind_unsigned_char}
#endif

#ifndef _signed_short_int
#define _signed_short_int(x) {.as_signed_short_int=(x),.kind=log_record_arg_kind_signed_short_int}
#endif

#ifndef _unsigned_short_int
#define _unsigned_short_int(x) {.as_unsigned_short_int=(x),.kind=log_record_arg_kind_unsigned_short_int}
#endif

#ifndef _signed_int
#define _signed_int(x) {.as_signed_int=(x),.kind=log_record_arg_kind_signed_int}
#endif

#ifndef _unsigned_int
#define _unsigned_int(x) {.as_unsigned_int=(x),.kind=log_record_arg_kind_unsigned_int}
#endif

#ifndef _signed_long_int
#define _signed_long_int(x) {.as_signed_long_int=(x),.kind=log_record_arg_kind_signed_long_int}
#endif

#ifndef _unsigned_long_int
#define _unsigned_long_int(x) {.as_unsigned_long_int=(x),.kind=log_record_arg_kind_unsigned_long_int}
#endif

#ifndef _signed_long_long_int
#define _signed_long_long_int(x) {.as_signed_long_long_int=(x),.kind=log_record_arg_kind_signed_long_long_int}
#endif

#ifndef _unsigned_long_long_int
#define _unsigned_long_long_int(x) {.as_unsigned_long_long_int=(x),.kind=log_record_arg_kind_unsigned_long_long_int}
#endif

#ifndef _float
#define _float(x) {.as_float=(x),.kind=log_record_arg_kind_float}
#endif

#ifndef _double
#define _double(x) {.as_double=(x),.kind=log_record_arg_kind_double}
#endif

#ifndef _long_double
#define _long_double(x) {.as_long_double=(x),.kind=log_record_arg_kind_long_double}
#endif

#ifndef _c_string
#define _c_string(x) {.as_c_string=(x),.kind=log_record_arg_kind_c_string}
#endif

#ifndef LOG_ARGS_NO_SHORT

#ifndef _c
#define _c(...) _char(__VA_ARGS__)
#endif

#ifndef _sc
#define _sc(...) _signed_char(__VA_ARGS__)
#endif

#ifndef _uc
#define _uc(...) _unsigned_char(__VA_ARGS__)
#endif

#ifndef _s
#define _s(...) _signed_short_int(__VA_ARGS__)
#endif

#ifndef _us
#define _us(...) _unsigned_short_int(__VA_ARGS__)
#endif

#ifndef _i
#define _i(...) _signed_int(__VA_ARGS__)
#endif

#ifndef _ui
#define _ui(...) _unsigned_int(__VA_ARGS__)
#endif

#ifndef _l
#define _l(...) _signed_long_int(__VA_ARGS__)
#endif

#ifndef _ul
#define _ul(...) _unsigned_long_int(__VA_ARGS__)
#endif

#ifndef _ll
#define _ll(...) _signed_long_long_int(__VA_ARGS__)
#endif

#ifndef _ull
#define _ull(...) _unsigned_long_long_int(__VA_ARGS__)
#endif

#ifndef _f
#define _f(...) _float(__VA_ARGS__)
#endif

#ifndef _d
#define _d(...) _double(__VA_ARGS__)
#endif

#ifndef _ld
#define _ld(...) _long_double(__VA_ARGS__)
#endif

#ifndef _str
#define _str(...) _c_string(__VA_ARGS__)
#endif

#endif // #ifndef LOG_ARGS_NO_SHORT
