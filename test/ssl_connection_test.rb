require_relative 'test_helper'

class SSLConnectionTest < MiniTest::Unit::Testcase

  def test_simple_connection
    entropy = PolarSSL::EntropyContext.new

    ctr_drbg = PolarSSL::CtrDrbg.new(entropy, "seed string")

    ssl = PolarSSL::SSLContext.new

    ssl.set_endpoint(PolarSSL::SSL_IS_CLIENT)
    ssl.set_authmode(PolarSSL::SSL_VERIFY_NONE)
    ssl.set_rng(ctr_drbg)

    ssl.set_bio(Proc.new { |fp| }, fp, Proc.new { |fp| }, fp)

    ssl.handshake

    ssl.write("GET / HTTP/1.0\r\n\r\n")

    ssl.read(1024)

    ssl.close_notify

    ssl.close
  end

end
