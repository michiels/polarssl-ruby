require 'mkmf'

$CFLAGS = "-g -Wall -Wdeclaration-after-statement " + $CFLAGS

LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']

HEADER_DIRS = [
  # First search /opt/local for macports
  '/opt/local/include',

  # Then search /usr/local for people that installed from source
  '/usr/local/include',

  # Check the ruby install locations
  INCLUDEDIR,

  # Finally fall back to /usr
  '/usr/include',
]

LIB_DIRS = [
  # First search /opt/local for macports
  '/opt/local/lib',

  # Then search /usr/local for people that installed from source
  '/usr/local/lib',

  # Check the ruby install locations
  LIBDIR,

  # Finally fall back to /usr
  '/usr/lib',
]

dir_config('polarssl', HEADER_DIRS, LIB_DIRS)

unless find_header('polarssl/entropy.h')
  abort "libpolarssl or libmbedtls is missing. please install libpolarssl"
end

unless find_library('mbedtls', 'entropy_init') || find_library('polarssl', 'entropy_init')
  abort "libpolarssl or libmbedtls is missing.  please install libpolarssl or libmbedtls"
end

create_makefile('polarssl/polarssl')
