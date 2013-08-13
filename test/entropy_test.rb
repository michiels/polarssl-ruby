require_relative 'test_helper'

class EntropyTest < MiniTest::Unit::TestCase

  def test_initialize
    GC.stress = true
    entropy = PolarSSL::Entropy.new
  end

  def test_gather
    GC.stress = true
    entropy = PolarSSL::Entropy.new
    assert entropy.gather()
  end

end
