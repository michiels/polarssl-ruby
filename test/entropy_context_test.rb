require_relative 'test_helper'

class EntropyContextTest < MiniTest::Unit::TestCase

  def test_initialize_entropy_context
    entropy = PolarSSL::EntropyContext.new()
  end

end
