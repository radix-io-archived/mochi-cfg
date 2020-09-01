AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS =
bin_SCRIPTS =
noinst_PROGRAMS =
noinst_HEADERS =
TESTS =
XFAIL_TESTS =
check_PROGRAMS =
EXTRA_PROGRAMS =
CLEANFILES = $(bin_SCRIPTS)
MAINTAINERCLEANFILES =
EXTRA_DIST =
BUILT_SOURCES =
include_HEADERS = include/mochi-cfg.h

EXTRA_DIST += \
 prepare.sh

AM_CPPFLAGS = -I$(top_srcdir)/include

AM_CFLAGS =

AM_LIBS =

AM_CXXFLAGS = $(AM_CFLAGS)

lib_LTLIBRARIES = src/libmochi-cfg.la
src_libmochi_cfg_la_SOURCES =

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = maint/mochi-cfg.pc

include Make.rules

include $(top_srcdir)/src/Makefile.subdir
