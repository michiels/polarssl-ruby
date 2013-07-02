#include "polarssl.h"
#include "polarssl/ctr_drbg.h"

static VALUE ctr_drbg_context_allocate();

void Init_ctr_drbg_context()
{
  VALUE cCtrDrbg = rb_define_class_under(mPolarSSL, "CtrDrbgContext", rb_cObject);

  rb_define_alloc_func(cCtrDrbg, ctr_drbg_context_allocate);
}

static VALUE ctr_drbg_context_allocate(VALUE klass)
{
  ctr_drbg_context *ctr_drbg;

  return Data_Make_Struct(klass, ctr_drbg_context, 0, free, ctr_drbg);
}
