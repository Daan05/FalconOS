/*#include "stdio.h"*/
/**/
/*#include <stdarg.h>*/
/*#include <stdbool.h>*/
/**/
/*static void print_string(const char *str) {*/
/*  while (*str) {*/
/*    tty_putchar(*str++);*/
/*  }*/
/*}*/
/**/
/*static void print_int(int value) {*/
/*  if (value < 0) {*/
/*    tty_putchar('-');*/
/*    value = -value;*/
/*  }*/
/**/
/*  char buffer[12]; // Enough for -2,147,483,648 and null terminator*/
/*  int i = 0;*/
/*  do {*/
/*    buffer[i++] = '0' + (value % 10);*/
/*    value /= 10;*/
/*  } while (value > 0);*/
/*  buffer[i] = '\0';*/
/**/
/*  // Reverse the buffer*/
/*  for (int j = i - 1; j >= 0; j--) {*/
/*    tty_putchar(buffer[j]);*/
/*  }*/
/*}*/
/**/
/*static void print_uint(unsigned int value) {*/
/*  char buffer[11]; // Enough for 4,294,967,295 and null terminator*/
/*  int i = 0;*/
/*  do {*/
/*    buffer[i++] = '0' + (value % 10);*/
/*    value /= 10;*/
/*  } while (value > 0);*/
/*  buffer[i] = '\0';*/
/**/
/*  // Reverse the buffer*/
/*  for (int j = i - 1; j >= 0; j--) {*/
/*    tty_putchar(buffer[j]);*/
/*  }*/
/*}*/
/**/
/*static void print_hex(unsigned int value, bool uppercase) {*/
/*  char buffer[9]; // Enough for 0xFFFFFFFF and null terminator*/
/*  int i = 0;*/
/*  do {*/
/*    int digit = value % 16;*/
/*    if (digit < 10) {*/
/*      buffer[i++] = '0' + digit;*/
/*    } else {*/
/*      buffer[i++] = (uppercase ? 'A' : 'a') + (digit - 10);*/
/*    }*/
/*    value /= 16;*/
/*  } while (value > 0);*/
/*  buffer[i] = '\0';*/
/**/
/*  // Reverse the buffer*/
/*  for (int j = i - 1; j >= 0; j--) {*/
/*    tty_putchar(buffer[j]);*/
/*  }*/
/*}*/
/**/
/*int printf(const char *format, ...) {*/
/*  va_list args;*/
/*  va_start(args, format);*/
/**/
/*  while (*format) {*/
/*    if (*format == '%') {*/
/*      format++;*/
/*      switch (*format) {*/
/*      case 'c': {*/
/*        char c = (char)va_arg(args, int);*/
/*        tty_putchar(c);*/
/*        break;*/
/*      }*/
/*      case 's': {*/
/*        const char *str = va_arg(args, const char *);*/
/*        print_string(str ? str : "(null)");*/
/*        break;*/
/*      }*/
/*      case 'd': {*/
/*        int value = va_arg(args, int);*/
/*        print_int(value);*/
/*        break;*/
/*      }*/
/*      case 'u': {*/
/*        unsigned int value = va_arg(args, unsigned int);*/
/*        print_uint(value);*/
/*        break;*/
/*      }*/
/*      case 'x': {*/
/*        unsigned int value = va_arg(args, unsigned int);*/
/*        print_hex(value, false);*/
/*        break;*/
/*      }*/
/*      case 'X': {*/
/*        unsigned int value = va_arg(args, unsigned int);*/
/*        print_hex(value, true);*/
/*        break;*/
/*      }*/
/*      case '%': {*/
/*        tty_putchar('%');*/
/*        break;*/
/*      }*/
/*      default:*/
/*        tty_putchar('%');*/
/*        tty_putchar(*format);*/
/*      }*/
/*    } else {*/
/*      tty_putchar(*format);*/
/*    }*/
/*    format++;*/
/*  }*/
/**/
/*  va_end(args);*/
/*  return 0;*/
/*}*/
