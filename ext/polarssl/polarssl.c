/*
 *  Wrapping code and entrypoint for the PolarSSL module.
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

#include "ruby.h"
#include "entropy.h"
#include "ctr_drbg.h"
#include "ssl.h"
#include "cipher.h"

void Init_polarssl()
{
  /* The PolarSSL module */
  VALUE rb_mPolarSSL = rb_define_module( "PolarSSL" );

  Init_entropy( );
  Init_ctr_drbg( );
  Init_ssl( );
  Init_cipher( );
}
