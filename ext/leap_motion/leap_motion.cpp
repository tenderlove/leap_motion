#include <leap_motion.h>

VALUE mLeapMotion;
VALUE cController;
VALUE cListener;
ID on_init;
ID on_connect;
ID on_disconnect;
ID on_exit;
ID on_frame;
ID on_focus_gained;
ID on_focus_lost;

typedef VALUE (ruby_method_vararg)(...);

class RubyListener : public Leap::Listener {
  public:
    RubyListener();
    ~RubyListener();
    VALUE getRubyListener();
    void setRubyListener(VALUE l);

    virtual void onInit(const Leap::Controller& controller);
    virtual void onConnect(const Leap::Controller& controller);
    virtual void onDisconnect(const Leap::Controller& controller);
    virtual void onExit(const Leap::Controller&);
    virtual void onFrame(const Leap::Controller&);
    virtual void onFocusGained(const Leap::Controller&);
    virtual void onFocusLost(const Leap::Controller&);

  protected:
    VALUE listener;
};

RubyListener::RubyListener() {
  listener = Qnil;
}

RubyListener::~RubyListener() {
  listener = Qnil;
}

void RubyListener::setRubyListener(VALUE l) { listener = l; }
VALUE RubyListener::getRubyListener() { return listener; }

void RubyListener::onInit(const Leap::Controller& controller) {
  rb_funcall(listener, on_init, 1, rb_iv_get(listener, "@controller"));
}

void RubyListener::onConnect(const Leap::Controller& controller) {
  rb_funcall(listener, on_connect, 1, rb_iv_get(listener, "@controller"));
}

void RubyListener::onDisconnect(const Leap::Controller& controller) {
  rb_funcall(listener, on_disconnect, 1, rb_iv_get(listener, "@controller"));
}

void RubyListener::onExit(const Leap::Controller& controller) {
  if (!NIL_P(listener)) {
    rb_funcall(listener, on_exit, 1, rb_iv_get(listener, "@controller"));
  }
}

void RubyListener::onFrame(const Leap::Controller& controller) {
  rb_funcall(listener, on_frame, 1, rb_iv_get(listener, "@controller"));
}

void RubyListener::onFocusGained(const Leap::Controller& controller) {
  rb_funcall(listener, on_focus_gained, 1, rb_iv_get(listener, "@controller"));
}

void RubyListener::onFocusLost(const Leap::Controller& controller) {
  rb_funcall(listener, on_focus_lost, 1, rb_iv_get(listener, "@controller"));
}

static VALUE dealloc(void * controller)
{
  delete reinterpret_cast<Leap::Controller*>(controller);
}

static VALUE allocate(VALUE klass)
{
  Leap::Controller * controller = new Leap::Controller();
  return Data_Wrap_Struct(klass, 0, dealloc, controller);
}

static VALUE add_listener(VALUE self, VALUE _listener)
{
  Leap::Controller * controller;
  RubyListener * listener;

  Data_Get_Struct(self, Leap::Controller, controller);
  Data_Get_Struct(_listener, RubyListener, listener);

  rb_iv_set(_listener, "@controller", self);

  if (true == controller->addListener(*listener)) {
    return Qtrue;
  }

  return Qfalse;
}

static VALUE remove_listener(VALUE self, VALUE _listener)
{
  Leap::Controller * controller;
  RubyListener * listener;

  Data_Get_Struct(self, Leap::Controller, controller);
  Data_Get_Struct(_listener, RubyListener, listener);

  if (true == controller->removeListener(*listener)) {
    return Qtrue;
  }

  rb_iv_set(_listener, "@controller", Qnil);

  return Qfalse;
}

static VALUE dealloc_listener(void * listener)
{
  delete reinterpret_cast<RubyListener*>(listener);
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
  rb_define_method(cController, "remove_listener", (ruby_method_vararg *)remove_listener, 1);

  rb_define_alloc_func(cListener, allocate_listener);

  on_init = rb_intern("on_init");
  on_connect = rb_intern("on_connect");
  on_disconnect = rb_intern("on_disconnect");
  on_exit = rb_intern("on_exit");
  on_frame = rb_intern("on_frame");
  on_focus_gained = rb_intern("on_focus_gained");
  on_focus_lost = rb_intern("on_focus_lost");
}
