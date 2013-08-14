/*
 *  Wrapping code for the PolarSSL::Entropy class.
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
#include "polarssl/entropy.h"
#include "ruby.h"

static VALUE R_entropy_allocate();
static VALUE R_entropy_initialize();
static VALUE R_entropy_gather();

void Init_entropy()
{
  VALUE cEntropy = rb_define_class_under( mPolarSSL, "Entropy", rb_cObject );

  rb_define_alloc_func( cEntropy, R_entropy_allocate );
  rb_define_method( cEntropy, "initialize", R_entropy_initialize, 0 );
  rb_define_method( cEntropy, "gather", R_entropy_gather, 0 );
}

static VALUE R_entropy_allocate( VALUE klass )
{
  entropy_context *entropy;

  return Data_Make_Struct( klass, entropy_context, 0, -1, entropy );
}

static VALUE R_entropy_initialize( VALUE self )
{
  entropy_context *entropy;

  Data_Get_Struct( self, entropy_context, entropy );
  entropy_init( entropy );

  return self;
}

static VALUE R_entropy_gather(VALUE self)
{
  entropy_context *entropy;
  VALUE ret;

  Data_Get_Struct( self, entropy_context, entropy );

  if( entropy_gather( entropy ) == 0 ) {
    ret = Qtrue;
  } else {
    ret = Qfalse;
  }

  return ret;
}