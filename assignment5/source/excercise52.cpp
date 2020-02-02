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

				//check if point is already in the mesh
				vector<h_edge*>::iterator it;
				it = std::find_if(h_edges_.begin(),h_edges_.end(),[this, indices, i](h_edge* e){
					return *(e->v_)==(*(vertices_[indices[i]]));
				});

				if(it==h_edges_.end()) {
					h_edges[i] = new h_edge{vertices_[indices[i]]};
					twin_h_edges[i] = new h_edge{vertices_[indices[(i+1)%3]],h_edges[i],t_face};
					vertices_[indices[i]]->edge_ = h_edges[i];
					h_edges_.push_back(h_edges[i]);
					h_edges_.push_back(twin_h_edges[i]);
				}
				else {

					//check if the edge already exists in the mesh
					if((*it)->twin_ != nullptr && *(*it)->twin_->v_ == *vertices_[indices[(i+1)%3]]){
						//use existend edge
						h_edges[i] = *it;

						//add incident face for other half_edge
						(*it)->twin_->face_ = t_face;
						twin_h_edges[i] = (*it)->twin_;
					}
					else{
						h_edges[i] = new h_edge{vertices_[indices[i]],nullptr,nullptr,(*it)->twin_,nullptr};
						twin_h_edges[i] = new h_edge{vertices_[indices[(i+1)%3]],h_edges[i],t_face};
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

				h_edge* twin_edge = twin_h_edges[j];
				twin_edge->next_ = twin_h_edges[(j+1)%3];
				twin_edge->prev_ = twin_h_edges[(j+2)%3];
			}

			//set face edge
			t_face->outer_ = twin_h_edges[0]; 
			faces_.push_back(t_face);
		}

		bool checkCircleCriterion() {

			/*for(auto it = faces_.begin();it!=faces_.end();it++) {

			}*/
			return false;
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
