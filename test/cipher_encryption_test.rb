require 'test_helper'
require 'base64'


class CipherTest < MiniTest::Unit::TestCase

  def test_aes_128_ctr_encrypt
    cipher = PolarSSL::Cipher.new("AES-128-CTR")

    cipher.setkey("1234567890123456", 128, PolarSSL::Cipher::OPERATION_ENCRYPT)
    cipher.update("Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Sed posuere consectetur est at lobortis. Curabitur blandit tempus porttitor. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Sed posuere consectetur est at lobortis.")
    encrypted = cipher.finish

    encrypted_base64 = Base64.encode64(encrypted)
    puts encrypted_base64

    assert !encrypted.nil?
    assert !encrypted_base64.nil?
  end

end