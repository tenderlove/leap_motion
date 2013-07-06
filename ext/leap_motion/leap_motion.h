#ifdef __cplusplus
#include <Leap.h>
#endif

typedef void * LeapController;

extern "C" LeapController new_controller(void);
extern "C" void Init_leap_ruby(void);

