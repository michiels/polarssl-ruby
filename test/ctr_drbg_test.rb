require_relative 'test_helper'

class CtrDrbgTest < MiniTest::Unit::TestCase

  def test_new
    entropy = PolarSSL::Entropy.new
    ctr_drbg = PolarSSL::CtrDrbg.new(entropy)
  end

end
