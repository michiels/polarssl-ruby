#include "polarssl.h"
#include "polarssl/ssl.h"

void Init_ssl()
{
  VALUE cSSL = rb_define_class_under(mPolarSSL, "SSL", rb_cObject);

  rb_define_const(cSSL, "SSL_IS_CLIENT", INT2NUM(SSL_IS_CLIENT));
}
