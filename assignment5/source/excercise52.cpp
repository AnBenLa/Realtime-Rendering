#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

struct point{
    double x,y;
};

bool operator == (point const &a, point const& b) {
    return (abs(a.x - b.x) < 0.0001f) && (abs(a.y-b.y) < 0.0001f);
}

struct triangle{
    point p1;
    point p2;
    point p3;    
};

struct circle{
    point center;
    double radius;
};

struct face;
struct vertex;

struct h_edge {
	vertex* v_;    //incident starting-vertex
	h_edge* twin_; //twin half-edge
	face* face_;   //left positioned incident face

	//adjacent halfedges
	h_edge* next_;
	h_edge* prev_;
};

struct vertex {
	point point_;  //coordinates
	h_edge* edge_; //pointer to of the outgoing half edges
};

bool operator == (vertex const& v1, vertex const& v2) {
	return v1.point_==v2.point_;
}

struct face {
	h_edge* outer_;       //one half-edge of outer cycle
	list<h_edge*> inner_; //one half-edge per inner hole
};

circle calculateCircle(point const& A, point const& B, point const& C) {

    /////////////// calculate circle center //////////////
    point center;
    
    //construct two lines with the given points (A,B) and (B,C)
    double deltaX_a = B.x-A.x;
    double deltaY_a = B.y-A.y;
    double deltaX_b = C.x-B.x;
    double deltaY_b = C.y-B.y;

    double AB_mid_y = (A.y+B.y)/2;
    double BC_mid_y = (B.y+C.y)/2;
    double AB_mid_x = (A.x+B.x)/2;
    double BC_mid_x = (B.x+C.x)/2;

    //use the idea that the intersection of two lines, perpendicular to these lines and passing through the midpoints, is the center of the searched circle

    //except vertical lines
    if(deltaX_a==0) { 
        center.y = AB_mid_y;
        if(deltaY_b==0) {
            center.x = BC_mid_x;
        }
        else {
            double mb = deltaY_b/deltaX_b;
            center.x = mb*(BC_mid_y-center.y)+BC_mid_x;
        }
    }
    else if(deltaX_b==0) {
        center.y = BC_mid_y; //check point in circle
        if(deltaY_a==0) {
            center.x = AB_mid_x;
        }
        else {
            double ma = deltaY_a/deltaX_a;
            center.x = ma*(AB_mid_y-center.y)+AB_mid_x;
        }
    }

    //except horizontal lines
    else if(deltaY_a==0) {
        center.x = AB_mid_x;
        double mb = deltaY_b/deltaX_b; 
        center.y = (BC_mid_x-center.x)/mb + BC_mid_y;
    }
    else if(deltaY_b==0){
        center.x = BC_mid_x;
        double ma = deltaY_a/deltaX_a;
        center.y = (AB_mid_x-center.x)/ma + AB_mid_y;
    }
    else {
        //set the equations for the perpendiculars equal
        //solve equation for x
        double ma = deltaY_a/deltaX_a;
        double mb = deltaY_b/deltaX_b; 

        center.x = (ma*mb*(AB_mid_y-BC_mid_y)+mb*AB_mid_x - ma*BC_mid_x)/(mb-ma);
        center.y = AB_mid_y - (center.x - AB_mid_x)/ma;
    }   

    /////////////// calculate radius //////////////
    double radius = sqrt(pow(center.x-A.x,2)+pow(center.y-A.y,2));

    return circle{center,radius};
}

class DCEL {
	public:
		vector<vertex*> vertices_;
		vector<h_edge*> h_edges_;
		vector<face*> faces_;

		bool isEmpty() {
			return (h_edges_.size());
		}

		void insert(point const& point) {
			vertices_.push_back(new vertex{point,nullptr});
		}

		void insertTriangle(unsigned int i1, unsigned int i2, unsigned int i3) {

			unsigned int indices[3] = {i1,i2,i3};
			h_edge* h_edges[3]; 
			h_edge* twin_h_edges[3];

			face* t_face = new face{nullptr,list<h_edge*>()};

			for(int i = 0; i<3; i++) {
				//check if point is already incident to an edge 
				vector<h_edge*>::iterator it;
				it = find_if(h_edges_.begin(),h_edges_.end(),[this, indices, i](h_edge* e){
					return *(e->v_)==(*(vertices_[indices[i]]));
				});

				if(it==h_edges_.end()) {
					h_edges[i] = new h_edge{vertices_[indices[i]],nullptr,t_face,nullptr,nullptr};
					twin_h_edges[i] = new h_edge{vertices_[indices[(i+1)%3]],h_edges[i]};
					vertices_[indices[i]]->edge_ = h_edges[i];
					h_edges_.push_back(h_edges[i]);
					h_edges_.push_back(twin_h_edges[i]);
				}
				else {
					//check if the edge already exists in the mesh
					if((*it)->twin_ != nullptr && *(*it)->twin_->v_ == *vertices_[indices[(i+1)%3]]) {
						//use existend edge
						h_edges[i] = (*it);

						//add incident face for other half_edge
						(*it)->face_ = t_face;
						twin_h_edges[i] = (*it)->twin_;
					}
					else {
						h_edges[i] = new h_edge{vertices_[indices[i]],nullptr,t_face,nullptr,nullptr};
						twin_h_edges[i] = new h_edge{vertices_[indices[(i+1)%3]],h_edges[i]};
						h_edges_.push_back(h_edges[i]);
						h_edges_.push_back(twin_h_edges[i]);
					}
				}
			}

			// link edges together
			for(int j = 0; j<3; j++) {
				h_edge* edge = h_edges[j];
				edge->twin_= twin_h_edges[j];
				edge->next_= h_edges[(j+1)%3];
				edge->prev_= h_edges[(j+2)%3];
			}

			//set face edge
			t_face->outer_ = h_edges[0]; 
			faces_.push_back(t_face);
		}


		bool checkCircleCriterion() {

			bool result = true;

			for(auto it = faces_.begin();it!=faces_.end();it++) {
				point a,b,c;

				h_edge* first_edge = (*it)->outer_;
				h_edge* edge = first_edge;

				//get triangle vertices
				a = first_edge->v_->point_;
				b = first_edge->next_->v_->point_;
				c = first_edge->prev_->v_->point_;

				//go through all edges
				do {
					//edge = edge->next_;
					h_edge* twin;

					//determine if edge has another adjacent triangle
					if(edge->twin_->face_!=nullptr){
						twin = edge->twin_;

						//check criteria for current edge
						//calculate circle
						triangle t = triangle{a,b,c};
						point p = twin->prev_->v_->point_;
        				circle circle = calculateCircle(t.p1,t.p2,t.p3);

          				//use circle equation to check if point is inside
        				if(pow(p.x-circle.center.x,2)+pow(p.y-circle.center.y,2) < pow(circle.radius,2))
            				result = false;
        				else
            				result = true;
					}
					edge = edge->next_;
				}
				while (!(*edge->next_->v_ == *first_edge->v_));
			}
			return result;
		}

		DCEL(){};
		~DCEL() {
			for(auto it = vertices_.begin(); it!=vertices_.end();it++){
				delete(*it);
				*it=nullptr;
			}	

			for (auto it = h_edges_.begin(); it!=h_edges_.end();it++){
				delete(*it);
				*it=nullptr;
			}

			for (auto it = faces_.begin(); it!=faces_.end();it++){
				delete(*it);
				*it=nullptr;
			}
		}
};

int main() {

	ifstream cin ("./input.txt");
    int num_points, num_triangle_indices;
    double x_tmp, y_tmp;

    cin >> num_points >> num_triangle_indices;
    vector<point> points;
    vector<triangle> triangles;

    DCEL dcel;

    if(num_triangle_indices/3 > 1000) {
    	std::cout<<"NO";
    	return 0;
    }

    while(num_points--){
        cin >> x_tmp >> y_tmp;
        points.push_back(point{x_tmp, y_tmp});
        dcel.insert(point{x_tmp, y_tmp});
    }
    while(num_triangle_indices--){
        int p1,p2,p3;
        cin >> p1 >> p2 >> p3;

        triangle t = triangle{points[p1],points[p2],points[p3]};
        triangles.push_back(t);
        dcel.insertTriangle(p1,p3,p2);
    }

    //evaluate triangulation
    if(dcel.checkCircleCriterion())
        std::cout<<"YES";
    else std::cout<<"NO";
	return 0;
}
