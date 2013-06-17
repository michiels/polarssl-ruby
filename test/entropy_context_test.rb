require_relative 'test_helper'

class EntropyContextTest < MiniTest::Unit::TestCase

  def test_initialize
    entropy = PolarSSL::EntropyContext.new
  end

  def test_gather
    entropy = PolarSSL::EntropyContext.new
    entropy.gather()
  end

end
