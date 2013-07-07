#include <leap_motion.h>
#include <ruby.h>

LeapController new_controller()
{
  return reinterpret_cast<void*>(new Leap::Controller);
}

VALUE mLeapMotion;
VALUE cController;

static VALUE allocate(VALUE klass)
{
  LeapController controller = new_controller();
  return Data_Wrap_Struct(klass, 0, 0, controller);
}

void Init_leap_motion()
{
  mLeapMotion = rb_define_module("LeapMotion");
  cController = rb_define_class_under(mLeapMotion, "Controller", rb_cObject);

  rb_define_alloc_func(cController, allocate);
}
