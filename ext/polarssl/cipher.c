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

void Init_cipher()
{
  VALUE cCipher = rb_define_class_under( rb_mPolarSSL, "Cipher", rb_cObject);

  rb_define_const( cCipher, "OPERATION_ENCRYPT", INT2NUM(POLARSSL_ENCRYPT) );
  rb_define_const( cCipher, "OPERATION_DECRYPT", INT2NUM(POLARSSL_DECRYPT) );
  rb_define_const( cCipher, "OPERATION_NONE", INT2NUM(POLARSSL_OPERATION_NONE) );

  rb_define_alloc_func( cCipher, rb_cipher_allocate );
  rb_define_method( cCipher, "initialize", rb_cipher_initialize, 1 );
  rb_define_method( cCipher, "setkey", rb_cipher_setkey, 3 );
}

VALUE rb_cipher_allocate( VALUE klass )
{
  cipher_context_t *ctx;

  ctx = ALLOC( cipher_context_t );
  cipher_init_ctx(ctx, cipher_info_from_type(POLARSSL_CIPHER_NULL));

  return Data_Wrap_Struct( klass, 0, cipher_free_ctx, ctx);
}

VALUE rb_cipher_initialize( VALUE self, VALUE cipher_type )
{
  cipher_context_t *ctx;

  Data_Get_Struct( self, cipher_context_t, ctx );

  cipher_init_ctx(ctx, cipher_info_from_string(StringValueCStr(cipher_type)));

  return self;
}

VALUE rb_cipher_setkey( VALUE self, VALUE key, VALUE key_length, VALUE operation )
{
  cipher_context_t *ctx;

  Data_Get_Struct( self, cipher_context_t, ctx );

  cipher_setkey( ctx, (unsigned char *) StringValueCStr(key), FIX2INT(key_length), NUM2INT(operation) );

  return Qtrue;
}