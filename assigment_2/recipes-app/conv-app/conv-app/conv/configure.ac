#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([src/conv.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
CFLAGS=""
AC_PROG_CC
AC_PROG_CPP

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h math.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T

# Checks for library functions.
##AC_CHECK_PROGS([DOXYGEN], [doxygen])
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([conv])
AC_SEARCH_LIBS([pow], [m])

AC_CONFIG_FILES([Makefile src/Makefile])
##AC_CONFIG_FILES([Makefile src/Makefile  docs/Makefile])
AC_OUTPUT
