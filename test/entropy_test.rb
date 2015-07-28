require 'test_helper'

class EntropyTest < MiniTest::Test

  def test_initialize
    entropy = PolarSSL::Entropy.new
  end

  def test_gather
    entropy = PolarSSL::Entropy.new
    assert entropy.gather()
  end

end
