#include <leap_motion.h>

VALUE cHandList;

static void hl_dealloc(void * hl)
{
  delete reinterpret_cast<Leap::HandList*>(hl);
}

VALUE WrapHandList(Leap::HandList * hl)
{
  return Data_Wrap_Struct(cHandList, 0, hl_dealloc, hl);
}

static VALUE count(VALUE self)
{
  Leap::HandList * list;

  Data_Get_Struct(self, Leap::HandList, list);

  return INT2NUM(list->count());
}

static VALUE empty_p(VALUE self)
{
  Leap::HandList * list;

  Data_Get_Struct(self, Leap::HandList, list);

  if (list->isEmpty()) {
    return Qtrue;
  }

  return Qfalse;
}

void Init_leap_hand()
{
  cHandList = rb_define_class_under(mLeapMotion, "HandList", rb_cObject);
  rb_define_method(cHandList, "count", (ruby_method_vararg *)count, 0);
  rb_define_method(cHandList, "empty?", (ruby_method_vararg *)empty_p, 0);
}
