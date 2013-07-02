#include "ruby.h"
#include "entropy_context.h"
#include "ctr_drbg_context.h"

VALUE mPolarSSL;

void Init_polarssl()
{
  mPolarSSL = rb_define_module("PolarSSL");

  Init_entropy_context();
  Init_ctr_drbg_context();
}
