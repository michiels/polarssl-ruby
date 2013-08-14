require 'mkmf'

$CFLAGS = "-g -Wall -Wdeclaration-after-statement" + $CFLAGS

unless find_header('polarssl/entropy.h')
  abort "libpolarssl is missing. please install libpolarssl"
end

unless find_library('polarssl', 'entropy_init')
  abort "libpolarssl is missing.  please install libpolarssl"
end

create_makefile('polarssl/polarssl')