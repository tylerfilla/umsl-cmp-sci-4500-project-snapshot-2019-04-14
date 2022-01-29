/*
 * Cozmonaut
 * Copyright 2019 The Cozmonaut Contributors
 */

#ifndef LOG_H
#define LOG_H

#include <stddef.h>

/** A thread-safe log. */
struct log;

/** A log context. */
struct log_context;

/** Attributes of a log. */
struct log_attrs {
  /** The log name. */
  const char* name;
};

/**
 * Create a log.
 *
 * @param attrs The log attributes
 * @return The log
 */
struct log* log_new(struct log_attrs* attrs);

/**
 * Destroy a log.
 *
 * @param log The log
 */
void log_delete(struct log* log);

/**
 * Lock a log on the calling thread.
 *
 * Generally, you shouldn't need to call this.
 *
 * @param log The log
 */
void log_lock(struct log* log);

/**
 * Unlock a log on the calling thread.
 *
 * Generally, you shouldn't need to call this.
 *
 * @param log The log
 */
void log_unlock(struct log* log);

/**
 * Register a log context with a log.
 *
 * A single log context may belong to multiple logs.
 *
 * @param log The log
 * @param ctx The log context
 */
void log_register_context(struct log* log, struct log_context* ctx);

/**
 * Unregister a log context from a log.
 *
 * @param log The log
 * @param ctx The log context
 */
void log_unregister_context(struct log* log, struct log_context* ctx);

/** Attributes of a log context. */
struct log_context_attrs {
  /** The context name. */
  const char* name;
};

/**
 * Create a log context.
 *
 * @param attrs The context attributes
 * @return The log context
 */
struct log_context* log_context_new(struct log_context_attrs* attrs);

/**
 * Destroy a log context.
 *
 * @param ctx The log context
 */
void log_context_destroy(struct log_context* ctx);

/**
 * Make a log context current on the calling thread.
 *
 * Call with NULL to explicitly remove the current context.
 *
 * @param ctx The log context
 */
void log_context_make_current(struct log_context* ctx);

/** The severity level of a log record. */
enum log_level {
  log_level_trace,
  log_level_debug,
  log_level_info,
  log_level_warn,
  log_level_error,
  log_level_fatal,
};

/** A kind of log record argument. */
enum log_record_arg_kind {
  log_record_arg_kind_char,
  log_record_arg_kind_signed_char,
  log_record_arg_kind_unsigned_char,
  log_record_arg_kind_signed_short_int,
  log_record_arg_kind_unsigned_short_int,
  log_record_arg_kind_signed_int,
  log_record_arg_kind_unsigned_int,
  log_record_arg_kind_signed_long_int,
  log_record_arg_kind_unsigned_long_int,
  log_record_arg_kind_signed_long_long_int,
  log_record_arg_kind_unsigned_long_long_int,
  log_record_arg_kind_float,
  log_record_arg_kind_double,
  log_record_arg_kind_long_double,
  log_record_arg_kind_c_string,
};

/** A log record format argument. */
struct log_record_arg {
  union {
    /** View as a char. */
    char as_char;

    /** View as a signed char. */
    signed char as_signed_char;

    /** View as an unsigned char. */
    unsigned char as_unsigned_char;

    /** View as a signed short int. */
    signed short int as_signed_short_int;

    /** View as an unsigned short int. */
    unsigned short int as_unsigned_short_int;

    /** View as an int. */
    signed int as_signed_int;

    /** View as an unsigned int. */
    unsigned int as_unsigned_int;

    /** View as a long int. */
    signed long int as_signed_long_int;

    /** View as an unsigned long int. */
    unsigned long int as_unsigned_long_int;

    /** View as a signed long long int. */
    signed long long int as_signed_long_long_int;

    /** View as an unsigned long long int. */
    unsigned long long int as_unsigned_long_long_int;

    /** View as a float. */
    float as_float;

    /** View as a double. */
    double as_double;

    /** View as as long double. */
    long double as_long_double;

    /** View as a C-string. */
    const char* as_c_string;
  };

  /** The argument kind. */
  enum log_record_arg_kind kind;
};

/** A log record. */
struct log_record {
  /** The format string. */
  const char* fmt;

  /** The format arguments. */
  struct log_record_arg* args;

  /** The number of format arguments. */
  size_t num_args;

  /** The severity level. */
  enum log_level level;

  /** The source file name. */
  const char* file;

  /** The source line number. */
  unsigned long line;
};

/**
 * Submit a log record to a log context.
 *
 * @param ctx The log context
 * @param rec The log record
 */
void log_context_submit(struct log_context* ctx, struct log_record* rec);

/**
 * Submit a log record to the current log context.
 *
 * @param rec The log record
 */
void log_context_this_submit(struct log_record* rec);

/** @private */
#define LOG__RECORD_INIT(p_level, p_fmt, ...) {                                                       \
      .fmt = (p_fmt),                                                                                 \
      .args = (struct log_record_arg[]) { __VA_ARGS__ },                                              \
      .num_args = sizeof((struct log_record_arg[]) { __VA_ARGS__ }) / sizeof(struct log_record_arg),  \
      .level = (p_level),                                                                             \
      .file = __FILE__,                                                                               \
      .line = __LINE__,                                                                               \
    }

#define LOG(lvl, fmt, ...) log_context_this_submit(&(struct log_record) LOG__RECORD_INIT((lvl), (fmt), ##__VA_ARGS__))

/**
 * Submit a TRACE log record to the current log context.
 *
 * @param fmt The format string
 * @param ... The format arguments
 */
#define LOGT(fmt, ...) LOG(log_level_trace, (fmt), ##__VA_ARGS__)

/**
 * Submit a DEBUG log record to the current log context.
 *
 * @param fmt The format string
 * @param ... The format arguments
 */
#define LOGD(fmt, ...) LOG(log_level_trace, (fmt), ##__VA_ARGS__)

/**
 * Submit an INFO log record to the current log context.
 *
 * @param fmt The format string
 * @param ... The format arguments
 */
#define LOGI(fmt, ...) LOG(log_level_trace, (fmt), ##__VA_ARGS__)

/**
 * Submit a WARN log record to the current log context.
 *
 * @param fmt The format string
 * @param ... The format arguments
 */
#define LOGW(fmt, ...) LOG(log_level_trace, (fmt), ##__VA_ARGS__)

/**
 * Submit a ERROR log record to the current log context.
 *
 * @param fmt The format string
 * @param ... The format arguments
 */
#define LOGE(fmt, ...) LOG(log_level_trace, (fmt), ##__VA_ARGS__)

/**
 * Submit a FATAL log record to the current log context.
 *
 * @param fmt The format string
 * @param ... The format arguments
 */
#define LOGF(fmt, ...) LOG(log_level_trace, (fmt), ##__VA_ARGS__)

#endif // #ifndef LOG_H
