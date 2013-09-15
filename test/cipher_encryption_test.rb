require 'test_helper'
require 'base64'
require 'securerandom'

class CipherTest < MiniTest::Unit::TestCase

  def test_aes_128_ctr_encrypt
    cipher = PolarSSL::Cipher.new("AES-128-CTR")
    
    key = hex_to_bin("2b7e151628aed2a6abf7158809cf4f3c")
    iv = hex_to_bin("f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff")
    
    # TODO: use random generate iv
    # iv = SecureRandom.random_bytes(16)
    
    cipher.setkey(key, 128, PolarSSL::Cipher::OPERATION_ENCRYPT)
    
    cipher.reset(iv)
    
    cipher.update(hex_to_bin("6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e51"))

    encrypted = cipher.finish

    encrypted_base64 = Base64.encode64(encrypted)

    assert !encrypted.nil?
    assert !encrypted_base64.nil?

    should_encrypt_as = hex_to_bin("874d6191b620e3261bef6864990db6ce9806f66b7970fdff8617187bb9fffdff")
    assert_equal should_encrypt_as, encrypted
    
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
  
  private
  
  def hex_to_bin(hex)
    hex.scan(/../).map { |x| x.hex.chr }.join
  end
  
  def bin_to_hex(data)
    data.each_byte.map { |b| b.to_s(16).join }
  end

end