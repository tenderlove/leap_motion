#include <leap_motion.h>

VALUE cHandList;
VALUE cHand;

static void hl_dealloc(void * hl)
{
  delete reinterpret_cast<Leap::HandList*>(hl);
}

VALUE WrapHandList(Leap::HandList * hl)
{
  return Data_Wrap_Struct(cHandList, 0, hl_dealloc, hl);
}

static void hand_dealloc(void * hl)
{
  delete reinterpret_cast<Leap::Hand*>(hl);
}

VALUE WrapHand(Leap::Hand * hand)
{
  return Data_Wrap_Struct(cHand, 0, hand_dealloc, hand);
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

static VALUE leftmost(VALUE self)
{
  Leap::HandList * list;
  Leap::Hand * hand;

  Data_Get_Struct(self, Leap::HandList, list);

  hand = new Leap::Hand(list->leftmost());

  return WrapHand(hand);
}

static VALUE rightmost(VALUE self)
{
  Leap::HandList * list;
  Leap::Hand * hand;

  Data_Get_Struct(self, Leap::HandList, list);

  hand = new Leap::Hand(list->rightmost());

  return WrapHand(hand);
}

static VALUE frontmost(VALUE self)
{
  Leap::HandList * list;
  Leap::Hand * hand;

  Data_Get_Struct(self, Leap::HandList, list);

  hand = new Leap::Hand(list->frontmost());

  return WrapHand(hand);
}

static VALUE each(VALUE self)
{
  Leap::HandList * list;
  int i;

  Data_Get_Struct(self, Leap::HandList, list);

  for (i = 0; i < list->count(); i++) {
    Leap::Hand * hand;
    hand = new Leap::Hand((*list)[i]);
    rb_yield(WrapHand(hand));
  }

  return self;
}

/** HAND **/

static VALUE valid_p(VALUE self)
{
  Leap::Hand * hand;

  Data_Get_Struct(self, Leap::Hand, hand);

  if (hand->isValid()) {
    return Qtrue;
  }

  return Qfalse;
}

static VALUE hand_eql(VALUE self, VALUE other)
{
  Leap::Hand * hand;
  Leap::Hand * other_hand;

  Data_Get_Struct(self, Leap::Hand, hand);
  Data_Get_Struct(other, Leap::Hand, other_hand);

  if (*hand == *other_hand) {
    return Qtrue;
  }

  return Qfalse;
}

static VALUE hand_id(VALUE self)
{
  Leap::Hand * hand;

  Data_Get_Struct(self, Leap::Hand, hand);

  return INT2NUM(hand->id());
}

static VALUE fingers(VALUE self)
{
  Leap::Hand * hand;
  Leap::FingerList * list;

  Data_Get_Struct(self, Leap::Hand, hand);

  list = new Leap::FingerList(hand->fingers());

  return WrapFingerList(list);
}

static VALUE finger(VALUE self, VALUE index)
{
  Leap::Hand * hand;
  Leap::Finger * finger;

  Data_Get_Struct(self, Leap::Hand, hand);

  finger = new Leap::Finger(hand->finger(NUM2INT(index)));

  return WrapFinger(finger);
}

static VALUE hand_to_s(VALUE self)
{
  Leap::Hand * f;
  const char * string;

  Data_Get_Struct(self, Leap::Hand, f);

  string = f->toString().c_str();
  return rb_str_new2(string);
}

void Init_leap_hand()
{
  cHandList = rb_define_class_under(mLeapMotion, "HandList", rb_cObject);
  rb_define_method(cHandList, "count", (ruby_method_vararg *)count, 0);
  rb_define_method(cHandList, "empty?", (ruby_method_vararg *)empty_p, 0);
  rb_define_method(cHandList, "leftmost", (ruby_method_vararg *)leftmost, 0);
  rb_define_method(cHandList, "rightmost", (ruby_method_vararg *)rightmost, 0);
  rb_define_method(cHandList, "frontmost", (ruby_method_vararg *)frontmost, 0);
  rb_define_method(cHandList, "each", (ruby_method_vararg *)each, 0);

  cHand = rb_define_class_under(mLeapMotion, "Hand", rb_cObject);
  rb_define_method(cHand, "valid?", (ruby_method_vararg *)valid_p, 0);
  rb_define_method(cHand, "==", (ruby_method_vararg *)hand_eql, 1);
  rb_define_method(cHand, "id", (ruby_method_vararg *)hand_id, 0);
  rb_define_method(cHand, "fingers", (ruby_method_vararg *)fingers, 0);
  rb_define_method(cHand, "finger", (ruby_method_vararg *)finger, 1);
  rb_define_method(cHand, "to_s", (ruby_method_vararg *)hand_to_s, 0);
}
