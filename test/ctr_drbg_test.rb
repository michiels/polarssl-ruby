require_relative 'test_helper'

class CtrDrbgTest < MiniTest::Unit::TestCase

  def test_new
    ctr_drbg = PolarSSL::CtrDrbg.new
  end

end
