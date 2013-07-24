require 'minitest/autorun'
require 'leap_motion'

class TestVector < MiniTest::Test
  def test_init
    v = LeapMotion::Vector.new(1,2,3)
    assert_equal 1, v.x
    assert_equal 2, v.y
    assert_equal 3, v.z
  end

  def test_eql
    v1 = LeapMotion::Vector.new(1,2,3)
    v2 = LeapMotion::Vector.new(1,2,3)
    assert_equal v1, v2
  end

  def test_not_eql
    v1 = LeapMotion::Vector.new(1,2,3)
    v2 = LeapMotion::Vector.new(2,2,3)
    refute_equal v1, v2
  end
end
