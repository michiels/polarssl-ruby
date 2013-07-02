#include "polarssl.h"
#include "polarssl/ssl.h"

static VALUE R_ssl_set_endpoint();

void Init_ssl()
{
  VALUE cSSL = rb_define_class_under(mPolarSSL, "SSL", rb_cObject);

  rb_define_method(cSSL, "set_endpoint", R_ssl_set_endpoint, 1);

  rb_define_const(cSSL, "SSL_IS_CLIENT", INT2NUM(SSL_IS_CLIENT));
}

static VALUE R_ssl_set_endpoint(VALUE self, VALUE endpoint_mode)
{
  Check_Type(endpoint_mode, T_FIXNUM);

  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  ssl_set_endpoint(ssl, NUM2INT(endpoint_mode));

  return self;
}