#include "polarssl.h"
#include "polarssl/entropy.h"
#include "ruby.h"

static VALUE entropy_context_allocate();
static VALUE entropy_context_initialize();
static VALUE entropy_context_gather();

void Init_entropy_context()
{
  VALUE cEntropyContext = rb_define_class_under(mPolarSSL, "EntropyContext", rb_cObject);

  rb_define_alloc_func(cEntropyContext, entropy_context_allocate);
  rb_define_method(cEntropyContext, "initialize", entropy_context_initialize, 0);
  rb_define_method(cEntropyContext, "gather", entropy_context_gather, 0);
}

static VALUE entropy_context_allocate(VALUE klass)
{
  entropy_context *entropy;

  return Data_Make_Struct(klass, entropy_context, 0, free, entropy);
}

static VALUE entropy_context_initialize(VALUE self)
{
  entropy_context *entropy;

  Data_Get_Struct(self, entropy_context, entropy);
  entropy_init(entropy);

  return self;
}

static VALUE entropy_context_gather(VALUE self)
{
  entropy_context *entropy;
  Data_Get_Struct(self, entropy_context, entropy);

  entropy_gather(entropy);

  return self;
}