#ifdef __cplusplus
#include <Leap.h>
#endif

#include <ruby.h>

typedef VALUE (ruby_method_vararg)(...);

extern VALUE mLeapMotion;
extern VALUE cController;
extern VALUE cPointable;

extern "C" void Init_leap_motion(void);
extern "C" void Init_leap_frame(void);
extern "C" void Init_leap_hand(void);
extern "C" void Init_leap_finger(void);
extern "C" void Init_leap_tool(void);
extern "C" void Init_leap_vector(void);
extern "C" void Init_leap_pointable(void);
extern "C" void Init_leap_gesture(void);

#define ListCountImpl(rbklass, klass) \
  static VALUE rb_##rbklass##_count(VALUE self) \
{ \
  klass * list; \
  Data_Get_Struct(self, klass, list); \
  return INT2NUM(list->count()); \
}

#define ListCountDecl(rbklass, klass) \
  rb_define_method(rbklass, "count", (ruby_method_vararg *)rb_##rbklass##_count, 0);

#define ListEmptyImpl(rbklass, klass) \
  static VALUE rb_##rbklass##_empty_p(VALUE self) \
{ \
  klass * list; \
  Data_Get_Struct(self, klass, list); \
  if (list->isEmpty()) { \
    return Qtrue; \
  } \
  return Qfalse; \
}

#define ListEmptyDecl(rbklass, klass) \
  rb_define_method(rbklass, "empty?", (ruby_method_vararg *)rb_##rbklass##_empty_p, 0);


VALUE WrapFrame(Leap::Frame * f);
VALUE WrapHandList(Leap::HandList * hl);
VALUE WrapHand(Leap::Hand * h);

VALUE WrapFingerList(Leap::FingerList * fl);
VALUE WrapFinger(Leap::Finger * finger);

VALUE WrapGestureList(Leap::GestureList * gl);
VALUE WrapGesture(Leap::Gesture * gesture);

VALUE WrapToolList(Leap::ToolList * tl);
VALUE WrapTool(Leap::Tool * tool);

VALUE WrapPointableList(Leap::PointableList * pl);
VALUE WrapVector(Leap::Vector * v);
VALUE MakePointable(Leap::Pointable pointable);

