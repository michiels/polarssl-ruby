require_relative 'test_helper'
require 'socket'

class SSLConnectionTest < MiniTest::Unit::TestCase

  def test_simple_connection
    socket = TCPSocket.new('polarssl.org', 443)

    entropy = PolarSSL::Entropy.new

    ctr_drbg = PolarSSL::CtrDrbg.new(entropy)

    ssl = PolarSSL::SSL.new

    ssl.set_endpoint(PolarSSL::SSL::SSL_IS_CLIENT)
    ssl.set_authmode(PolarSSL::SSL::SSL_VERIFY_NONE)
    ssl.set_rng(ctr_drbg)

    ssl.set_bio(Proc.new { |fp| }, socket, Proc.new { |fp| }, socket)

    ssl.handshake

    ssl.write("GET / HTTP/1.0\r\n\r\n")

    ssl.read(1024)

    ssl.close_notify

    socket.close

    ssl.close
  end

end
