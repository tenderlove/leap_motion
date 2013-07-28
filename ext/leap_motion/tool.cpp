#include <leap_motion.h>

VALUE cToolList;
VALUE cTool;

static void fl_dealloc(void * fl)
{
  delete reinterpret_cast<Leap::ToolList*>(fl);
}

VALUE WrapToolList(Leap::ToolList * fl)
{
  return Data_Wrap_Struct(cToolList, 0, fl_dealloc, fl);
}

static void tool_dealloc(void * f)
{
  delete reinterpret_cast<Leap::Tool*>(f);
}

VALUE WrapTool(Leap::Tool * tool)
{
  return Data_Wrap_Struct(cTool, 0, tool_dealloc, tool);
}

ListCountImpl(cToolList, Leap::ToolList);
ListEmptyImpl(cToolList, Leap::ToolList);

static VALUE leftmost(VALUE self)
{
  Leap::ToolList * list;
  Leap::Tool * tool;

  Data_Get_Struct(self, Leap::ToolList, list);

  tool = new Leap::Tool(list->leftmost());

  return WrapTool(tool);
}

static VALUE rightmost(VALUE self)
{
  Leap::ToolList * list;
  Leap::Tool * tool;

  Data_Get_Struct(self, Leap::ToolList, list);

  tool = new Leap::Tool(list->rightmost());

  return WrapTool(tool);
}

static VALUE frontmost(VALUE self)
{
  Leap::ToolList * list;
  Leap::Tool * tool;

  Data_Get_Struct(self, Leap::ToolList, list);

  tool = new Leap::Tool(list->frontmost());

  return WrapTool(tool);
}

static VALUE each(VALUE self)
{
  Leap::ToolList * list;
  int i;

  Data_Get_Struct(self, Leap::ToolList, list);

  for (i = 0; i < list->count(); i++) {
    Leap::Tool * tool;
    tool = new Leap::Tool((*list)[i]);
    rb_yield(WrapTool(tool));
  }

  return self;
}

static VALUE tool_to_s(VALUE self)
{
  Leap::Tool * tool;
  const char * string;

  Data_Get_Struct(self, Leap::Tool, tool);

  string = tool->toString().c_str();
  return rb_str_new2(string);
}

void Init_leap_tool()
{
  cToolList = rb_define_class_under(mLeapMotion, "ToolList", rb_cObject);

  ListCountDecl(cToolList, Leap::ToolList);
  ListEmptyDecl(cToolList, Leap::ToolList);

  rb_define_method(cToolList, "leftmost", (ruby_method_vararg *)leftmost, 0);
  rb_define_method(cToolList, "rightmost", (ruby_method_vararg *)rightmost, 0);
  rb_define_method(cToolList, "frontmost", (ruby_method_vararg *)frontmost, 0);
  rb_define_method(cToolList, "each", (ruby_method_vararg *)each, 0);

  cTool = rb_define_class_under(mLeapMotion, "Tool", cPointable);
  rb_define_method(cTool, "to_s", (ruby_method_vararg *)tool_to_s, 0);
}

