require 'test_helper'

class EntropyTest < MiniTest::Unit::TestCase

  def test_initialize
    entropy = PolarSSL::Entropy.new
  end

  def test_gather
    entropy = PolarSSL::Entropy.new
    assert entropy.gather()
  end

end
