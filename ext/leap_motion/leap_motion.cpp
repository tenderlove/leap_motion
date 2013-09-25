#include <leap_motion.h>

VALUE mLeapMotion;
VALUE cController;

ID on_init;
ID on_connect;
ID on_disconnect;
ID on_exit;
ID on_frame;
ID on_focus_gained;
ID on_focus_lost;

class RubyListener : public Leap::Listener {
  public:
    RubyListener();
    ~RubyListener();
    VALUE getRubyListener();
    void setRubyListener(VALUE l);
    int * getInitPipes();
    int * getConnectPipes();
    int * getDisconnectPipes();
    int * getFramePipes();
    int * getFocusGainedPipes();
    int * getFocusLostPipes();

    virtual void onInit(const Leap::Controller& controller);
    virtual void onConnect(const Leap::Controller& controller);
    virtual void onDisconnect(const Leap::Controller& controller);
    virtual void onFrame(const Leap::Controller&);
    virtual void onFocusGained(const Leap::Controller&);
    virtual void onFocusLost(const Leap::Controller&);

  protected:
    VALUE listener;
    int initPipes[2];
    int connectPipes[2];
    int disconnectPipes[2];
    int framePipes[2];
    int focusGainedPipes[2];
    int focusLostPipes[2];
};

RubyListener::RubyListener()
: Leap::Listener()
{
  listener = Qnil;
  rb_pipe(framePipes);
  rb_pipe(initPipes);
  rb_pipe(connectPipes);
  rb_pipe(disconnectPipes);
  rb_pipe(framePipes);
  rb_pipe(focusGainedPipes);
  rb_pipe(focusLostPipes);
}

RubyListener::~RubyListener() {
  listener = Qnil;
}

int * RubyListener::getInitPipes() { return initPipes; }
int * RubyListener::getConnectPipes() { return connectPipes; }
int * RubyListener::getDisconnectPipes() { return disconnectPipes; }
int * RubyListener::getFramePipes() { return framePipes; }
int * RubyListener::getFocusGainedPipes() { return focusGainedPipes; }
int * RubyListener::getFocusLostPipes() { return focusLostPipes; }

void RubyListener::setRubyListener(VALUE l) { listener = l; }
VALUE RubyListener::getRubyListener() { return listener; }

void RubyListener::onInit(const Leap::Controller& controller) {
  write(initPipes[1], "i", 1);
}

void RubyListener::onConnect(const Leap::Controller& controller) {
  write(connectPipes[1], "c", 1);
}

void RubyListener::onDisconnect(const Leap::Controller& controller) {
  write(disconnectPipes[1], "d", 1);
}

void RubyListener::onFrame(const Leap::Controller& controller) {
  write(framePipes[1], "f", 1);
}

void RubyListener::onFocusGained(const Leap::Controller& controller) {
  write(focusGainedPipes[1], "f", 1);
}

void RubyListener::onFocusLost(const Leap::Controller& controller) {
  write(focusLostPipes[1], "l", 1);
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
  controller->enableGesture(Leap::Gesture::TYPE_CIRCLE);
  controller->enableGesture(Leap::Gesture::TYPE_KEY_TAP);
  controller->enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
  controller->enableGesture(Leap::Gesture::TYPE_SWIPE);
  listener = new RubyListener;
}

RubyController::~RubyController() {
  controller->removeListener(*listener);
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
  Leap::Frame * copy;

  Data_Get_Struct(self, RubyController, controller);

  copy = new Leap::Frame(controller->getController()->frame(0));

  return WrapFrame(copy);
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

static VALUE init_fd(VALUE self) {
  RubyController * controller;
  Data_Get_Struct(self, RubyController, controller);
  return INT2NUM(controller->getListener()->getInitPipes()[0]);
}

static VALUE connect_fd(VALUE self) {
  RubyController * controller;
  Data_Get_Struct(self, RubyController, controller);
  return INT2NUM(controller->getListener()->getConnectPipes()[0]);
}

static VALUE disconnect_fd(VALUE self) {
  RubyController * controller;
  Data_Get_Struct(self, RubyController, controller);
  return INT2NUM(controller->getListener()->getDisconnectPipes()[0]);
}

static VALUE frame_fd(VALUE self) {
  RubyController * controller;
  Data_Get_Struct(self, RubyController, controller);
  return INT2NUM(controller->getListener()->getFramePipes()[0]);
}

static VALUE focus_gained_fd(VALUE self) {
  RubyController * controller;
  Data_Get_Struct(self, RubyController, controller);
  return INT2NUM(controller->getListener()->getFocusGainedPipes()[0]);
}

static VALUE focus_lost_fd(VALUE self) {
  RubyController * controller;
  Data_Get_Struct(self, RubyController, controller);
  return INT2NUM(controller->getListener()->getFocusLostPipes()[0]);
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

void Init_leap_motion()
{
  mLeapMotion = rb_define_module("LeapMotion");
  cController = rb_define_class_under(mLeapMotion, "Controller", rb_cObject);

  rb_define_alloc_func(cController, allocate);
  rb_define_method(cController, "connected?", (ruby_method_vararg *)connected_p, 0);
  rb_define_method(cController, "has_focus?", (ruby_method_vararg *)has_focus_p, 0);
  rb_define_method(cController, "policy_flags", (ruby_method_vararg *)policy_flags, 0);
  rb_define_method(cController, "frame", (ruby_method_vararg *)frame, 0);
  rb_define_private_method(cController, "listen!", (ruby_method_vararg *)listen, 0);
  rb_define_private_method(cController, "unlisten!", (ruby_method_vararg *)unlisten, 0);
  rb_define_private_method(cController, "init_fd", (ruby_method_vararg *)init_fd, 0);
  rb_define_private_method(cController, "connect_fd", (ruby_method_vararg *)connect_fd, 0);
  rb_define_private_method(cController, "disconnect_fd", (ruby_method_vararg *)disconnect_fd, 0);
  rb_define_private_method(cController, "frame_fd", (ruby_method_vararg *)frame_fd, 0);
  rb_define_private_method(cController, "focus_gained_fd", (ruby_method_vararg *)focus_gained_fd, 0);
  rb_define_private_method(cController, "focus_lost_fd", (ruby_method_vararg *)focus_lost_fd, 0);

  Init_leap_vector();
  Init_leap_frame();
  Init_leap_hand();
  Init_leap_pointable();
  Init_leap_finger();
  Init_leap_gesture();
  Init_leap_tool();

  on_init = rb_intern("on_init");
  on_connect = rb_intern("on_connect");
  on_disconnect = rb_intern("on_disconnect");
  on_exit = rb_intern("on_exit");
  on_frame = rb_intern("on_frame");
  on_focus_gained = rb_intern("on_focus_gained");
  on_focus_lost = rb_intern("on_focus_lost");
}
