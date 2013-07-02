#include "ruby.h"
#include "entropy.h"
#include "ctr_drbg.h"

VALUE mPolarSSL;

void Init_polarssl()
{
  mPolarSSL = rb_define_module("PolarSSL");

  Init_entropy();
  Init_ctr_drbg();
}
