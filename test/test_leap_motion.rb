require 'minitest/autorun'
require 'leap_motion'

class TestLeapMotion < MiniTest::Unit::TestCase
  def test_sanity
    assert LeapMotion::Controller.new
  end

  def test_listener
    assert LeapMotion::Listener.new
  end
end
