#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([funcdraw snumopt], [1.0], [panderan@163.com])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([mission1/expression_eval.c], 
                 [mission2/bignum_operation.c],
                 [mission3/main.c])
AC_CONFIG_HEADERS([config.h])
PKG_PROG_PKG_CONFIG([0.29])
# Checks for programs.
AC_PROG_CC

# Checks for libraries.
 PKG_CHECK_MODULES([GTK], [gtk+-3.0])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

# Checks for library functions.
AC_CHECK_FUNCS([memset pow])
AC_CONFIG_FILES([Makefile mission1/Makefile 
                          mission2/Makefile 
                          mission3/Makefile])
AC_OUTPUT
