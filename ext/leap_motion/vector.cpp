#include <leap_motion.h>

VALUE cVector;

static void dealloc(void * v)
{
  delete reinterpret_cast<Leap::Vector*>(v);
}

VALUE WrapVector(Leap::Vector * v)
{
  return Data_Wrap_Struct(cVector, 0, dealloc, v);
}

static VALUE allocate(VALUE klass)
{
  Leap::Vector * v = new Leap::Vector();

  return WrapVector(v);
}

static VALUE x(VALUE self)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  return DBL2NUM(vector->x);
}

static VALUE y(VALUE self)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  return DBL2NUM(vector->y);
}

static VALUE z(VALUE self)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  return DBL2NUM(vector->z);
}

static VALUE to_s(VALUE self)
{
  Leap::Vector * vector;
  const char * string;

  Data_Get_Struct(self, Leap::Vector, vector);

  string = vector->toString().c_str();
  return rb_str_new2(string);
}

static VALUE init(VALUE self, VALUE x, VALUE y, VALUE z)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  vector->x = NUM2DBL(x);
  vector->y = NUM2DBL(y);
  vector->z = NUM2DBL(z);

  return self;
}

void Init_leap_vector()
{
  cVector = rb_define_class_under(mLeapMotion, "Vector", rb_cObject);

  rb_define_alloc_func(cVector, allocate);

  rb_define_method(cVector, "initialize", (ruby_method_vararg *)init, 3);

  rb_define_method(cVector, "x", (ruby_method_vararg *)x, 0);
  rb_define_method(cVector, "y", (ruby_method_vararg *)y, 0);
  rb_define_method(cVector, "z", (ruby_method_vararg *)z, 0);
  rb_define_method(cVector, "to_s", (ruby_method_vararg *)to_s, 0);
}
