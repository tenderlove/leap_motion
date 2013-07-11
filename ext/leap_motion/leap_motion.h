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

VALUE WrapFrame(Leap::Frame * f);
VALUE WrapHandList(Leap::HandList * hl);

