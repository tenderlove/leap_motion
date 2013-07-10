require 'leap_motion.so'
require 'set'
require 'thread'

module LeapMotion
  VERSION = '1.0.0'

  class Controller
    def initialize
      super
      @listeners = Set.new
      @mutex = Mutex.new
    end

    def add_listener listener
      @mutex.synchronize do
        return false if @listeners.include? listener
        @listeners << listener
        listen!
      end
    end

    def remove_listener listener
      @mutex.synchronize do
        return false unless @listeners.include? listener
        @listeners.delete listener
        unlisten! if @listeners.empty?
      end
    end

    private
    def on_init
      @listeners.each { |listener| listener.on_init self }
    end

    def on_connect
      @listeners.each { |listener| listener.on_connect self }
    end

    def on_disconnect
      @listeners.each { |listener| listener.on_disconnect self }
    end

    def on_exit
      @listeners.each { |listener| listener.on_exit self }
    end

    def on_frame
      @listeners.each { |listener| listener.on_frame self }
    end

    def on_focus_gained
      @listeners.each { |listener| listener.on_focus_gained self }
    end

    def on_focus_lost
      @listeners.each { |listener| listener.on_focus_lost self }
    end
  end

  class Listener
    def on_init controller
    end

    def on_connect controller
    end

    def on_disconnect controller
    end

    def on_exit controller
    end

    def on_frame controller
    end

    def on_focus_gained controller
    end

    def on_focus_lost controller
    end
  end
end
