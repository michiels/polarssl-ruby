require 'test_helper'

class EntropyTest < Minitest::Test

  def test_initialize
    entropy = PolarSSL::Entropy.new
  end

  def test_gather
    entropy = PolarSSL::Entropy.new
    assert entropy.gather()
  end

end
