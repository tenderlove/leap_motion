require 'leap_motion.so'
require 'set'
require 'thread'

module LeapMotion
  VERSION = '1.0.0'

  class HandList;      include Enumerable; end
  class FingerList;    include Enumerable; end
  class PointableList; include Enumerable; end

  class Controller
    EVENTS = [:init, :connect, :disconnect, :frame, :focus_gained, :focus_lost]

    def initialize
      super
      @listeners = Set.new
      @mutex     = Mutex.new
      ios        = EVENTS.map { |event| IO.new send "#{event}_fd" }
      @listener  = start_listener ios
    end

    def join; @listener.join; end

    def each_frame
      reader = IO.new frame_fd
      listen!
      loop do
        rs, = IO.select [reader]
        rs.each { |io|
          io.read 1
          yield frame
        }
      end
    ensure
      unlisten! if @listeners.empty?
    end

    def add_listener listener
      @mutex.synchronize do
        return false if @listeners.include? listener
        @listeners = @listeners.dup.add listener
        listen!
      end
    end

    def remove_listener listener
      @mutex.synchronize do
        return false unless @listeners.include? listener
        @listeners = @listeners.dup.delete listener
        unlisten! if @listeners.empty?
      end
    end

    private

    def start_listener ios
      methods = EVENTS.map { |e| "on_#{e}" }
      dispatch = Hash[ios.zip(methods)]
      Thread.new do
        loop do
          rs, = IO.select ios
          rs.each { |io|
            io.read 1
            send dispatch[io]
          }
        end
      end
    end

    EVENTS.each do |event|
      class_eval <<-EORUBY, __FILE__, __LINE__ + 1
      def on_#{event}
        @listeners.each { |l| l.on_#{event} self }
      end
      EORUBY
    end
  end

  module Events
    def on_init controller; end
    def on_connect controller; end
    def on_disconnect controller; end
    def on_exit controller; end
    def on_frame controller; end
    def on_focus_gained controller; end
    def on_focus_lost controller; end
  end

  class Listener
    include Events
  end

  class Vector
    UP       = new 0, 1, 0
    DOWN     = new 0, -1, 0
    FORWARD  = new 0, 0, -1
    BACKWARD = new 0, 0, 1
    LEFT     = new(-1, 0, 0)
    RIGHT    = new 1, 0, 0
    X_AXIS   = new 1, 0, 0
    Y_AXIS   = new 0, 1, 0
    Z_AXIS   = new 0, 0, 1
    ZERO     = new 0, 0, 0

    def to_a
      [x, y, z]
    end

    def [] idx
      to_a.fetch(idx, 0.0)
    end
  end
end
