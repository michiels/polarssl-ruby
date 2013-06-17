#include <polarssl.h>

VALUE mPolarSSL;

void Init_polarssl(void)
{
  mPolarSSL = rb_define_module("PolarSSL");

  Init_entropy_context();
}