#include <leap_motion.h>

VALUE cPointableList;
VALUE cPointable;

static void pointable_dealloc(void * f)
{
  delete reinterpret_cast<Leap::Pointable*>(f);
}

VALUE WrapPointable(Leap::Pointable * pointer)
{
  return Data_Wrap_Struct(cPointable, 0, pointable_dealloc, pointer);
}

static void l_dealloc(void * l)
{
  delete reinterpret_cast<Leap::PointableList*>(l);
}

VALUE WrapPointableList(Leap::PointableList * l)
{
  return Data_Wrap_Struct(cPointableList, 0, l_dealloc, l);
}

ListCountImpl(cPointableList, Leap::PointableList);
ListEmptyImpl(cPointableList, Leap::PointableList);

VALUE MakePointable(Leap::Pointable pointable)
{
  if (pointable.isFinger())
    return WrapFinger(new Leap::Finger(pointable));

  if (pointable.isTool())
    rb_raise(rb_eRuntimeError, "fuu");

  return WrapPointable(new Leap::Pointable(pointable));
}

static VALUE leftmost(VALUE self)
{
  Leap::PointableList * list;
  Leap::Pointable pointable;

  Data_Get_Struct(self, Leap::PointableList, list);

  return MakePointable(list->leftmost());
}

static VALUE rightmost(VALUE self)
{
  Leap::PointableList * list;
  Leap::Pointable pointable;

  Data_Get_Struct(self, Leap::PointableList, list);

  return MakePointable(list->rightmost());
}

static VALUE frontmost(VALUE self)
{
  Leap::PointableList * list;
  Leap::Pointable pointable;

  Data_Get_Struct(self, Leap::PointableList, list);

  return MakePointable(list->frontmost());
}

static VALUE each(VALUE self)
{
  Leap::PointableList * list;
  int i;

  Data_Get_Struct(self, Leap::PointableList, list);

  for (i = 0; i < list->count(); i++) {
    rb_yield(MakePointable((*list)[i]));
  }

  return self;
}
static VALUE pointable_id(VALUE self)
{
  Leap::Pointable * pointer;
  const char * string;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  return INT2NUM(pointer->id());
}

static VALUE hand(VALUE self)
{
  Leap::Pointable * pointer;
  Leap::Hand * hand;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  hand = new Leap::Hand(pointer->hand());

  return WrapHand(hand);
}

static VALUE tip_position(VALUE self)
{
  Leap::Pointable * pointer;
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  vector = new Leap::Vector(pointer->tipPosition());

  return WrapVector(vector);
}

static VALUE tip_velocity(VALUE self)
{
  Leap::Pointable * pointer;
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  vector = new Leap::Vector(pointer->tipVelocity());

  return WrapVector(vector);
}

static VALUE direction(VALUE self)
{
  Leap::Pointable * pointer;
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  vector = new Leap::Vector(pointer->direction());

  return WrapVector(vector);
}

static VALUE width(VALUE self)
{
  Leap::Pointable * pointer;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  return DBL2NUM(pointer->width());
}

static VALUE length(VALUE self)
{
  Leap::Pointable * pointer;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  return DBL2NUM(pointer->length());
}

static VALUE finger_p(VALUE self)
{
  Leap::Pointable * pointer;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  if (pointer->isFinger())
    return Qtrue;

  return Qfalse;
}

static VALUE tool_p(VALUE self)
{
  Leap::Pointable * pointer;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  if (pointer->isTool())
    return Qtrue;

  return Qfalse;
}

static VALUE valid_p(VALUE self)
{
  Leap::Pointable * pointer;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  if (pointer->isValid())
    return Qtrue;

  return Qfalse;
}

static VALUE touch_zone(VALUE self)
{
  Leap::Pointable * pointer;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  switch(pointer->touchZone()) {
    case(Leap::Pointable::ZONE_NONE):
      return ID2SYM(rb_intern("none"));
    case(Leap::Pointable::ZONE_HOVERING):
      return ID2SYM(rb_intern("hovering"));
    case(Leap::Pointable::ZONE_TOUCHING):
      return ID2SYM(rb_intern("touching"));
  };

  return Qfalse;
}

static VALUE touch_distance(VALUE self)
{
  Leap::Pointable * pointer;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  return DBL2NUM(pointer->touchDistance());
}

static VALUE stabilized_tip_position(VALUE self)
{
  Leap::Pointable * pointer;
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Pointable, pointer);

  vector = new Leap::Vector(pointer->stabilizedTipPosition());

  return WrapVector(vector);
}

static VALUE pointer_eql(VALUE self, VALUE other)
{
  Leap::Pointable * pointer;
  Leap::Pointable * other_pointer;

  Data_Get_Struct(self, Leap::Pointable, pointer);
  Data_Get_Struct(other, Leap::Pointable, other_pointer);

  if (*pointer == *other_pointer) {
    return Qtrue;
  }
  return Qfalse;
}

void Init_leap_pointable()
{
  cPointable = rb_define_class_under(mLeapMotion, "Pointable", rb_cObject);
  cPointableList = rb_define_class_under(mLeapMotion, "PointableList", rb_cObject);

  ListCountDecl(cPointableList, Leap::PointableList);
  ListEmptyDecl(cPointableList, Leap::PointableList);

  rb_define_method(cPointableList, "leftmost", (ruby_method_vararg *)leftmost, 0);
  rb_define_method(cPointableList, "rightmost", (ruby_method_vararg *)rightmost, 0);
  rb_define_method(cPointableList, "frontmost", (ruby_method_vararg *)frontmost, 0);
  rb_define_method(cPointableList, "each", (ruby_method_vararg *)each, 0);

  rb_define_method(cPointable, "id", (ruby_method_vararg *)pointable_id, 0);
  rb_define_method(cPointable, "hand", (ruby_method_vararg *)hand, 0);
  rb_define_method(cPointable, "tip_position", (ruby_method_vararg *)tip_position, 0);
  rb_define_method(cPointable, "tip_velocity", (ruby_method_vararg *)tip_velocity, 0);
  rb_define_method(cPointable, "direction", (ruby_method_vararg *)direction, 0);
  rb_define_method(cPointable, "width", (ruby_method_vararg *)width, 0);
  rb_define_method(cPointable, "length", (ruby_method_vararg *)length, 0);
  rb_define_method(cPointable, "finger?", (ruby_method_vararg *)finger_p, 0);
  rb_define_method(cPointable, "tool?", (ruby_method_vararg *)tool_p, 0);
  rb_define_method(cPointable, "valid?", (ruby_method_vararg *)valid_p, 0);
  rb_define_method(cPointable, "touch_zone", (ruby_method_vararg *)touch_zone, 0);
  rb_define_method(cPointable, "touch_distance", (ruby_method_vararg *)touch_distance, 0);
  rb_define_method(cPointable, "stabilized_tip_position", (ruby_method_vararg *)stabilized_tip_position, 0);
  rb_define_method(cPointable, "==", (ruby_method_vararg *)pointer_eql, 1);
}
