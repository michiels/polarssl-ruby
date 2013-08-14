#include "polarssl.h"
#include "polarssl/entropy.h"
#include "ruby.h"

static VALUE R_entropy_allocate();
static VALUE R_entropy_initialize();
static VALUE R_entropy_gather();

void Init_entropy()
{
  VALUE cEntropy = rb_define_class_under(mPolarSSL, "Entropy", rb_cObject);

  rb_define_alloc_func(cEntropy, R_entropy_allocate);
  rb_define_method(cEntropy, "initialize", R_entropy_initialize, 0);
  rb_define_method(cEntropy, "gather", R_entropy_gather, 0);
}

static VALUE R_entropy_allocate(VALUE klass)
{
  entropy_context *entropy;

  return Data_Make_Struct(klass, entropy_context, 0, -1, entropy);
}

static VALUE R_entropy_initialize(VALUE self)
{
  entropy_context *entropy;

  Data_Get_Struct(self, entropy_context, entropy);
  entropy_init(entropy);

  return self;
}

static VALUE R_entropy_gather(VALUE self)
{
  entropy_context *entropy;
  Data_Get_Struct(self, entropy_context, entropy);

  VALUE ret;

  if(entropy_gather(entropy) == 0) {
    ret = Qtrue;
  } else {
    ret = Qfalse;
  }

  return ret;
}