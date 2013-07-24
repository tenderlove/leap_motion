require 'minitest/autorun'
require 'leap_motion'
require 'monitor'

class TestLeapMotion < MiniTest::Test
  attr_reader :controller

  def setup
    super
    @controller = LeapMotion::Controller.new
  end

  def test_listener
    assert LeapMotion::Listener.new
  end

  class Latch
    def initialize
      @locked = true
      @lock = Monitor.new
      @cv = @lock.new_cond
    end

    def release
      @lock.synchronize do
        @locked = false
        @cv.broadcast
      end
    end
    def await; @lock.synchronize { @cv.wait_while { @locked } }; end
  end

  def test_listener_gets_info
    events = []
    latch = Latch.new
    listener = Class.new {
      LeapMotion::Controller::EVENTS.each do |event|
        define_method("on_#{event}") { |c| events << c; latch.release }
      end
    }.new
    controller.add_listener listener
    latch.await
    controller.remove_listener listener
    refute_predicate events, :empty?
    assert_includes events, controller
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
