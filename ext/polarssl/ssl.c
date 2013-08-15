/*
 *  Wrapping code for the PolarSSL::SSL class.
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
#include "polarssl/ssl.h"
#include "polarssl/ctr_drbg.h"
#include "polarssl/net.h"
#include "polarssl/version.h"
#include "ruby/io.h"

VALUE e_MallocFailed;
VALUE e_NetWantRead;
VALUE e_NetWantWrite;
VALUE e_SSLError;

static VALUE R_ssl_allocate();
static VALUE R_ssl_set_endpoint();
static VALUE R_ssl_set_authmode();
static VALUE R_ssl_set_rng();
static VALUE R_ssl_set_socket();
static VALUE R_ssl_handshake();
static VALUE R_ssl_write();
static VALUE R_ssl_read();
static VALUE R_ssl_close_notify();
static VALUE R_close();

void my_debug( void *ctx, int level, const char *str )
{
    fprintf( (FILE *)ctx, "%s", str );
}

void Init_ssl(void)
{
    /* Document-class: PolarSSL::SSL
     * This class is the base for doing SSL communication over a socket.
     *
     * == Sample
     *
     *      require 'polarssl'
     *
     *      socket = TCPSocket.new('polarssl.org', 443)
     *
     *      entropy = PolarSSL::Entropy.new
     *      ctr_drbg = PolarSSL::CtrDrbg.new(entropy)
     *      ssl = PolarSSL::SSL.new
     *
     *      ssl.set_endpoint(PolarSSL::SSL::SSL_IS_CLIENT)
     *      ssl.set_authmode(PolarSSL::SSL::SSL_VERIFY_NONE)
     *      ssl.set_rng(ctr_drbg)
     *
     *      ssl.set_socket(socket)
     *
     *      ssl.handshake
     *
     *      ssl.write("GET / HTTP/1.0\r\nHost: polarssl.org\r\n\r\n")
     *
     *      while chunk = ssl.read(1024)
     *          response << chunk
     *      end
     *
     *      puts response
     *
     *      ssl.close_notify
     *
     *      socket.close
     *
     *      ssl.close
     */
    VALUE cSSL = rb_define_class_under( rb_mPolarSSL, "SSL", rb_path2class("Object") );

    /* Document-class: PolarSSL::MallocFailed
     * Raised when not enough memory can be allocated for initializing the ssl context with ssl_init();
     */
    e_MallocFailed = rb_define_class_under( rb_mPolarSSL, "MallocFailed", rb_path2class("StandardError") );

    /* Document-class: PolarSSL::NetWantRead
     * Raised when the ssl connection expects a read.
     */
    e_NetWantRead = rb_define_class_under( rb_mPolarSSL, "NetWantRead", rb_eStandardError );

    /* Document-class: PolarSSL::NetWantWrite
     * Raised when the ssl connection expects a write.
     */
    e_NetWantWrite = rb_define_class_under( rb_mPolarSSL, "NetWantWrite", rb_eStandardError );

    /* Document-class: PolarSSL::SSL::Error
     * Raised when an SSL error occurs.
     */
    e_SSLError = rb_define_class_under( cSSL, "Error", rb_eRuntimeError );

    /*
     * 0: Endpoint mode for acting as a client.
     */
    rb_define_const( cSSL, "SSL_IS_CLIENT", INT2NUM( SSL_IS_CLIENT ) );

    /*
     * 0: Certificate verification mode for doing no verification.
     */
    rb_define_const( cSSL, "SSL_VERIFY_NONE", INT2NUM( SSL_VERIFY_NONE ) );

    /*
     * 1: Certificate verification mode for optional verification.
     */
    rb_define_const( cSSL, "SSL_VERIFY_OPTIONAL", INT2NUM( SSL_VERIFY_OPTIONAL ) );

    /*
     * 2: Certificate verification mode for having required verification.
     */
    rb_define_const( cSSL, "SSL_VERIFY_REQUIRED", INT2NUM( SSL_VERIFY_REQUIRED ) );

    rb_define_alloc_func( cSSL, R_ssl_allocate );
    rb_define_method( cSSL, "set_endpoint", R_ssl_set_endpoint, 1 );
    rb_define_method( cSSL, "set_authmode", R_ssl_set_authmode, 1 );
    rb_define_method( cSSL, "set_rng", R_ssl_set_rng, 1 );
    rb_define_method( cSSL, "set_socket", R_ssl_set_socket, 1);
    rb_define_method( cSSL, "handshake", R_ssl_handshake, 0 );
    rb_define_method( cSSL, "write", R_ssl_write, 1 );
    rb_define_method( cSSL, "read", R_ssl_read, 1 );
    rb_define_method( cSSL, "close_notify", R_ssl_close_notify, 0 );
    rb_define_method( cSSL, "close", R_close, 0 );
}

static VALUE R_ssl_allocate( VALUE klass )
{
    ssl_context *ssl;
    int ret;

    #if POLARSSL_VERSION_MAJOR == 1 && POLARSSL_VERSION_MINOR == 1
    ssl_session *ssn;
    #endif

    ssl = ALLOC( ssl_context );

    ret = ssl_init( ssl );

    if ( ret == POLARSSL_ERR_SSL_MALLOC_FAILED )
        rb_raise(e_MallocFailed, "ssl_init() memory allocation failed.");

    #if POLARSSL_VERSION_MAJOR == 1 && POLARSSL_VERSION_MINOR == 1
    ssn = ALLOC( ssl_session );
    ssl_set_session( ssl, 0, 600, ssn );
    ssl_set_ciphersuites( ssl, ssl_default_ciphersuites );
    #endif

    // ssl_set_dbg(ssl, my_debug, stdout);

    return Data_Wrap_Struct( klass, 0, ssl_free, ssl );
}

/*
 *  call-seq:
 *      set_endpoint( endpoint_mode )
 *
 *  Sets the endpoint mode for the current SSL connection to act as a server or a client.
 *  Possible values are:
 *
 *  * PolarSSL::SSL::SSL_IS_CLIENT
 */
static VALUE R_ssl_set_endpoint( VALUE self, VALUE endpoint_mode )
{
    ssl_context *ssl;

    Check_Type( endpoint_mode, T_FIXNUM );

    Data_Get_Struct( self, ssl_context, ssl );

    ssl_set_endpoint( ssl, NUM2INT( endpoint_mode ) );

    return Qtrue;
}

/*
 *  call-seq:
 *      set_authmode( authentication_mode )
 *
 *  Sets the certificate verification mode for the SSL connection.
 *  Possible values are:
 *
 *  * PolarSSL::SSL::SSL_VERIFY_NONE
 *  * PolarSSL::SSL::SSL_VERIFY_OPTIONAL
 *  * PolarSSL::SSL::SSL_VERIFY_REQUIRED
 */
static VALUE R_ssl_set_authmode( VALUE self, VALUE authmode )
{
    ssl_context *ssl;

    Check_Type( authmode, T_FIXNUM );

    Data_Get_Struct( self, ssl_context, ssl );

    ssl_set_authmode( ssl, NUM2INT( authmode ) );

    return Qtrue;
}

/*
 *  call-seq:
 *      set_rng( random_number_generator )
 *
 *  Sets the random number generator to be used for this connection. You need to use
 *  an instance of PolarSSL::CtrDrbg for this. Example:
 *
 *      entropy = PolarSSL::Entropy.new
 *      ctr_drbg = PolarSSL::CtrDrbg.new(entropy)
 *
 *      ssl = PolarSSL::SSL.new
 *      ssl.set_rng(ctr_drbg)
 *
 *      # other ssl connection settings and usage
 *
 */
static VALUE R_ssl_set_rng( VALUE self, VALUE rng )
{
    ssl_context *ssl;
    ctr_drbg_context *ctr_drbg;

    Check_Type( rng, T_DATA );

    Data_Get_Struct( self, ssl_context, ssl );
    Data_Get_Struct( rng, ctr_drbg_context, ctr_drbg );

    ssl_set_rng( ssl, ctr_drbg_random, ctr_drbg );

    return Qtrue;
}

/*
 *  call-seq:
 *      set_socket( socket )
 *
 *  Set the TCPSocket to be used for this SSL connection. Example:
 *
 *      socket = TCPSocket.new('polarssl.org', 443)
 *
 *      ssl = PolarSSL::SSL.new
 *      ssl.set_socket(socket)
 *
 */
static VALUE R_ssl_set_socket( VALUE self, VALUE socket )
{
    ssl_context *ssl;
    rb_io_t *fptr;

    Check_Type( socket, T_FILE );

    Data_Get_Struct( self, ssl_context, ssl );

    GetOpenFile( socket, fptr );

    ssl_set_bio( ssl, net_recv, &fptr->fd, net_send, &fptr->fd );

    return Qtrue;
}

/*
 *  call-seq:
 *      handshake() -> true or exception
 *
 *  Initiates SSL handshake. When something goes wrong, raises one of:
 *
 *  * PolarSSL::NetWantRead,
 *  * PolarSSL::NetWantWrite or
 *  * PolarSSL::SSL::Error
 */
static VALUE R_ssl_handshake(VALUE self)
{
    ssl_context *ssl;
    int ret;

    Data_Get_Struct( self, ssl_context, ssl );

    ret = ssl_handshake( ssl );

    if ( ret < 0 )
    {
        if ( ret == POLARSSL_ERR_NET_WANT_READ )
        {
          rb_raise( e_NetWantRead, "ssl_handshake() returned POLARSSL_ERR_NET_WANT_READ" );
        }
        else if ( ret == POLARSSL_ERR_NET_WANT_WRITE )
        {
          rb_raise( e_NetWantWrite, "ssl_handshake() returned POLARSSL_ERR_NET_WANT_WRITE" );
        }
        else
        {
          rb_raise( e_SSLError, "-0x%x", -ret );
        }
    }
    else
    {
        return Qtrue;
    }
}

/*
 *  call-seq:
 *      write( string ) -> true or raises
 *
 *  Writes to the SSL connection or raises PolarSSL::SSL::Error.
 */
static VALUE R_ssl_write( VALUE self, VALUE string )
{
    ssl_context *ssl;
    char *buffer;
    int ret;

    Check_Type( string, T_STRING );

    Data_Get_Struct( self, ssl_context, ssl );

    buffer = RSTRING_PTR( string );

    ret = ssl_write( ssl, (const unsigned char *) buffer, RSTRING_LEN( string ) );

    if ( ret < 0 )
        rb_raise( e_SSLError, "-0x%x", -ret );

    return Qtrue;
}

/*
 *  call-seq:
 *      read( length ) -> string or raises
 *
 *  Reads +length+ bytes from the SSL connection or raises PolarSSL::SSL::Error.
 */
static VALUE R_ssl_read( VALUE self, VALUE length )
{
    ssl_context *ssl;
    VALUE result;
    int buffer_size = NUM2INT( length );
    unsigned char buffer[buffer_size];
    int length_to_read;
    int length_read;

    Check_Type( length, T_FIXNUM );

    Data_Get_Struct( self, ssl_context, ssl );

    length_to_read = sizeof( buffer ) - 1;
    length_read = ssl_read( ssl, buffer, length_to_read );

    if ( length_read == 0 || length_read == POLARSSL_ERR_SSL_PEER_CLOSE_NOTIFY )
    {
        result = Qnil;
    }
    else if (length_read < 0)
    {
        rb_raise( e_SSLError, "-0x%x", -length_read );
    }
    else
    {
        result = rb_str_new2( (char *)buffer );
    }

    return result;
}

/*
 *  call-seq:
 *      close_notify() -> true or raises
 *
 *  Notifies the peer to close the connection. true or raises PolarSSL::SSL::Error
 */
static VALUE R_ssl_close_notify( VALUE self )
{
    ssl_context *ssl;
    int ret;
    Data_Get_Struct( self, ssl_context, ssl );

    ret = ssl_close_notify( ssl );

    if (ret < 0)
        rb_raise( e_SSLError, "-0x%x", -ret );

    return Qtrue;
}

/*
 *  call-seq:
 *      close() -> true
 *
 *  Final method to be called when done using the SSL connection. This immediately frees
 *  any SSL data in the memory instead of waiting for the Ruby garbage collector.
 */
static VALUE R_close( VALUE self )
{
    ssl_context *ssl;
    Data_Get_Struct( self, ssl_context, ssl );

    memset( ssl, 0, sizeof( ssl_context ) );

    return Qtrue;
}
