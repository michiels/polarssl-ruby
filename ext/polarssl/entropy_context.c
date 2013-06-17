#include <entropy_context.h>

void Init_entropy_context(void)
{
  VALUE cEntropyContext = rb_define_class_under(mPolarSSL, "EntropyContext", rb_cObject);
  rb_define_alloc_func(cEntropyContext, entropy_context_allocate);
  rb_define_method(cEntropyContext, "initialize", entropy_context_initialize, 0);
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