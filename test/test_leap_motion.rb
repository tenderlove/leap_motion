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

  class Recorder
    attr_reader :methods

    def initialize
      @methods = []
    end

    def method_missing name, controller
      @methods << [name, controller]
    end
  end

  def test_listener_gets_info
    listener = Recorder.new
    controller.add_listener listener
    controller.remove_listener listener
    refute_predicate listener.methods, :empty?
    assert_includes listener.methods.map(&:last), controller
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

  def test_frame
    frame = controller.frame
    refute frame.valid?
  end

  def test_frame_id
    frame = controller.frame
    assert frame.id
  end

  def test_timestamp
    frame = controller.frame
    assert frame.timestamp
  end

  def test_frame_invalid
    frame = controller.frame
    refute frame.valid?
    refute frame.invalid.valid?
  end

  def test_frame_equality
    frame = controller.frame
    assert_match 'Invalid', frame.to_s
  end
end
