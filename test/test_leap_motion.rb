require 'minitest/autorun'
require 'leap_motion'

class TestLeapMotion < MiniTest::Unit::TestCase
  def test_sanity
    assert LeapMotion::Controller.new
  end

  def test_listener
    assert LeapMotion::Listener.new
  end

  def test_add_listener
    controller = LeapMotion::Controller.new
    listener = LeapMotion::Listener.new
    assert controller.add_listener listener
  end
end
