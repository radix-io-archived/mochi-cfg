#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([mochi-cfg], [0.3], [],[],[]) 
AC_CONFIG_MACRO_DIRS([m4])
LT_INIT

AC_CANONICAL_TARGET
AC_CANONICAL_SYSTEM
AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([foreign subdir-objects -Wall])

# we should remove this soon, only needed for automake 1.10 and older
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_SRCDIR([README.md])
AC_CONFIG_HEADERS([mochi-cfg-config.h])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
AC_PROG_CXXCPP

AC_PROG_MKDIR_P

AC_REQUIRE_CPP

AC_CHECK_SIZEOF([long int])

dnl
dnl Verify pkg-config
dnl
PKG_PROG_PKG_CONFIG
if test "x$PKG_CONFIG" == "x"; then
   AC_MSG_ERROR([Could not find pkg-config utility!])
fi

PKG_CHECK_MODULES([JANSSON],[jansson],[],
   [AC_MSG_ERROR([Could not find working jansson installation!])])
LIBS="$JANSSON_LIBS $LIBS"
CPPFLAGS="$JANSSON_CFLAGS $CPPFLAGS"
CFLAGS="$JANSSON_CFLAGS $CFLAGS"

AC_CONFIG_FILES([Makefile maint/mochi-cfg.pc])
AC_OUTPUT
