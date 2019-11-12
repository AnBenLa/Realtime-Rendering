#ifndef BUW_GLWINDOW_HPP
#define BUW_GLWINDOW_HPP

// header, system 
#include <string>
#include <functional>

// header, project

class glwindow {
public :
  
  ~glwindow();

private : // single pattern - glwindow not copyable

  glwindow(unsigned width, unsigned height,
           unsigned posx,  unsigned posy,
		       int argc, char* argv[],
           std::function<void ()> draw_fun_ptr,
           std::function<void ()> press_fun_ptr,
           std::function<void ()> release_fun_ptr );
  
  
public : // methods
		        
  // draw a line from start to end in color (r,g,b in [0..255])
  // x, y in [0..1] 
  void draw_line2d ( double x0, double y0, 
                     double x1, double y1,
                     unsigned r, 
                     unsigned g, 
                     unsigned b ) const;

  // draw a point at position in color (r,g,b in [0..255]) 
  // x, y in [0..1] 
  void draw_point2d ( double x, 
                      double y,
                      unsigned r,
                      unsigned g,
                      unsigned b ) const;
  
  // get current relative mouse position in window
  void mouse_position (double& x, double& y) const;

  // set range of display
  void ortho ( float l, float r, float b, float t );

  void reshape_window ( int w, int h );

  // display window
  void run() const;
  
  static void init(unsigned width, unsigned height,
                   unsigned posx, unsigned posy,
		               int argc, char* argv[],
		               std::function<void ()> draw_fun_ptr,
                   std::function<void ()> press_fun_ptr,
                   std::function<void ()> release_fun_ptr);
		               
	static glwindow& instance();

public: // methods

  void draw() const;

  void reshape (int, int);

  void set_mouse_position(float, float);

  void mouse_button ( int button, int state);

private: // member  
  glwindow(glwindow const&);

  glwindow& operator=(glwindow const&);
  


  unsigned width_;
  unsigned height_;
  
  unsigned posx_;
  unsigned posy_;
  
  float mousex_;
  float mousey_;

  float left_;
  float right_;
  float bottom_;
  float top_;
  
  int argc_;
  char** argv_;
  
  // pointer to draw function
  std::function<void ()> draw_cb_;
  std::function<void ()> release_cb_;
  std::function<void ()> press_cb_;

public : // external methods 



};


void display();
void reshape_cb(int, int);
void idle();
void mouse(int,int);
void mouse_func(int,int, int, int);

#endif // BUW_GLWINDOW_HPP
