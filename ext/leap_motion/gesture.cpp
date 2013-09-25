#include <leap_motion.h>

VALUE cGestureList;
VALUE cGesture;

static void gl_dealloc(void * gl)
{
  delete reinterpret_cast<Leap::GestureList*>(gl);
}

VALUE WrapGestureList(Leap::GestureList * gl)
{
  return Data_Wrap_Struct(cGestureList, 0, gl_dealloc, gl);
}

static void gesture_dealloc(void * g)
{
  delete reinterpret_cast<Leap::Gesture*>(g);
}

VALUE WrapGesture(Leap::Gesture * gesture)
{
  return Data_Wrap_Struct(cGesture, 0, gesture_dealloc, gesture);
}

ListCountImpl(cGestureList, Leap::GestureList);
ListEmptyImpl(cGestureList, Leap::GestureList);

static VALUE each(VALUE self)
{
  Leap::GestureList * list;
  int i;

  Data_Get_Struct(self, Leap::GestureList, list);

  for (i = 0; i < list->count(); i++) {
    Leap::Gesture * gesture;
    gesture = new Leap::Gesture((*list)[i]);
    rb_yield(WrapGesture(gesture));
  }

  return self;
}

static VALUE gesture_type(VALUE self)
{
  Leap::Gesture * gesture;
  Data_Get_Struct(self, Leap::Gesture, gesture);
  return INT2NUM(gesture->type());
}

static VALUE gesture_to_s(VALUE self)
{
  Leap::Gesture * gesture;
  const char * string;

  Data_Get_Struct(self, Leap::Gesture, gesture);

  string = gesture->toString().c_str();
  return rb_str_new2(string);
}

void Init_leap_gesture()
{
  cGestureList = rb_define_class_under(mLeapMotion, "GestureList", rb_cObject);
  ListCountDecl(cGestureList, Leap::GestureList);
  ListEmptyDecl(cGestureList, Leap::GestureList);
  rb_define_method(cGestureList, "each", (ruby_method_vararg *)each, 0);

  cGesture = rb_define_class_under(mLeapMotion, "Gesture", rb_cObject);
  rb_define_method(cGesture, "to_s", (ruby_method_vararg *)gesture_to_s, 0);
  rb_define_method(cGesture, "type", (ruby_method_vararg *)gesture_type, 0);
}
