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
    refute controller.add_listener listener
  end

  def test_remove_listener
    controller = LeapMotion::Controller.new
    listener = LeapMotion::Listener.new
    assert controller.add_listener listener
    assert controller.remove_listener listener
    refute controller.remove_listener listener
  end

  def test_connected?
    controller = LeapMotion::Controller.new
    refute controller.connected?
  end
end
