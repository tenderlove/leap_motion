require 'minitest/autorun'
require 'leap_motion'

class TestLeapMotion < MiniTest::Unit::TestCase
  attr_reader :controller

  def setup
    super
    @controller = LeapMotion::Controller.new
  end

  def test_listener
    assert LeapMotion::Listener.new
  end

  def test_add_listener
    listener = LeapMotion::Listener.new
    assert controller.add_listener listener
    refute controller.add_listener listener
  end

  def test_remove_listener
    listener = LeapMotion::Listener.new
    assert controller.add_listener listener
    assert controller.remove_listener listener
    refute controller.remove_listener listener
  end

  def test_connected?
    refute controller.connected?
  end

  def test_has_focus?
    refute controller.has_focus?
  end

  def test_policy_flags
    assert_equal 0, controller.policy_flags
  end
end
