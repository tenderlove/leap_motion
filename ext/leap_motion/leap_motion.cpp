#include <leap_motion.h>
#include <ruby.h>

VALUE mLeapMotion;
VALUE cController;
VALUE cListener;

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
  rb_define_alloc_func(cListener, allocate_listener);
}
