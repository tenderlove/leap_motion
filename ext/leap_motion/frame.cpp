#include <leap_motion.h>

VALUE cFrame;

static void frame_dealloc(void * frame)
{
  delete reinterpret_cast<Leap::Frame*>(frame);
}

VALUE WrapFrame(Leap::Frame * f)
{
  return Data_Wrap_Struct(cFrame, 0, frame_dealloc, f);
}

static VALUE valid_p(VALUE self)
{
  Leap::Frame * f;

  Data_Get_Struct(self, Leap::Frame, f);

  if (true == f->isValid()) {
    return Qtrue;
  }

  return Qfalse;
}

static VALUE frame_id(VALUE self)
{
  Leap::Frame * f;

  Data_Get_Struct(self, Leap::Frame, f);

  return INT2NUM(f->id());
}

static VALUE frame_timestamp(VALUE self)
{
  Leap::Frame * f;

  Data_Get_Struct(self, Leap::Frame, f);

  return INT2NUM(f->timestamp());
}

static VALUE frame_invalid(VALUE self)
{
  Leap::Frame * f;
  Leap::Frame * invalid;

  Data_Get_Struct(self, Leap::Frame, f);

  invalid = new Leap::Frame(f->invalid());

  return Data_Wrap_Struct(cFrame, 0, 0, invalid);
}

static VALUE frame_to_s(VALUE self)
{
  Leap::Frame * f;
  const char * string;

  Data_Get_Struct(self, Leap::Frame, f);

  string = f->toString().c_str();
  return rb_str_new2(string);
}

/*
static VALUE frame_hands(VALUE self)
{
  Leap::Frame * f;
  const char * string;

  Data_Get_Struct(self, Leap::Frame, f);
  Data_Get_Struct(self, Leap::Frame, f);

  string = f->toString().c_str();
  return rb_str_new2(string);
}
*/

void Init_leap_frame()
{
  cFrame = rb_define_class_under(mLeapMotion, "Frame", rb_cObject);

  rb_define_method(cFrame, "valid?", (ruby_method_vararg *)valid_p, 0);
  rb_define_method(cFrame, "id", (ruby_method_vararg *)frame_id, 0);
  rb_define_method(cFrame, "timestamp", (ruby_method_vararg *)frame_timestamp, 0);
  rb_define_method(cFrame, "invalid", (ruby_method_vararg *)frame_invalid, 0);
  rb_define_method(cFrame, "to_s", (ruby_method_vararg *)frame_to_s, 0);
  // rb_define_method(cFrame, "hands", (ruby_method_vararg *)frame_hands, 0);
}
