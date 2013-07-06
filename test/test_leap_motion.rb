require 'minitest/autorun'
require 'leap_motion'

class TestLeapMotion < MiniTest::Unit::TestCase
  def test_sanity
    assert LeapMotion::Controller.new
  end
end
