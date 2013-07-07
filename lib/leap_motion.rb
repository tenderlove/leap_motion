require 'leap_motion.so'

module LeapMotion
  VERSION = '1.0.0'

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
