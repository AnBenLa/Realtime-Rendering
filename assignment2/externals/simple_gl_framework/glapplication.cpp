// header i/f
#include "glapplication.hpp"

// header, system
#include <functional>

// header, project

///////////////////////////////////////////////////////////////////////////////
glapplication::glapplication(int argc, char* argv[])
{
  glwindow::init( 800, 800, 100, 100, argc, argv,
                  std::bind(&glapplication::draw, this), 
                  std::bind(&glapplication::mouse_press, this),
                  std::bind(&glapplication::mouse_release, this) );
}

///////////////////////////////////////////////////////////////////////////////
glapplication::~glapplication()
{}

///////////////////////////////////////////////////////////////////////////////
glwindow& 
glapplication::window ()
{
  return glwindow::instance();
}

///////////////////////////////////////////////////////////////////////////////
void 
glapplication::set_resolution ( int width, int height )
{
  glwindow::instance().reshape_window( width, height );
}

///////////////////////////////////////////////////////////////////////////////
void 
glapplication::set_range ( float left, float right, float top, float bottom )
{
  window().ortho (left, right, bottom, top);
}

///////////////////////////////////////////////////////////////////////////////
void 
glapplication::run ()
{
  glwindow::instance().run();
}
