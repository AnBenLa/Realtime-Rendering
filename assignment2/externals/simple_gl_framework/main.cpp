#include <iostream>
#include <glapplication.hpp>
#include <vector>
#include <algorithm>
#ifdef _WIN32
	#include <Windows.h>
#endif

#ifdef _WIN32
#include <gl/glut.h>
#else
#include <GL/glut.h>
#endif

//#include <gl/glu.h>
/////////////////////////////////////////////////////////////////////////////
// glapplication provides the following methods for window interaction
// - window()
// - draw()
// - mouse_press()
// - mouse_release()
/////////////////////////////////////////////////////////////////////////////

class point2D
{
public:
	float x, y;
	point2D()
	{
		x = y = 0.0f;
	}
	point2D(const point2D& p)
	{
		this->x = p.x;
		this->y = p.y;

	}
	point2D(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

using namespace std;

void error_finish()
{
	cout << "Press enter to finish -->";
	getchar();
	exit(1);
}


class application : public glapplication
{
	vector<point2D> input_points;
	vector<vector<point2D>> output_points;
	point2D minPoint, maxPoint;
public :
  /////////////////////////////////////////////////////////////////////////////
	application(int argc, char* argv[])
		: glapplication(argc, argv)
	{
		minPoint = point2D(10000, 100000);
		maxPoint = point2D(-10000, -100000);
		string input = argv[1], output = argv[2];

		FILE* f = fopen(input.c_str(), "rt");
		if (f == NULL)
		{
			cout << "Cannot open file " << input << endl;
			error_finish();
		}
		int n = 0;
		if (fscanf(f, "%d", &n) != 1)
		{
			cout << "Error reading input number of points from " << input << endl;
			fclose(f);
			error_finish();
		}

		for (int i = 0; i < n; i++)
		{
			float x, y;
			if (fscanf(f, "%f %f", &x, &y) != 2)
			{
				cout << "Error reading point number " << i + 1 << " from " << input << endl;
				fclose(f);
				error_finish();
			}
			input_points.push_back(point2D(x, y));
			if (x < minPoint.x) minPoint.x = x;
			if (x > maxPoint.x) maxPoint.x = x;
			if (y < minPoint.y) minPoint.y = y;
			if (y > maxPoint.y) maxPoint.y = y;
		}

		cout << input_points.size() << " has been read from " << input << endl;
		fclose(f);

		minPoint.x = minPoint.y = min(minPoint.x, minPoint.y);
		maxPoint.x = maxPoint.y = max(maxPoint.x, maxPoint.y);

		// checking degenerated space (point)
		if (minPoint.x == maxPoint.x && minPoint.y == maxPoint.y)
		{
			minPoint.x = minPoint.y = minPoint.x - 4;
			maxPoint.x = maxPoint.y = maxPoint.x + 4;
		}

		// extending the bounding box by 20%
		float lx = maxPoint.x - minPoint.x;
		float ly = maxPoint.y - minPoint.y;
		float cx = (maxPoint.x + minPoint.x) / 2;
		float cy = (maxPoint.y + minPoint.y) / 2;
		minPoint.x = cx - lx * 0.6f;
		maxPoint.x = cx + lx * 0.6f;
		minPoint.y = cy - ly * 0.6f;
		maxPoint.y = cy + ly * 0.6f;


		f = fopen(output.c_str(), "rt");
		if (f == NULL)
		{
			cout << "Cannot open file " << output << endl;
			return; // we can continue with the input file only... it is ok!
		}
		while (fscanf(f, "%d", &n) == 1)
		{
			output_points.push_back(vector < point2D >());
			int m = output_points.size() - 1;
			for (int i = 0; i < n; i++)
			{
				float x, y;
				if (fscanf(f, "%f %f", &x, &y) != 2)
				{
					cout << "Error reading the point number " << i + 1 << " from " << output << endl;
					fclose(f);
					error_finish();
				}
				output_points[m].push_back(point2D(x, y));
			}
		}
		cout << "We have read " << output_points.size() << " output polygon(s)" << endl;
		fclose(f);
		set_range(minPoint.x * lx/ly, maxPoint.x * lx / ly, minPoint.y, maxPoint.y);

		glutPostRedisplay();
	}
  /////////////////////////////////////////////////////////////////////////////
  // overwrite method to draw something into the window
  /////////////////////////////////////////////////////////////////////////////
  /*virtual*/ void draw () 
  {
	set_range(minPoint.x, maxPoint.x, minPoint.y, maxPoint.y);
	// draw a yellow (255,255,0) line
	
	for (unsigned i = 0; i < output_points.size(); i++)
	{
		for (unsigned j = 0; j < output_points[i].size(); j++)
		{
			unsigned int J = (j + 1) % output_points[i].size();
			window().draw_line2d(output_points[i][j].x, output_points[i][j].y, output_points[i][J].x, output_points[i][J].y, 255, 255, 0);
		}
	}
	glPointSize(10);
	for (unsigned i = 0; i < input_points.size(); i++)
		window().draw_point2d(input_points[i].x, input_points[i].y, 255, 0, 0);

	glPointSize(5);
	for (unsigned i = 0; i < output_points.size(); i++)
		for (unsigned j = 0; j < output_points[i].size(); j++)
			window().draw_point2d(output_points[i][j].x, output_points[i][j].y, 255, 255, 255);

	glPointSize(2);
  }

  /////////////////////////////////////////////////////////////////////////////
  // overwrite method to handle mouse press event
  /////////////////////////////////////////////////////////////////////////////
  /*virtual*/ void mouse_press ()
  {
    double x, y;
    window().mouse_position(x, y);
    std::cout << "mouse pointer pressed at : [ " << x << " , " << y << " ]" << std::endl;
  }

  /////////////////////////////////////////////////////////////////////////////
  // overwrite method to handle mouse release event
  /////////////////////////////////////////////////////////////////////////////
  /*virtual*/ void mouse_release ()
  {
    double x, y;
    window().mouse_position(x, y);
    std::cout << "mouse pointer released at : [ " << x << " , " << y << " ]" << std::endl;
  }
};


/////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	// checking number of arguments before opening opengl window
	if (argc != 3)
	{
		cout << "Please, use it in this way:" << endl;
		cout << "draw <intput filename> <output filename>" << endl;
		error_finish();
	}

  // create application
  application app(argc, argv);

  // set screen resolution
  app.set_resolution ( 512, 512); 

  // set range that is drawn to: left, right, bottom, top
  // app.set_range ( -350.0f, 350.0f, -350.0f, 350.0f ); 
  
  // run application
  app.run();

  return 0;

}
