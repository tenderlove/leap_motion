#ifdef __cplusplus
#include <Leap.h>
#endif

#include <ruby.h>

typedef VALUE (ruby_method_vararg)(...);

extern VALUE mLeapMotion;
extern VALUE cController;

extern "C" void Init_leap_motion(void);
extern "C" void Init_leap_frame(void);
extern "C" void Init_leap_hand(void);
extern "C" void Init_leap_finger(void);

#define ListCountImpl(rbklass, klass) \
  static VALUE rb_##rbklass##_count(VALUE self) \
{ \
  klass * list; \
  Data_Get_Struct(self, klass, list); \
  return INT2NUM(list->count()); \
}

#define ListCountDecl(rbklass, klass) \
  rb_define_method(rbklass, "count", (ruby_method_vararg *)rb_##rbklass##_count, 0);


VALUE WrapFrame(Leap::Frame * f);
VALUE WrapHandList(Leap::HandList * hl);
VALUE WrapFingerList(Leap::FingerList * hl);
VALUE WrapFinger(Leap::Finger * finger);

