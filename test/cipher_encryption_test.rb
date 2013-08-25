require 'test_helper'

class CipherTest < MiniTest::Unit::TestCase

  def test_aes_128_ctr_encrypt
    cipher = PolarSSL::Cipher.new

    cipher.setkey("1234567890123456", 128)
    cipher.update("hallo")
    encrypted = cipher.finish

    assert_not_nil encrypted
  end

end