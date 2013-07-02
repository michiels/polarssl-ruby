#include "polarssl.h"
#include "polarssl/ctr_drbg.h"

static VALUE R_ctr_drbg_allocate();
static VALUE R_ctr_drbg_initialize();

void Init_ctr_drbg()
{
  VALUE cCtrDrbg = rb_define_class_under(mPolarSSL, "CtrDrbg", rb_cObject);

  rb_define_alloc_func(cCtrDrbg, R_ctr_drbg_allocate);
  rb_define_method(cCtrDrbg, "initialize", R_ctr_drbg_initialize, -1);
}

static VALUE R_ctr_drbg_allocate(VALUE klass)
{
  ctr_drbg_context *ctr_drbg;

  return Data_Make_Struct(klass, ctr_drbg_context, 0, free, ctr_drbg);
}

static VALUE R_ctr_drbg_initialize(int argc, VALUE *argv, VALUE klass)
{
  VALUE entropy;
}