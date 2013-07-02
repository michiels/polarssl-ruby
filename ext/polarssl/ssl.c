#include "polarssl.h"

void Init_ssl()
{
  VALUE cSSL = rb_define_class_under(mPolarSSL, "SSL", rb_cObject);
}
