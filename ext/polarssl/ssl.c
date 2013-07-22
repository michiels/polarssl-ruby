#include "polarssl.h"
#include "polarssl/ssl.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/net.h"
#include "ruby/io.h"

VALUE e_MallocFailed;
VALUE e_NetWantRead;
VALUE e_NetWantWrite;
VALUE e_SSLError;

static VALUE R_ssl_allocate();
static VALUE R_ssl_initialize();
static VALUE R_ssl_set_endpoint();
static VALUE R_ssl_set_authmode();
static VALUE R_ssl_set_rng();
static VALUE R_ssl_set_bio();
static VALUE R_ssl_handshake();
static VALUE R_ssl_write();
static VALUE R_ssl_read();
static VALUE R_ssl_close_notify();
static VALUE R_close();

void my_debug(void *ctx, int level, const char *str)
{
  fprintf((FILE *)ctx, "%s", str);
}

extern void Init_ssl()
{
  e_MallocFailed = rb_define_class_under(mPolarSSL, "MallocFailed", rb_eStandardError);
  e_NetWantRead = rb_define_class_under(mPolarSSL, "NetWantRead", rb_eStandardError);

  VALUE cSSL = rb_define_class_under(mPolarSSL, "SSL", rb_cObject);

  e_SSLError = rb_define_class_under(cSSL, "Error", rb_eRuntimeError);

  rb_define_const(cSSL, "SSL_IS_CLIENT", INT2NUM(SSL_IS_CLIENT));
  rb_define_const(cSSL, "SSL_VERIFY_NONE", INT2NUM(SSL_VERIFY_NONE));
  rb_define_const(cSSL, "SSL_VERIFY_REQUIRED", INT2NUM(SSL_VERIFY_REQUIRED));

  rb_define_alloc_func(cSSL, R_ssl_allocate);
  rb_define_method(cSSL, "initialize", R_ssl_initialize, 0);
  rb_define_method(cSSL, "set_endpoint", R_ssl_set_endpoint, 1);
  rb_define_method(cSSL, "set_authmode", R_ssl_set_authmode, 1);
  rb_define_method(cSSL, "set_rng", R_ssl_set_rng, 1);
  rb_define_method(cSSL, "set_bio", R_ssl_set_bio, 4);
  rb_define_method(cSSL, "handshake", R_ssl_handshake, 0);
  rb_define_method(cSSL, "write", R_ssl_write, 1);
  rb_define_method(cSSL, "read", R_ssl_read, 1);
  rb_define_method(cSSL, "close_notify", R_ssl_close_notify, 0);
  rb_define_method(cSSL, "close", R_close, 0);
}

static VALUE R_ssl_allocate(VALUE klass)
{
  ssl_context *ssl;

  return Data_Make_Struct(klass, ssl_context, 0, ssl_free, ssl);
}

static VALUE R_ssl_initialize(VALUE self)
{
  ssl_context *ssl;

  Data_Get_Struct(self, ssl_context, ssl);

  int ret = ssl_init(ssl);

  if (ret == POLARSSL_ERR_SSL_MALLOC_FAILED)
  {
    rb_raise(e_MallocFailed, "ssl_init() memory allocation failed.");
  }

  // ssl_set_dbg(ssl, my_debug, stdout);

  return self;
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

  VALUE exception;

  int ret = ssl_handshake(ssl);

  if (ret < 0) {
    if (ret == POLARSSL_ERR_NET_WANT_READ)
    {
      rb_raise(e_NetWantRead, "ssl_handshake() returned POLARSSL_ERR_NET_WANT_READ");
    }
    else if (ret == POLARSSL_ERR_NET_WANT_WRITE)
    {
      rb_raise(e_NetWantWrite, "ssl_handshake() returned POLARSSL_ERR_NET_WANT_WRITE");
    }
    else
    {
      rb_raise(e_SSLError, "-0x%x", -ret);
    }
  } else {
    return Qtrue;
  }
}

static VALUE R_ssl_write(VALUE self, VALUE string)
{
  Check_Type(string, T_STRING);

  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  char *buffer;
  buffer = RSTRING_PTR(string);

  int ret = ssl_write(ssl, (const unsigned char *) buffer, RSTRING_LEN(string));

  if (ret < 0)
  {
    rb_raise(e_SSLError, "-0x%x", -ret);
  }

  return Qtrue;
}

static VALUE R_ssl_read(VALUE self, VALUE length)
{
  Check_Type(length, T_FIXNUM);

  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  VALUE result;

  int buffer_size = NUM2INT(length);
  unsigned char buffer[buffer_size];

  int length_to_read = sizeof(buffer) - 1;
  int length_read = ssl_read(ssl, buffer, length_to_read);

  if (length_read == 0 || length_read == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY) {
    result = Qnil;
  } else if (length_read < 0) {
    rb_raise(e_SSLError, "-0x%x", -length_read);
  } else {
    result = rb_str_new2(buffer);
  }

  return result;
}

static VALUE R_ssl_close_notify(VALUE self)
{
  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  int ret = ssl_close_notify(ssl);

  if (ret < 0)
  {
    rb_raise(e_SSLError, "-0x%x", -ret);
  }

  return Qtrue;
}

static VALUE R_close(VALUE self)
{
  ssl_context *ssl;
  Data_Get_Struct(self, ssl_context, ssl);

  memset(ssl, 0, sizeof(*ssl));

  return Qtrue;
}
