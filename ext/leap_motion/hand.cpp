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

void Init_leap_hand()
{
  cHandList = rb_define_class_under(mLeapMotion, "HandList", rb_cObject);
  rb_define_method(cHandList, "count", (ruby_method_vararg *)count, 0);
  rb_define_method(cHandList, "empty?", (ruby_method_vararg *)empty_p, 0);
  rb_define_method(cHandList, "leftmost", (ruby_method_vararg *)leftmost, 0);
  rb_define_method(cHandList, "rightmost", (ruby_method_vararg *)rightmost, 0);
  rb_define_method(cHandList, "frontmost", (ruby_method_vararg *)frontmost, 0);

  cHand = rb_define_class_under(mLeapMotion, "Hand", rb_cObject);
  rb_define_method(cHand, "valid?", (ruby_method_vararg *)valid_p, 0);
  rb_define_method(cHand, "==", (ruby_method_vararg *)hand_eql, 1);
}
