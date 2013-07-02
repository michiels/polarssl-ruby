#include "polarssl.h"
#include "polarssl/ssl.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/net.h"
#include "ruby/io.h"

static VALUE R_ssl_allocate();
static VALUE R_ssl_initialize();
static VALUE R_ssl_set_endpoint();
static VALUE R_ssl_set_authmode();
static VALUE R_ssl_set_rng();
static VALUE R_ssl_set_bio();
static VALUE R_ssl_handshake();


void Init_ssl()
{
  VALUE cSSL = rb_define_class_under(mPolarSSL, "SSL", rb_cObject);

  rb_define_const(cSSL, "SSL_IS_CLIENT", INT2NUM(SSL_IS_CLIENT));
  rb_define_const(cSSL, "SSL_VERIFY_NONE", INT2NUM(SSL_VERIFY_NONE));

  rb_define_alloc_func(cSSL, R_ssl_allocate);
  rb_define_method(cSSL, "initialize", R_ssl_initialize, 0);
  rb_define_method(cSSL, "set_endpoint", R_ssl_set_endpoint, 1);
  rb_define_method(cSSL, "set_authmode", R_ssl_set_authmode, 1);
  rb_define_method(cSSL, "set_rng", R_ssl_set_rng, 1);
  rb_define_method(cSSL, "set_bio", R_ssl_set_bio, 4);
  rb_define_method(cSSL, "handshake", R_ssl_handshake, 0);
}

static VALUE R_ssl_allocate(VALUE klass)
{
  ssl_context *ssl;

  return Data_Make_Struct(klass, ssl_context, 0, NULL, ssl);
}

static VALUE R_ssl_initialize(VALUE self)
{
  ssl_context *ssl;

  Data_Get_Struct(self, ssl_context, ssl);
  ssl_init(ssl);

  return self;
}

static VALUE R_ssl_set_endpoint(VALUE self, VALUE endpoint_mode)
{
  Check_Type(endpoint_mode, T_FIXNUM);

  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  // ssl_set_endpoint(ssl, NUM2INT(endpoint_mode));
  ssl_set_endpoint(ssl, SSL_IS_CLIENT);

  return self;
}

static VALUE R_ssl_set_authmode(VALUE self, VALUE authmode)
{
  Check_Type(authmode, T_FIXNUM);

  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  // ssl_set_authmode(ssl, NUM2INT(authmode));
  ssl_set_authmode(ssl, SSL_VERIFY_NONE);

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

static VALUE R_ssl_set_bio(VALUE self, VALUE recv_func, VALUE input_socket, VALUE send_func, VALUE output_socket)
{
  Check_Type(input_socket, T_FILE);
  Check_Type(output_socket, T_FILE);

  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  rb_io_t *fptr;
  GetOpenFile(input_socket, fptr);

  ssl_set_bio(ssl, net_recv, &fptr->fd, net_send, &fptr->fd);

  return self;
}

static VALUE R_ssl_handshake(VALUE self)
{
  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  ssl_handshake(ssl);

  return self;
}