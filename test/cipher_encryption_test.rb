require 'test_helper'
require 'base64'
require 'securerandom'

class CipherTest < MiniTest::Unit::TestCase
  CIPHER = 'AES-128-CTR'
  VALUE = 'test data value'
  KEY = 'bar'

  def test_aes_128_ctr_encrypt
    iv = SecureRandom.random_bytes(16)

    cipher = PolarSSL::Cipher.new CIPHER
    cipher.setkey KEY, 128, PolarSSL::Cipher::OPERATION_ENCRYPT
    cipher.set_iv(iv, 16)
    cipher.reset
    cipher.update VALUE
    encrypted = cipher.finish

    cipher = PolarSSL::Cipher.new CIPHER
    cipher.setkey KEY, 128, PolarSSL::Cipher::OPERATION_DECRYPT
    cipher.set_iv(iv, 16)
    cipher.reset
    cipher.update encrypted
    decrypted = cipher.finish

    assert_equal VALUE, decrypted
  end

  def test_unsupported_cipher
    assert_raises PolarSSL::Cipher::UnsupportedCipher do
      PolarSSL::Cipher.new("meh")
    end
  end
  
  def test_unsupported_key
    assert_raises PolarSSL::Cipher::Error do
      cipher = PolarSSL::Cipher.new("AES-128-CTR")
      cipher.setkey("1234567890123456", 127, PolarSSL::Cipher::OPERATION_ENCRYPT)
    end
  end
end
