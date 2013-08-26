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
#include "ruby.h"

VALUE rb_cipher_allocate();
VALUE rb_cipher_initialize();
VALUE rb_cipher_setkey();
VALUE rb_cipher_update();
VALUE rb_cipher_finish();
void  rb_cipher_free();

VALUE e_UnsupportedCipher;
VALUE e_CipherError;

typedef struct
{
  cipher_context_t *ctx;
  unsigned char *output;
  size_t olen;
  size_t input_length;
} rb_cipher_t;

void Init_cipher(void)
{
    /** Document-class: PolarSSL::Cipher
      *
      * This class lets you encrypt and decrypt data.
      *
      * == Example
      *
      *   require 'polarssl'
      *   require 'base64'
      *
      *   cipher = PolarSSL::Cipher.new("AES-128-CTR")
      *   cipher.setkey("mykey", 128, PolarSSL::Cipher::OPERATION_ENCRYPT)
      *   cipher.update("secret stuff I want encrypted")
      *   encrypted_data = cipher.finish()
      *
      *   encoded_encrypted_data = Base64.encode64(encrypted_data)
      *   puts encoded_encrypted_data
      *
      * == When you get an exception
      *
      * When using the Cipher class, you might get an exception. Some
      * exeptions return a PolarSSL error code, like PolarSSL::Cipher::Error.
      *
      * These error codes are directly passed on from the PolarSSL library
      * and you can look up what they mean in the PolarSSL API documentation
      * at: https://polarssl.org/api/.
      *
      * == Supported Cipher types:
      *
      *   CAMELLIA-128-CBC
      *   CAMELLIA-192-CBC
      *   CAMELLIA-256-CBC
      *
      *   CAMELLIA-128-CFB128
      *   CAMELLIA-192-CFB128
      *   CAMELLIA-256-CFB128
      *
      *   CAMELLIA-128-CTR
      *   CAMELLIA-192-CTR
      *   CAMELLIA-256-CTR
      *
      *   AES-128-CBC
      *   AES-192-CBC
      *   AES-256-CBC
      *
      *   AES-128-CFB128
      *   AES-192-CFB128
      *   AES-256-CFB128
      *
      *   AES-128-CTR
      *   AES-192-CTR
      *   AES-256-CTR
      *
      *   DES-CBC
      *   DES-EDE-CBC
      *   DES-EDE3-CBC
      *
      *   BLOWFISH-CBC
      *   BLOWFISH-CFB64
      *   BLOWFISH-CTR
      *
      *   NULL
      *
      */
    VALUE cCipher = rb_define_class_under( rb_mPolarSSL, "Cipher", rb_path2class("Object") );

    /* 1: Use cipher for encryption */
    rb_define_const( cCipher, "OPERATION_ENCRYPT", INT2NUM(POLARSSL_ENCRYPT) );

    /* 0: Use cipher for decryption */
    rb_define_const( cCipher, "OPERATION_DECRYPT", INT2NUM(POLARSSL_DECRYPT) );

    /* -1: Don't use cipher for anything */
    rb_define_const( cCipher, "OPERATION_NONE", INT2NUM(POLARSSL_OPERATION_NONE) );

    /* Document-class: PolarSSL::Cipher::UnsupportedCipher
     * Raised when you do not pass a supported cipher type to PolarSSL::Cipher.new()
     */
    e_UnsupportedCipher = rb_define_class_under( cCipher, "UnsupportedCipher", rb_eStandardError );

    /* Document-class: PolarSSL::Cipher::Error
     * Raised when the PolarSSL library throws a certain Cipher error code
     */
    e_CipherError = rb_define_class_under( cCipher, "Error", rb_eStandardError) ;

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
  memset( rb_cipher, 0, sizeof( rb_cipher_t) );

  rb_cipher->olen = 0;
  rb_cipher->input_length = 0;

  rb_cipher->ctx = ALLOC( cipher_context_t );
  memset( rb_cipher->ctx, 0, sizeof( cipher_context_t ) );

  return Data_Wrap_Struct( klass, 0, rb_cipher_free, rb_cipher );
}

/*
 *  call-seq: new(cipher_type)
 *
 *  Initializes a new Cipher object to encrypt data with. For supported cipher types,
 *  see: https://github.com/michiels/polarssl-ruby/wiki/Using-PolarSSL::Cipher
 *
 */
VALUE rb_cipher_initialize( VALUE self, VALUE cipher_type )
{
  rb_cipher_t *rb_cipher;
  char *cipher_type_str;
  const cipher_info_t *cipher_info;
  int ret;

  cipher_type_str = StringValueCStr( cipher_type );

  Data_Get_Struct( self, rb_cipher_t, rb_cipher );

  cipher_info = cipher_info_from_string( cipher_type_str );

  if (cipher_info == NULL)
  {
    rb_raise(e_UnsupportedCipher, "%s is not a supported cipher", cipher_type_str );
  }
  else {
    ret = cipher_init_ctx( rb_cipher->ctx, cipher_info );
    if ( ret < 0 )
      rb_raise( e_CipherError, "PolarSSL error: -0x%x", -ret );
  }

  return self;
}


/*
 *  call-seq: setkey(key, key_length, operation)
 *
 *  Sets the key to be used for encrypting/decrypting this cipher. The key, key_length and operation
 *  depend on which cipher you are using. For example, when using AES-128-CTR you would use something like:
 *
 *    cipher = PolarSSL::Cipher.new('AES-128-CTR')
 *    cipher.setkey('mykey', 128, PolarSSL::Cipher::OPERATION_ENCRYPT)
 *
 *  for both encryping and decrypting your cipher.
 *
 */
VALUE rb_cipher_setkey( VALUE self, VALUE key, VALUE key_length, VALUE operation )
{
  rb_cipher_t *rb_cipher;
  int ret;

  Data_Get_Struct( self, rb_cipher_t, rb_cipher );

  ret = cipher_setkey( rb_cipher->ctx, (const unsigned char *) StringValueCStr( key ), FIX2INT( key_length ), NUM2INT( operation ) );

  if (ret < 0)
    rb_raise( e_CipherError, "PolarSSL error: -0x%x", -ret);

  return Qtrue;
}

/*
 *  call-seq: update(input)
 *
 *  Adds input to your cipher.
 *
 */
VALUE rb_cipher_update( VALUE self, VALUE rb_input)
{
  rb_cipher_t *rb_cipher;
  char *input;
  int ret;

  Data_Get_Struct( self, rb_cipher_t, rb_cipher );

  input = StringValueCStr( rb_input );

  rb_cipher->input_length += strlen(input);

  /* Increases the output buffer so it results into the total input length so far. */
  REALLOC_N(rb_cipher->output, unsigned char, rb_cipher->input_length);

  ret = cipher_update( rb_cipher->ctx, (const unsigned char *) input, strlen(input), rb_cipher->output, &rb_cipher->olen);

  if (ret < 0)
    rb_raise( e_CipherError, "PolarSSL error: -0x%x", -ret);

  return Qtrue;
}

/*
 *  call-seq: finish()
 *
 *  Finishes encrypting the data added by one or multiple update() calls and returns the encrypted data.
 *
 */
VALUE rb_cipher_finish( VALUE self )
{
  rb_cipher_t *rb_cipher;
  int ret;

  Data_Get_Struct( self, rb_cipher_t, rb_cipher );

  ret = cipher_finish( rb_cipher->ctx, rb_cipher->output, &rb_cipher->olen );

  if (ret < 0)
    rb_raise( e_CipherError, "PolarSSL error: -0x%x", -ret);

  return rb_str_new( (const char *) rb_cipher->output, rb_cipher->input_length );
}

void rb_cipher_free( rb_cipher_t *rb_cipher )
{

  if (rb_cipher->ctx)
    cipher_free_ctx(rb_cipher->ctx);

  xfree(rb_cipher);
}