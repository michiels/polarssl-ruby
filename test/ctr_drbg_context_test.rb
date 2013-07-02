require_relative 'test_helper'

class CtrDrbgContextTest < MiniTest::Unit::TestCase

  def test_new
    ctr_drbg = PolarSSL::CtrDrbgContext.new
  end

end
