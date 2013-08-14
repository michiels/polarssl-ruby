/*
 *  Wrapping code for the PolarSSL::CtrDrbg class.
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
#include "polarssl/ctr_drbg.h"
#include "polarssl/entropy.h"

static VALUE e_EntropySourceFailed;

static VALUE R_ctr_drbg_allocate();
static VALUE R_ctr_drbg_initialize();
static VALUE R_ctr_drbg_self_test();

void Init_ctr_drbg()
{
  VALUE cCtrDrbg = rb_define_class_under( mPolarSSL, "CtrDrbg", rb_cObject );

  rb_define_singleton_method( cCtrDrbg, "self_test", R_ctr_drbg_self_test, 0 );

  rb_define_alloc_func( cCtrDrbg, R_ctr_drbg_allocate );
  rb_define_method( cCtrDrbg, "initialize", R_ctr_drbg_initialize, 1 );
}

static VALUE R_ctr_drbg_allocate( VALUE klass )
{
  ctr_drbg_context *ctr_drbg;

  return Data_Make_Struct( klass, ctr_drbg_context, 0, -1, ctr_drbg );
}

static VALUE R_ctr_drbg_initialize( VALUE self, VALUE entropy )
{
  entropy_context *entropy_p;
  ctr_drbg_context *ctr_drbg;
  int ret;

  Data_Get_Struct( self, ctr_drbg_context, ctr_drbg );
  Data_Get_Struct( entropy, entropy_context, entropy_p );

  ret = ctr_drbg_init( ctr_drbg, entropy_func, entropy_p, NULL, 0 );

  if( ret == POLARSSL_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED )
    rb_raise( e_EntropySourceFailed, "Could not initialize entropy source" );

  return self;
}

static VALUE R_ctr_drbg_self_test( VALUE klass )
{
  VALUE ret;

  if ( ctr_drbg_self_test( 1 ) == 0 )
  {
    ret = Qtrue;
  } else {
    ret = Qfalse;
  }

  return ret;
}