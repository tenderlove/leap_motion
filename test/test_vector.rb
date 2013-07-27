require 'minitest/autorun'
require 'leap_motion'

module LeapMotion
  class TestVector < MiniTest::Test
    def test_init
      v = LeapMotion::Vector.new(1,2,3)
      assert_equal 1, v.x
      assert_equal 2, v.y
      assert_equal 3, v.z
    end

    def test_eql
      v1 = LeapMotion::Vector.new(1,2,3)
      v2 = LeapMotion::Vector.new(1,2,3)
      assert_equal v1, v2
    end

    def test_not_eql
      v1 = LeapMotion::Vector.new(1,2,3)
      v2 = LeapMotion::Vector.new(2,2,3)
      refute_equal v1, v2
    end

    def test_magnitude
      assert Vector::ZERO.magnitude
    end

    def test_magnitude_squared
      assert Vector::ZERO.magnitude_squared
    end

    def test_distance_to
      assert Vector::ZERO.distance_to(Vector::X_AXIS)
    end

    def test_angle_to
      assert Vector::Y_AXIS.angle_to(Vector::X_AXIS)
    end

    def test_pitch
      assert Vector::Y_AXIS.pitch
    end

    def test_yaw
      assert Vector::Y_AXIS.yaw
    end

    def test_roll
      assert Vector::Y_AXIS.roll
    end

    def test_dot
      assert Vector::Y_AXIS.dot(Vector::X_AXIS)
    end

    def test_cross
      assert_equal Vector::ZERO, Vector::X_AXIS.cross(Vector::ZERO)
    end

    def test_zero
      assert_equal Vector.new(0,0,0), Vector::ZERO
    end

    def test_x_axis
      assert_equal Vector.new(1,0,0), Vector::X_AXIS
    end

    def test_y_axis
      assert_equal Vector.new(0,1,0), Vector::Y_AXIS
    end

    def test_z_axis
      assert_equal Vector.new(0,0,1), Vector::Z_AXIS
    end

    def test_left
      assert_equal Vector.new(-1,0,0), Vector::LEFT
    end

    def test_right
      assert_equal Vector.new(1,0,0), Vector::RIGHT
    end

    def test_down
      assert_equal Vector.new(0,-1,0), Vector::DOWN
    end

    def test_up
      assert_equal Vector.new(0,1,0), Vector::UP
    end

    def test_forward
      assert_equal Vector.new(0,0,-1), Vector::FORWARD
    end

    def test_backward
      assert_equal Vector.new(0,0,1), Vector::BACKWARD
    end
  end
end
