require 'test_helper'
require 'base64'


class CipherTest < MiniTest::Unit::TestCase

  def test_aes_128_ctr_encrypt
    cipher = PolarSSL::Cipher.new("AES-128-CTR")

    cipher.setkey("1234567890123456", 128, PolarSSL::Cipher::OPERATION_ENCRYPT)
    cipher.update("Fusce dapibus, tellus ac cursus commodo, tortor mauris condimentum nibh, ut fermentum massa justo sit amet risus. Sed posuere consectetur est at lobortis. Curabitur blandit tempus porttitor. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Sed posuere consectetur est at lobortis.")
    encrypted = cipher.finish

    encrypted_base64 = Base64.encode64(encrypted)

    assert !encrypted.nil?
    assert !encrypted_base64.nil?

    should_encrypt_as = <<EOF
nsDrK6JHaPXC8janRLIODtJfY8K6d+OfHQSSsdUSW/rDmzJ9GT1YeTllMPMA
MSKpyxi3BT8mLY+sR/cC+ElxAS5GSiiqrZ/D99WcqgNHwNNeXgTwghWZX9w3
rk+t6S5qwMs7aXoIZ+Hw/0Vb2IhLLcUA7V1ei3XHsdJ6XvVN8JhbuIkHrkVL
Cxn/UlDdNXk23dORm878TyvQ9839FBT80zflSyYjsHCj43HxC6KBg7lPNeEz
3swWwri2g2odFJojm0Mp0PS+bKFDgu+pXvGrCon5Fhy4nPJyFl73TvThF+w9
2FRm/A7Vb5U75wJAzv6FQZQSwfsACVLWPpuHiR7fV7Uyc2VIV2yOpaI9d3Ix
1neY3ANZhqZxWJ03MqrS/zzahJTJ16JbpyIvTTwoVoZWBe9ypuGO7uPrN7da
+kw=
EOF
  assert_equal should_encrypt_as, encrypted_base64
  end

end