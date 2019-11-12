#ifndef BUW_GLAPPLICATION_HPP
#define BUW_GLAPPLICATION_HPP

// header, system 
#include <glwindow.hpp>

// header, project

class glapplication 
{
public :
  
  glapplication(int argc, char* argv[]);
  virtual ~glapplication();

public : // methods
	
  // interface of application class
  virtual void draw          () = 0;
  virtual void mouse_press   () = 0;
  virtual void mouse_release () = 0;

  // gets instance of window to draw in
  glwindow& window        ();

  // set resolution
  void set_resolution     ( int width, int height );

  // change coordinate range that is drawn into window
  void set_range          ( float left, float right, float top, float bottom );
  
  // run the application
  void run                ();

private: // member

};

#endif // BUW_GLAPPLICATION_HPP
