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

ListCountImpl(cHandList, Leap::HandList);
ListEmptyImpl(cHandList, Leap::HandList);

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

static VALUE pointables(VALUE self)
{
  Leap::Hand * hand;
  Leap::PointableList * list;

  Data_Get_Struct(self, Leap::Hand, hand);

  list = new Leap::PointableList(hand->pointables());

  return WrapPointableList(list);
}

static VALUE pointable(VALUE self, VALUE index)
{
  Leap::Hand * hand;
  Leap::Finger * finger;

  Data_Get_Struct(self, Leap::Hand, hand);

  return MakePointable(hand->pointable(NUM2INT(index)));
}

static VALUE tools(VALUE self)
{
  Leap::Hand * hand;
  Leap::ToolList * list;

  Data_Get_Struct(self, Leap::Hand, hand);

  list = new Leap::ToolList(hand->tools());

  return WrapToolList(list);
}

static VALUE tool(VALUE self, VALUE index)
{
  Leap::Hand * hand;
  Leap::Finger * finger;

  Data_Get_Struct(self, Leap::Hand, hand);

  return WrapTool(new Leap::Tool(hand->tool(NUM2INT(index))));
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

static VALUE palm_position(VALUE self)
{
  Leap::Hand * f;

  Data_Get_Struct(self, Leap::Hand, f);

  return WrapVector(new Leap::Vector(f->palmPosition()));
}

static VALUE palm_velocity(VALUE self)
{
  Leap::Hand * f;

  Data_Get_Struct(self, Leap::Hand, f);

  return WrapVector(new Leap::Vector(f->palmVelocity()));
}

static VALUE palm_normal(VALUE self)
{
  Leap::Hand * f;

  Data_Get_Struct(self, Leap::Hand, f);

  return WrapVector(new Leap::Vector(f->palmNormal()));
}

static VALUE direction(VALUE self)
{
  Leap::Hand * f;

  Data_Get_Struct(self, Leap::Hand, f);

  return WrapVector(new Leap::Vector(f->direction()));
}

static VALUE sphere_center(VALUE self)
{
  Leap::Hand * f;

  Data_Get_Struct(self, Leap::Hand, f);

  return WrapVector(new Leap::Vector(f->sphereCenter()));
}

static VALUE sphere_radius(VALUE self)
{
  Leap::Hand * f;

  Data_Get_Struct(self, Leap::Hand, f);

  return DBL2NUM(f->sphereRadius());
}

void Init_leap_hand()
{
  cHandList = rb_define_class_under(mLeapMotion, "HandList", rb_cObject);
  ListCountDecl(cHandList, Leap::HandList);
  ListEmptyDecl(cHandList, Leap::HandList);
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
  rb_define_method(cHand, "pointables", (ruby_method_vararg *)pointables, 0);
  rb_define_method(cHand, "pointable", (ruby_method_vararg *)pointable, 1);
  rb_define_method(cHand, "tools", (ruby_method_vararg *)tools, 0);
  rb_define_method(cHand, "tool", (ruby_method_vararg *)tool, 1);
  rb_define_method(cHand, "palm_position", (ruby_method_vararg *)palm_position, 0);
  rb_define_method(cHand, "palm_velocity", (ruby_method_vararg *)palm_velocity, 0);
  rb_define_method(cHand, "palm_normal", (ruby_method_vararg *)palm_normal, 0);
  rb_define_method(cHand, "direction", (ruby_method_vararg *)direction, 0);
  rb_define_method(cHand, "sphere_center", (ruby_method_vararg *)sphere_center, 0);
  rb_define_method(cHand, "sphere_radius", (ruby_method_vararg *)sphere_radius, 0);
  rb_define_method(cHand, "to_s", (ruby_method_vararg *)hand_to_s, 0);
}
