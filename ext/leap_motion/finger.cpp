#include <leap_motion.h>

VALUE cFingerList;
VALUE cFinger;

static void fl_dealloc(void * fl)
{
  delete reinterpret_cast<Leap::FingerList*>(fl);
}

VALUE WrapFingerList(Leap::FingerList * fl)
{
  return Data_Wrap_Struct(cFingerList, 0, fl_dealloc, fl);
}

static void finger_dealloc(void * f)
{
  delete reinterpret_cast<Leap::Finger*>(f);
}

VALUE WrapFinger(Leap::Finger * finger)
{
  return Data_Wrap_Struct(cFinger, 0, finger_dealloc, finger);
}

ListCountImpl(cFingerList, Leap::FingerList);
ListEmptyImpl(cFingerList, Leap::FingerList);

static VALUE leftmost(VALUE self)
{
  Leap::FingerList * list;
  Leap::Finger * finger;

  Data_Get_Struct(self, Leap::FingerList, list);

  finger = new Leap::Finger(list->leftmost());

  return WrapFinger(finger);
}

static VALUE rightmost(VALUE self)
{
  Leap::FingerList * list;
  Leap::Finger * finger;

  Data_Get_Struct(self, Leap::FingerList, list);

  finger = new Leap::Finger(list->rightmost());

  return WrapFinger(finger);
}

static VALUE frontmost(VALUE self)
{
  Leap::FingerList * list;
  Leap::Finger * finger;

  Data_Get_Struct(self, Leap::FingerList, list);

  finger = new Leap::Finger(list->frontmost());

  return WrapFinger(finger);
}

static VALUE each(VALUE self)
{
  Leap::FingerList * list;
  int i;

  Data_Get_Struct(self, Leap::FingerList, list);

  for (i = 0; i < list->count(); i++) {
    Leap::Finger * finger;
    finger = new Leap::Finger((*list)[i]);
    rb_yield(WrapFinger(finger));
  }

  return self;
}

static VALUE finger_to_s(VALUE self)
{
  Leap::Finger * finger;
  const char * string;

  Data_Get_Struct(self, Leap::Finger, finger);

  string = finger->toString().c_str();
  return rb_str_new2(string);
}

void Init_leap_finger()
{
  cFingerList = rb_define_class_under(mLeapMotion, "FingerList", rb_cObject);

  ListCountDecl(cFingerList, Leap::FingerList);
  ListEmptyDecl(cFingerList, Leap::FingerList);

  rb_define_method(cFingerList, "leftmost", (ruby_method_vararg *)leftmost, 0);
  rb_define_method(cFingerList, "rightmost", (ruby_method_vararg *)rightmost, 0);
  rb_define_method(cFingerList, "frontmost", (ruby_method_vararg *)frontmost, 0);
  rb_define_method(cFingerList, "each", (ruby_method_vararg *)each, 0);

  cFinger = rb_define_class_under(mLeapMotion, "Finger", cPointable);
  rb_define_method(cFinger, "to_s", (ruby_method_vararg *)finger_to_s, 0);
}
