require 'minitest/autorun'
require 'leap_motion'

class TestVector < MiniTest::Test
  def test_init
    v = LeapMotion::Vector.new(1,2,3)
    assert_equal 1, v.x
    assert_equal 2, v.y
    assert_equal 3, v.z
  end
end
