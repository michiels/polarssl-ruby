#include "ruby.h"
#include "entropy.h"
#include "ctr_drbg.h"
#include "ssl.h"

VALUE mPolarSSL;

void Init_polarssl()
{
  mPolarSSL = rb_define_module("PolarSSL");

  Init_entropy();
  Init_ctr_drbg();
  Init_ssl();
}
