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

    # TODO: Implement passing methods/procs to people can define their own send/recv methods
    ssl.set_bio(Proc.new { |fp| }, socket, Proc.new { |fp| }, socket)

    ssl.handshake

    ssl.write("GET / HTTP/1.0\r\nHost: polarssl.org\r\n\r\n")

    while ret = ssl.read(1024)
        puts "==== chunk #{ret.length} ====="
        puts ret
    end

    ssl.close_notify

    socket.close

    # TODO: Specifically close SSL context so memory is freed and no-one can read from memory.
    ssl.close
  end

end
