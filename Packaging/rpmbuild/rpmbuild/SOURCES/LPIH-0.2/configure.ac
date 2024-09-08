#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([LPIH], [0.2], [Limax_ImperatoriaVI@outlook.com])
AC_CONFIG_SRCDIR([lpih-main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
# FIXME: Replace 'main' with a function in '-lGL':
AC_CHECK_LIB([GL], [main])

# Checks for header files.
AC_PATH_X

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([strstr])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
