#include "polarssl.h"
#include "polarssl/ssl.h"
#include "polarssl/ctr_drbg.h"

static VALUE R_ssl_set_endpoint();
static VALUE R_ssl_allocate();
static VALUE R_ssl_set_authmode();
static VALUE R_ssl_set_rng();

void Init_ssl()
{
  VALUE cSSL = rb_define_class_under(mPolarSSL, "SSL", rb_cObject);

  rb_define_const(cSSL, "SSL_IS_CLIENT", INT2NUM(SSL_IS_CLIENT));
  rb_define_const(cSSL, "SSL_VERIFY_NONE", INT2NUM(SSL_VERIFY_NONE));

  rb_define_alloc_func(cSSL, R_ssl_allocate);
  rb_define_method(cSSL, "set_endpoint", R_ssl_set_endpoint, 1);
  rb_define_method(cSSL, "set_authmode", R_ssl_set_authmode, 1);
  rb_define_method(cSSL, "set_rng", R_ssl_set_rng, 1);
}

static VALUE R_ssl_allocate(VALUE klass)
{
  ssl_context *ssl;

  return Data_Make_Struct(klass, ssl_context, 0, ssl_free, ssl);
}

static VALUE R_ssl_set_endpoint(VALUE self, VALUE endpoint_mode)
{
  Check_Type(endpoint_mode, T_FIXNUM);

  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  ssl_set_endpoint(ssl, NUM2INT(endpoint_mode));

  return self;
}

static VALUE R_ssl_set_authmode(VALUE self, VALUE authmode)
{
  Check_Type(authmode, T_FIXNUM);

  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  ssl_set_authmode(ssl, NUM2INT(authmode));

  return self;
}

static VALUE R_ssl_set_rng(VALUE self, VALUE rng)
{
  Check_Type(rng, T_DATA);

  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  ctr_drbg_context *ctr_drbg;
  Data_Get_Struct(rng, ctr_drbg_context, ctr_drbg);

  ssl_set_rng(ssl, ctr_drbg_random, ctr_drbg);

  return self;
}