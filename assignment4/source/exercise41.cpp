#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

enum dimension {
    x, y
};

struct point {
    float x, y;

    bool operator == (point const& b){
        return (x == b.x) && (y == b.y);
    }

    bool operator < (point const& b) {
        if (x < b.x )
            return true;
        else if (x == b.x && y == b.y)
            return true;
        return false;
    }

    size_t operator()(const point& b) const noexcept {
        size_t hash = b.x + 10 * b.y;
        return hash;
    };
};

namespace std {
    template<> struct hash<point>
    {
        std::size_t operator()(const point& p) const noexcept
        {
            return p(p);
        }
    };
};

class boundingbox {
public:
    point bottom_Left{}, top_right{};

    boundingbox(point a, point b) {
        bottom_Left = point{min(a.x, b.x), min(a.y, b.y)};
        top_right = point{max(a.x, b.x), max(a.y, b.y)};
    }

    bool contains(point const &c) {
        return bottom_Left.x <= c.x && c.x <= top_right.x && bottom_Left.y <= c.y && c.y <= top_right.y;
    }

    bool inside(boundingbox &bb) {
        return bb.contains(bottom_Left) && bb.contains(top_right);
    }

    //TODO needs to be checked with border cases
    bool intersect(boundingbox const &bb) {
        return (min(top_right.x,bb.top_right.x) - max(bottom_Left.x, bb.bottom_Left.x) >= 0)
            && (min(top_right.y,bb.top_right.y) - max(bottom_Left.y, bb.bottom_Left.y) >= 0);
    }
};

//returns the boundingbox of a and b
boundingbox operator+(boundingbox const &a, boundingbox const &b) {
    vector<float> y{a.bottom_Left.y, a.top_right.y, b.bottom_Left.y, b.top_right.y};
    vector<float> x{a.bottom_Left.x, a.top_right.x, b.bottom_Left.x, b.top_right.x};
    point bottom_left{*min_element(x.begin(), x.end()), *min_element(y.begin(), y.end())};
    point top_right{*max_element(x.begin(), x.end()), *max_element(y.begin(), y.end())};
    return boundingbox{bottom_left, top_right};
}

class kdnode {
    //true if internal node, false if leaf node
    bool flag;
    kdnode *father;
    kdnode *left_child;
    kdnode *right_child;
    //points on the splitting line are always in the left set!!!
    //y if the splitting line is vertical or x? if the splitting lines is horizontal)
    dimension splitting_line_orientation;
    //the x / y position of the vertical / horizontal line
    float splitting_line_position;
    point p;
    boundingbox bound;

    kdnode(vector<point> const& points_sorted_x, vector<point> const& points_sorted_y){
        if(father == nullptr){
            splitting_line_orientation = x;
        } else {
            if(father->splitting_line_orientation == x)
                splitting_line_orientation = y;
            else
                splitting_line_orientation = x;
        }
        point splitting_point;
        vector<point> left_half_x{}, left_half_y{}, right_half_x{}, right_half_y{};
        if(splitting_line_orientation == x){
            int splitting_point_index = (points_sorted_x.size() - 1)/2;
            splitting_point = points_sorted_x[splitting_point_index];
            splitting_line_position = splitting_point.x;
            unordered_set<point> left_side;
            for(int i = 0; i <= splitting_point_index; ++i){
                left_half_x.push_back(points_sorted_x[i]);
                left_side.insert(points_sorted_x[i]);
            }
            for(int i = splitting_point_index + 1; i < points_sorted_x.size(); ++i)
                right_half_x.push_back(points_sorted_x[i]);
            for(auto const& point : points_sorted_y){
                if(left_side.find(point) != left_side.end()){
                    left_half_y.push_back(point);
                } else {
                    right_half_y.push_back(point);
                }
            }
        } else {
            int splitting_point_index = (points_sorted_y.size() - 1)/2;
            splitting_point = points_sorted_y[splitting_point_index];
            splitting_line_position = splitting_point.y;
            unordered_set<point> left_side;
            for(int i = 0; i <= splitting_point_index; ++i){
                left_half_y.push_back(points_sorted_y[i]);
                left_side.insert(points_sorted_y[i]);
            }
            for(int i = splitting_point_index + 1; i < points_sorted_y.size(); ++i)
                right_half_y.push_back(points_sorted_y[i]);
            for(auto const& point : points_sorted_x){
                if(left_side.find(point) != left_side.end()){
                    left_half_x.push_back(point);
                } else {
                    right_half_x.push_back(point);
                }
            }
        }
        //TODO create children nodes from the left and right halves

    }

};

class kdtree {
private:
    kdnode *root;

    vector<point> reportSubtree(kdnode *n) {
        //TODO implement report Subtree
        return NULL;
    }

public:
    void build(vector<point> const &points) {
        //TODO create tree
    }

    vector<point> search(boundingbox const &bb) {
        //TODO implement search
        return NULL;
    }
};

int main() {
    return 0;
}