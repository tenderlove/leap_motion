#include <leap_motion.h>

VALUE mLeapMotion;
VALUE cController;
VALUE cListener;
VALUE cFrame;

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
    virtual void onFrame(const Leap::Controller&);
    virtual void onFocusGained(const Leap::Controller&);
    virtual void onFocusLost(const Leap::Controller&);

  protected:
    VALUE listener;
};

RubyListener::RubyListener()
: Leap::Listener()
{
  listener = Qnil;
}

RubyListener::~RubyListener() {
  listener = Qnil;
}

void RubyListener::setRubyListener(VALUE l) { listener = l; }
VALUE RubyListener::getRubyListener() { return listener; }

void RubyListener::onInit(const Leap::Controller& controller) {
  rb_funcall(listener, on_init, 0);
}

void RubyListener::onConnect(const Leap::Controller& controller) {
  rb_funcall(listener, on_connect, 0);
}

void RubyListener::onDisconnect(const Leap::Controller& controller) {
  rb_funcall(listener, on_disconnect, 0);
}

void RubyListener::onFrame(const Leap::Controller& controller) {
  rb_funcall(listener, on_frame, 0);
}

void RubyListener::onFocusGained(const Leap::Controller& controller) {
  rb_funcall(listener, on_focus_gained, 0);
}

void RubyListener::onFocusLost(const Leap::Controller& controller) {
  rb_funcall(listener, on_focus_lost, 0);
}

class RubyController
{
  public:
    RubyController();
    ~RubyController();
    Leap::Controller * getController();
    RubyListener * getListener();

  protected:
    Leap::Controller * controller;
    RubyListener * listener;
};

RubyController::RubyController() {
  controller = new Leap::Controller;
  listener = new RubyListener;
}

RubyController::~RubyController() {
  delete controller;
  delete listener;
}

Leap::Controller * RubyController::getController() { return controller; }
RubyListener * RubyController::getListener() { return listener; }

static void dealloc(void * controller)
{
  delete reinterpret_cast<RubyController*>(controller);
}

static VALUE allocate(VALUE klass)
{
  VALUE self;
  RubyController * controller = new RubyController();
  self = Data_Wrap_Struct(klass, 0, dealloc, controller);
  controller->getListener()->setRubyListener(self);

  return self;
}

static VALUE connected_p(VALUE self)
{
  RubyController * controller;

  Data_Get_Struct(self, RubyController, controller);

  if (true == controller->getController()->isConnected()) {
    return Qtrue;
  }

  return Qfalse;
}

static VALUE has_focus_p(VALUE self)
{
  RubyController * controller;

  Data_Get_Struct(self, RubyController, controller);

  if (true == controller->getController()->hasFocus()) {
    return Qtrue;
  }

  return Qfalse;
}

static VALUE policy_flags(VALUE self)
{
  RubyController * controller;

  Data_Get_Struct(self, RubyController, controller);

  return INT2NUM(controller->getController()->policyFlags());
}

static VALUE frame(VALUE self)
{
  RubyController * controller;
  Leap::Frame f;
  Leap::Frame * copy;

  Data_Get_Struct(self, RubyController, controller);

  f = controller->getController()->frame(0);
  copy = new Leap::Frame(f);

  return Data_Wrap_Struct(cFrame, 0, 0, copy);
}

static VALUE listen(VALUE self)
{
  RubyController * controller;
  RubyListener * listener;

  Data_Get_Struct(self, RubyController, controller);

  listener = controller->getListener();

  if (controller->getController()->addListener(*listener)) {
    return Qtrue;
  }

  return Qfalse;
}

static VALUE unlisten(VALUE self)
{
  RubyController * controller;
  RubyListener * listener;

  Data_Get_Struct(self, RubyController, controller);

  listener = controller->getListener();

  if (controller->getController()->removeListener(*listener)) {
    return Qtrue;
  }

  return Qfalse;
}

static void dealloc_listener(void * listener)
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

static VALUE valid_p(VALUE self)
{
  Leap::Frame * f;

  Data_Get_Struct(self, Leap::Frame, f);

  if (true == f->isValid()) {
    return Qtrue;
  }

  return Qfalse;
}

static VALUE frame_id(VALUE self)
{
  Leap::Frame * f;

  Data_Get_Struct(self, Leap::Frame, f);

  return INT2NUM(f->id());
}

static VALUE frame_timestamp(VALUE self)
{
  Leap::Frame * f;

  Data_Get_Struct(self, Leap::Frame, f);

  return INT2NUM(f->timestamp());
}

static VALUE frame_invalid(VALUE self)
{
  Leap::Frame * f;
  Leap::Frame * invalid;

  Data_Get_Struct(self, Leap::Frame, f);

  invalid = new Leap::Frame(f->invalid());

  return Data_Wrap_Struct(cFrame, 0, 0, invalid);
}

static VALUE frame_to_s(VALUE self)
{
  Leap::Frame * f;
  const char * string;

  Data_Get_Struct(self, Leap::Frame, f);

  string = f->toString().c_str();
  return rb_str_new2(string);
}

void Init_leap_motion()
{
  mLeapMotion = rb_define_module("LeapMotion");
  cController = rb_define_class_under(mLeapMotion, "Controller", rb_cObject);
  cListener = rb_define_class_under(mLeapMotion, "Listener", rb_cObject);
  cFrame = rb_define_class_under(mLeapMotion, "Frame", rb_cObject);

  rb_define_alloc_func(cController, allocate);
  rb_define_method(cController, "connected?", (ruby_method_vararg *)connected_p, 0);
  rb_define_method(cController, "has_focus?", (ruby_method_vararg *)has_focus_p, 0);
  rb_define_method(cController, "policy_flags", (ruby_method_vararg *)policy_flags, 0);
  rb_define_method(cController, "frame", (ruby_method_vararg *)frame, 0);
  rb_define_private_method(cController, "listen!", (ruby_method_vararg *)listen, 0);
  rb_define_private_method(cController, "unlisten!", (ruby_method_vararg *)unlisten, 0);

  rb_define_alloc_func(cListener, allocate_listener);

  rb_define_method(cFrame, "valid?", (ruby_method_vararg *)valid_p, 0);
  rb_define_method(cFrame, "id", (ruby_method_vararg *)frame_id, 0);
  rb_define_method(cFrame, "timestamp", (ruby_method_vararg *)frame_timestamp, 0);
  rb_define_method(cFrame, "invalid", (ruby_method_vararg *)frame_invalid, 0);
  rb_define_method(cFrame, "to_s", (ruby_method_vararg *)frame_to_s, 0);

  on_init = rb_intern("on_init");
  on_connect = rb_intern("on_connect");
  on_disconnect = rb_intern("on_disconnect");
  on_exit = rb_intern("on_exit");
  on_frame = rb_intern("on_frame");
  on_focus_gained = rb_intern("on_focus_gained");
  on_focus_lost = rb_intern("on_focus_lost");
}
