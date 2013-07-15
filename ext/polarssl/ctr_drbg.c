#include "polarssl.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/entropy.h"

static VALUE e_EntropySourceFailed;

static VALUE R_ctr_drbg_allocate();
static VALUE R_ctr_drbg_initialize();
static VALUE R_ctr_drbg_self_test();

void Init_ctr_drbg()
{
  VALUE cCtrDrbg = rb_define_class_under(mPolarSSL, "CtrDrbg", rb_cObject);
  VALUE e_EntropySourceFailed = rb_define_class_under(mPolarSSL, "EntropySourceFailed", rb_eStandardError);

  rb_define_singleton_method(cCtrDrbg, "self_test", R_ctr_drbg_self_test, 0);

  rb_define_alloc_func(cCtrDrbg, R_ctr_drbg_allocate);
  rb_define_method(cCtrDrbg, "initialize", R_ctr_drbg_initialize, 1);
}

static VALUE R_ctr_drbg_allocate(VALUE klass)
{
  ctr_drbg_context *ctr_drbg;

  return Data_Make_Struct(klass, ctr_drbg_context, 0, free, ctr_drbg);
}

static VALUE R_ctr_drbg_initialize(VALUE self, VALUE entropy)
{
  entropy_context *entropy_p;
  ctr_drbg_context *ctr_drbg;

  Data_Get_Struct(self, ctr_drbg_context, ctr_drbg);
  Data_Get_Struct(entropy, entropy_context, entropy_p);

  int ret = ctr_drbg_init(ctr_drbg, entropy_func, entropy_p, NULL, 0);

  if (ret == POLARSSL_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED)
  {
    rb_raise(e_EntropySourceFailed, "Could not initialize entropy source");
  }

  return self;
}

static VALUE R_ctr_drbg_self_test(VALUE klass)
{
  VALUE ret;

  if (ctr_drbg_self_test(1) == 0)
  {
    ret = Qtrue;
  } else {
    ret = Qfalse;
  }

  return ret;
}