#ifdef __cplusplus
#include <Leap.h>
#endif

#include <ruby.h>

typedef VALUE (ruby_method_vararg)(...);

extern VALUE mLeapMotion;
extern VALUE cController;
extern VALUE cFrame;

extern "C" void Init_leap_motion(void);
extern "C" void Init_leap_frame(void);

