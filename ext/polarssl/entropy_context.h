#include <polarssl.h>
#include <polarssl/entropy.h>

static VALUE entropy_context_allocate();
static VALUE entropy_context_initialize();