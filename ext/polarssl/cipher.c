/*
 *  Wrapping code for the PolarSSL::Cipher class.
 *
 *  Copyright (C) 2013  Michiel Sikkes
 *
 *  This file is part of polarssl-ruby (http://github.com/michiels/polarssl-ruby)
 *
 *  All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

 #include "polarssl.h"
 #include "polarssl/cipher.h"

VALUE rb_cipher_allocate();
VALUE rb_cipher_initialize();
VALUE rb_cipher_setkey();
VALUE rb_cipher_update();
VALUE rb_cipher_finish();

struct rb_cipher
{
  cipher_context_t *ctx;
  char output[1024];
  size_t olen;
  size_t input_length;
};

typedef struct rb_cipher rb_cipher_t;

void Init_cipher()
{
  VALUE cCipher = rb_define_class_under( rb_mPolarSSL, "Cipher", rb_cObject);

  rb_define_const( cCipher, "OPERATION_ENCRYPT", INT2NUM(POLARSSL_ENCRYPT) );
  rb_define_const( cCipher, "OPERATION_DECRYPT", INT2NUM(POLARSSL_DECRYPT) );
  rb_define_const( cCipher, "OPERATION_NONE", INT2NUM(POLARSSL_OPERATION_NONE) );

  rb_define_alloc_func( cCipher, rb_cipher_allocate );
  rb_define_method( cCipher, "initialize", rb_cipher_initialize, 1 );
  rb_define_method( cCipher, "setkey", rb_cipher_setkey, 3 );
  rb_define_method( cCipher, "update", rb_cipher_update, 1 );
  rb_define_method( cCipher, "finish", rb_cipher_finish, 0 );
}

VALUE rb_cipher_allocate( VALUE klass )
{
  rb_cipher_t *rb_cipher;

  rb_cipher = ALLOC( rb_cipher_t );
  rb_cipher->olen = 0;
  rb_cipher->input_length = 0;
  rb_cipher->ctx = ALLOC( cipher_context_t );

  return Data_Wrap_Struct( klass, 0, -1, rb_cipher);
}

VALUE rb_cipher_initialize( VALUE self, VALUE cipher_type )
{
  rb_cipher_t *rb_cipher;

  Data_Get_Struct( self, rb_cipher_t, rb_cipher );

  cipher_init_ctx( rb_cipher->ctx, cipher_info_from_string( StringValueCStr( cipher_type ) ) );

  return self;
}

VALUE rb_cipher_setkey( VALUE self, VALUE key, VALUE key_length, VALUE operation )
{
  rb_cipher_t *rb_cipher;

  Data_Get_Struct( self, rb_cipher_t, rb_cipher );

  cipher_setkey( rb_cipher->ctx, StringValuePtr( key ), FIX2INT( key_length ), NUM2INT( operation ) );

  return Qtrue;
}

VALUE rb_cipher_update( VALUE self, VALUE rb_input)
{
  rb_cipher_t *rb_cipher;
  char *input;

  Data_Get_Struct( self, rb_cipher_t, rb_cipher );

  input = StringValueCStr( rb_input );
  rb_cipher->input_length = strlen(input);

  cipher_update( rb_cipher->ctx, input, strlen(input), rb_cipher->output, &rb_cipher->olen);

  return Qtrue;
}

VALUE rb_cipher_finish( VALUE self )
{
  rb_cipher_t *rb_cipher;

  Data_Get_Struct( self, rb_cipher_t, rb_cipher );

  printf("before finish\n");
  printf("olen: %zu\n", rb_cipher->olen);
  printf("strlen: %zu\n", strlen( rb_cipher->output ) );

  cipher_finish( rb_cipher->ctx, rb_cipher->output, &rb_cipher->olen );

  return rb_str_new( rb_cipher->output, rb_cipher->input_length );
}