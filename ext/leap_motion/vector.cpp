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

static VALUE eql(VALUE self, VALUE other)
{
  Leap::Vector * vector;
  Leap::Vector * other_vector;

  Data_Get_Struct(self, Leap::Vector, vector);
  Data_Get_Struct(other, Leap::Vector, other_vector);

  if (*vector == *other_vector) {
    return Qtrue;
  }

  return Qfalse;
}

static VALUE magnitude(VALUE self)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  return DBL2NUM(vector->magnitude());
}

static VALUE magnitude_squared(VALUE self)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  return DBL2NUM(vector->magnitudeSquared());
}

static VALUE distance_to(VALUE self, VALUE other)
{
  Leap::Vector * vector;
  Leap::Vector * other_vector;

  Data_Get_Struct(self, Leap::Vector, vector);
  Data_Get_Struct(other, Leap::Vector, other_vector);

  return DBL2NUM(vector->distanceTo(*other_vector));
}

static VALUE angle_to(VALUE self, VALUE other)
{
  Leap::Vector * vector;
  Leap::Vector * other_vector;

  Data_Get_Struct(self, Leap::Vector, vector);
  Data_Get_Struct(other, Leap::Vector, other_vector);

  return DBL2NUM(vector->angleTo(*other_vector));
}

static VALUE pitch(VALUE self)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  return DBL2NUM(vector->pitch());
}

static VALUE yaw(VALUE self)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  return DBL2NUM(vector->yaw());
}

static VALUE roll(VALUE self)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  return DBL2NUM(vector->roll());
}

static VALUE dot(VALUE self, VALUE other)
{
  Leap::Vector * vector;
  Leap::Vector * other_vector;

  Data_Get_Struct(self, Leap::Vector, vector);
  Data_Get_Struct(other, Leap::Vector, other_vector);

  return DBL2NUM(vector->dot(*other_vector));
}

static VALUE cross(VALUE self, VALUE other)
{
  Leap::Vector * vector;
  Leap::Vector * other_vector;
  Leap::Vector * new_vector;

  Data_Get_Struct(self, Leap::Vector, vector);
  Data_Get_Struct(other, Leap::Vector, other_vector);

  new_vector = new Leap::Vector(vector->cross(*other_vector));

  return WrapVector(new_vector);
}

static VALUE normalized(VALUE self)
{
  Leap::Vector * vector;
  Leap::Vector * new_vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  new_vector = new Leap::Vector(vector->normalized());

  return WrapVector(new_vector);
}

static VALUE negate(VALUE self)
{
  Leap::Vector * vector;
  Leap::Vector * new_vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  new_vector = new Leap::Vector(-(*vector));

  return WrapVector(new_vector);
}

static VALUE add(VALUE self, VALUE other)
{
  Leap::Vector * vector;
  Leap::Vector * other_vector;
  Leap::Vector * new_vector;

  Data_Get_Struct(self, Leap::Vector, vector);
  Data_Get_Struct(other, Leap::Vector, other_vector);

  new_vector = new Leap::Vector(*vector + *other_vector);

  return WrapVector(new_vector);
}

static VALUE subtract(VALUE self, VALUE other)
{
  Leap::Vector * vector;
  Leap::Vector * other_vector;
  Leap::Vector * new_vector;

  Data_Get_Struct(self, Leap::Vector, vector);
  Data_Get_Struct(other, Leap::Vector, other_vector);

  new_vector = new Leap::Vector(*vector - *other_vector);

  return WrapVector(new_vector);
}

static VALUE multiply(VALUE self, VALUE scalar)
{
  Leap::Vector * vector;
  Leap::Vector * new_vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  new_vector = new Leap::Vector(*vector * NUM2DBL(scalar));

  return WrapVector(new_vector);
}

static VALUE divide(VALUE self, VALUE scalar)
{
  Leap::Vector * vector;
  Leap::Vector * new_vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  new_vector = new Leap::Vector(*vector / NUM2DBL(scalar));

  return WrapVector(new_vector);
}

static VALUE valid_p(VALUE self, VALUE scalar)
{
  Leap::Vector * vector;

  Data_Get_Struct(self, Leap::Vector, vector);

  if (vector->isValid())
    return Qtrue;

  return Qfalse;
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
  rb_define_method(cVector, "==", (ruby_method_vararg *)eql, 1);
  rb_define_method(cVector, "magnitude", (ruby_method_vararg *)magnitude, 0);
  rb_define_method(cVector, "magnitude_squared", (ruby_method_vararg *)magnitude_squared, 0);
  rb_define_method(cVector, "distance_to", (ruby_method_vararg *)distance_to, 1);
  rb_define_method(cVector, "angle_to", (ruby_method_vararg *)angle_to, 1);
  rb_define_method(cVector, "pitch", (ruby_method_vararg *)pitch, 0);
  rb_define_method(cVector, "yaw", (ruby_method_vararg *)yaw, 0);
  rb_define_method(cVector, "roll", (ruby_method_vararg *)roll, 0);
  rb_define_method(cVector, "dot", (ruby_method_vararg *)dot, 1);
  rb_define_method(cVector, "cross", (ruby_method_vararg *)cross, 1);
  rb_define_method(cVector, "normalized", (ruby_method_vararg *)normalized, 0);
  rb_define_method(cVector, "-@", (ruby_method_vararg *)negate, 0);
  rb_define_method(cVector, "+", (ruby_method_vararg *)add, 1);
  rb_define_method(cVector, "-", (ruby_method_vararg *)subtract, 1);
  rb_define_method(cVector, "*", (ruby_method_vararg *)multiply, 1);
  rb_define_method(cVector, "/", (ruby_method_vararg *)divide, 1);
  rb_define_method(cVector, "valid?", (ruby_method_vararg *)valid_p, 0);
}
