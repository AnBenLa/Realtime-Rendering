// header i/f
#include "glwindow.hpp"

// header, system
#include <GL/freeglut.h>
#include <iostream>
#include <cassert>

// header, project


// globals
glwindow* g_glwindow = 0;

////////////////////////////////////////////////////////////////////////////////
glwindow::glwindow(unsigned width, unsigned height, unsigned posx, unsigned posy,
		               int argc, char* argv[], 
                   std::function<void ()> draw_fun_ptr,
                   std::function<void ()> press_fun_ptr,
                   std::function<void ()> release_fun_ptr )
  : width_(width), 
    height_(height),
    posx_(posx),
    posy_(posy), 
    argc_(argc), 
    argv_(argv),
    left_(0.0f),
    right_(1.0f),
    bottom_(0.0f),
    top_(1.0f),
    draw_cb_(draw_fun_ptr),
    press_cb_(press_fun_ptr),
    release_cb_(release_fun_ptr)
{ 
  g_glwindow = this;
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  
  glutInitWindowPosition(int(posx_), int(posy_));
  glutInitWindowSize(int(width_), int(height_));
  glutCreateWindow(argv_[0]);
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape_cb);
  glutPassiveMotionFunc(mouse);
  glutMouseFunc(mouse_func);
  glutIdleFunc(idle);
}

////////////////////////////////////////////////////////////////////////////////
glwindow::~glwindow()
{}

////////////////////////////////////////////////////////////////////////////////
/*static*/ void 
glwindow::init(unsigned width, unsigned height, unsigned posx, unsigned posy,
               int argc, char* argv[], 
               std::function<void ()> draw_fun_ptr,
               std::function<void ()> press_fun_ptr,
               std::function<void ()> release_fun_ptr)
{
  static glwindow instance(width, height, posx, posy, argc, argv, draw_fun_ptr, press_fun_ptr, release_fun_ptr);
}

////////////////////////////////////////////////////////////////////////////////
/*static*/ glwindow& 
glwindow::instance()
{
  assert(g_glwindow);
  return *g_glwindow;
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::draw_line2d(double x0, 
                      double y0, 
                      double x1, 
                      double y1,
                      unsigned r, 
                      unsigned g, 
                      unsigned b ) const
{
  glColor3f(GLfloat(r)/255.0f, GLfloat(g)/255.0f, GLfloat(b)/255.0f);
  glBegin(GL_LINES); {
    glVertex2f(GLfloat(x0), GLfloat(y0));
    glVertex2f(GLfloat(x1), GLfloat(y1));
  } glEnd();
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::draw_point2d(double x, double y, unsigned r, unsigned g, unsigned b) const
{
  glColor3f(GLfloat(r)/255.0f, GLfloat(g)/255.0f, GLfloat(b)/255.0f);
  //glPointSize(3);
  glBegin(GL_POINTS); {
    glVertex2f(GLfloat(x), GLfloat(y));
  } glEnd();
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::mouse_position(double& x, double& y) const
{
  x = mousex_;
  y = mousey_;
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::mouse_button ( int button, int state)
{
  // do not care which button
  switch ( state )
  {
    case GLUT_DOWN : press_cb_(); break;
    case GLUT_UP :   release_cb_(); break;
  };
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::run() const
{
  glutMainLoop();
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::draw() const
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if ( draw_cb_ ) {
    draw_cb_();
  }
  glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::ortho ( float l, float r, float b, float t )
{
  left_ = l;
  right_ = r;
  bottom_ = b;
  top_ = t;
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::reshape_window ( int w, int h )
{
  glutReshapeWindow(w, h);
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::reshape( int w, int h)
{
  width_  = w;
  height_ = h;

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float ar = float(width_) / height_;
  float cx = (right_ + left_)/2.0f;
  float cy = (top_ + bottom_) / 2.0f;
  float lx = right_ - left_;
  float ly = top_ - bottom_;
  glOrtho(cx - lx * 0.5f * ar, cx + lx * 0.5f * ar, top_, bottom_, 0.01, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -50.0f);
  glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////
void 
glwindow::set_mouse_position(float x, float y)
{
  mousex_ = left_ + ( right_ - left_ ) * (x/width_);
  mousey_ = bottom_ + (top_ - bottom_ ) * (y/height_);
}

////////////////////////////////////////////////////////////////////////////////
void 
display() 
{
  glwindow::instance().draw();
}

////////////////////////////////////////////////////////////////////////////////
void 
reshape_cb(int w, int h) 
{
  glwindow::instance().reshape(w, h);
}


////////////////////////////////////////////////////////////////////////////////
void 
idle()
{
  glutPostRedisplay();
}


////////////////////////////////////////////////////////////////////////////////
void 
mouse(int x, int y)
{
  glwindow::instance().set_mouse_position(float(x), float(y));
}


////////////////////////////////////////////////////////////////////////////////
void 
mouse_func(int button, int state, int x, int y)
{
  glwindow::instance().set_mouse_position(float(x), float(y));
  glwindow::instance().mouse_button(button, state);
}
