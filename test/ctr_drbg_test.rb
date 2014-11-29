require 'test_helper'

class CtrDrbgTest < Minitest::Test

  def test_new
    entropy = PolarSSL::Entropy.new
    ctr_drbg = PolarSSL::CtrDrbg.new(entropy)
  end

end
