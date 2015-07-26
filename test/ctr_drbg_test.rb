require 'test_helper'

class CtrDrbgTest < MiniTest::Test

  def test_new
    entropy = PolarSSL::Entropy.new
    ctr_drbg = PolarSSL::CtrDrbg.new(entropy)
  end

end
