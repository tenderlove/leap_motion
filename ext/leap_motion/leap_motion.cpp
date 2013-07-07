#include <leap_motion.h>

VALUE mLeapMotion;
VALUE cController;
VALUE cListener;

typedef VALUE (ruby_method_vararg)(...);

class RubyListener : public Leap::Listener {
  public:
    RubyListener();
    VALUE getRubyListener();
    void setRubyListener(VALUE l);

  protected:
    VALUE listener;
};

RubyListener::RubyListener() {
  listener = Qnil;
}

void RubyListener::setRubyListener(VALUE l) { listener = l; }
VALUE RubyListener::getRubyListener() { return listener; }

static VALUE dealloc(void * controller)
{
  delete reinterpret_cast<Leap::Controller*>(controller);
}

static VALUE allocate(VALUE klass)
{
  Leap::Controller * controller = new Leap::Controller();
  return Data_Wrap_Struct(klass, 0, dealloc, controller);
}

VALUE add_listener(VALUE self, VALUE _listener)
{
  Leap::Controller * controller;
  RubyListener * listener;

  Data_Get_Struct(self, Leap::Controller, controller);
  Data_Get_Struct(_listener, RubyListener, listener);

  controller->addListener(*listener);

  return _listener;
}

static VALUE dealloc_listener(void * controller)
{
  delete reinterpret_cast<RubyListener*>(controller);
}

static VALUE allocate_listener(VALUE klass)
{
  RubyListener * listener = new RubyListener();
  VALUE rbobj = Data_Wrap_Struct(klass, 0, dealloc_listener, listener);
  listener->setRubyListener(rbobj);
  return rbobj;
}

void Init_leap_motion()
{
  mLeapMotion = rb_define_module("LeapMotion");
  cController = rb_define_class_under(mLeapMotion, "Controller", rb_cObject);
  cListener = rb_define_class_under(mLeapMotion, "Listener", rb_cObject);

  rb_define_alloc_func(cController, allocate);
  rb_define_method(cController, "add_listener", (ruby_method_vararg *)add_listener, 1);

  rb_define_alloc_func(cListener, allocate_listener);
}
