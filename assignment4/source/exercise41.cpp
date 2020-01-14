#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <iomanip>

using namespace std;

enum dimension {
    xd, yd
};

struct point {
    float x, y;

    bool operator == (point const &b) {
        return (abs(x - b.x) < 0.0001f) && (abs(y-b.y) < 0.0001f);
    }

    bool operator<(point const &b) {
        if (x < b.x)
            return true;
        else if (x == b.x && y == b.y)
            return true;
        return false;
    }

    size_t operator()(const point &b) const noexcept {
        size_t hash = b.x + 10 * b.y;
        return hash;
    };
};

bool operator == (point const &a, point const& b) {
    return (abs(a.x - b.x) < 0.0001f) && (abs(b.y-b.y) < 0.0001f);
}

namespace std {
    template<>
    struct hash<point> {
        std::size_t operator()(const point &p) const noexcept {
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

    bool inside(boundingbox& bb) {
        return bb.contains(bottom_Left) && bb.contains(top_right);
    }

    //TODO needs to be checked with border cases
    bool intersect(boundingbox const &bb) {
        return (min(top_right.x, bb.top_right.x) - max(bottom_Left.x, bb.bottom_Left.x) >= 0)
               && (min(top_right.y, bb.top_right.y) - max(bottom_Left.y, bb.bottom_Left.y) >= 0);
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
    private:
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
    public:
        kdnode(vector<point> const &points_sorted_x,
                vector<point> const &points_sorted_y,
                kdnode* father_node) : bound{boundingbox{{},{}}} {
            father = father_node;
            if(points_sorted_x.size() == 1){
                //signal node as leaf node
                flag = false;
                p = points_sorted_x[0];
            } else {
                //signal node as internal node
                flag = true;
            }
            //create a bounding box for the current node using the sorted points
            float min_x, max_x, min_y, max_y;
            min_x = points_sorted_x[0].x;
            max_x = points_sorted_x[points_sorted_x.size() - 1].x;
            min_y = points_sorted_y[0].y;
            max_y = points_sorted_y[points_sorted_y.size() - 1].y;
            bound = boundingbox{point{min_x, min_y}, {max_x, max_y}};

            //check if we have to split horizontally or vertically
            if(flag) {
                if (father == nullptr) {
                    splitting_line_orientation = xd;
                } else {
                    if (father->splitting_line_orientation == xd)
                        splitting_line_orientation = yd;
                    else
                        splitting_line_orientation = xd;
                }

                //do the split
                point splitting_point;
                vector<point> left_half_x{}, left_half_y{}, right_half_x{}, right_half_y{};
                if (splitting_line_orientation == xd) {
                    //find the median which will be the splitting point
                    int splitting_point_index = (points_sorted_x.size() - 1) / 2;
                    splitting_point = points_sorted_x[splitting_point_index];
                    splitting_line_position = splitting_point.x;
                    unordered_set<point> left_side;
                    //take all elements lesser than equal the median into the left side
                    for (int i = 0; i <= splitting_point_index; ++i) {
                        left_half_x.push_back(points_sorted_x[i]);
                        left_side.insert(points_sorted_x[i]);
                    }
                    //take all elements larger than the median into the right side
                    for (int i = splitting_point_index + 1; i < points_sorted_x.size(); ++i)
                        right_half_x.push_back(points_sorted_x[i]);
                    //if the element is in the left side push it to left side sorted by y, else into the right sight sorted by y
                    for (auto const &point : points_sorted_y) {
                        if (left_side.find(point) != left_side.end()) {
                            left_half_y.push_back(point);
                        } else {
                            right_half_y.push_back(point);
                        }
                    }
                } else {
                    //find the median which will be the splitting point
                    int splitting_point_index = (points_sorted_y.size() - 1) / 2;
                    splitting_point = points_sorted_y[splitting_point_index];
                    splitting_line_position = splitting_point.y;
                    unordered_set<point> left_side;
                    //take all elements lesser than equal the median into the left side
                    for (int i = 0; i <= splitting_point_index; ++i) {
                        left_half_y.push_back(points_sorted_y[i]);
                        left_side.insert(points_sorted_y[i]);
                    }
                    //take all elements larger than the median into the right side
                    for (int i = splitting_point_index + 1; i < points_sorted_y.size(); ++i)
                        right_half_y.push_back(points_sorted_y[i]);
                    //if the element is in the left side push it to left side sorted by y, else into the right sight sorted by y
                    for (auto const &point : points_sorted_x) {
                        if (left_side.find(point) != left_side.end()) {
                            left_half_x.push_back(point);
                        } else {
                            right_half_x.push_back(point);
                        }
                    }
                }
                if (!left_half_x.empty())
                    left_child = new kdnode{left_half_x, left_half_y, this};
                if (!right_half_x.empty())
                    right_child = new kdnode(right_half_x, right_half_y, this);
            } else {
                left_child = nullptr;
                right_child = nullptr;
            }
        }

        bool is_point(){
            return !flag;
        }
        point get_point(){
            return p;
        }

        kdnode* get_left_child(){
            return left_child;
        }

        kdnode* get_right_child(){
            return  right_child;
        }

        boundingbox get_boundingbox (){
            return bound;
        }
};

class kdtree {
private:
    kdnode *root;

    vector<point> reportSubtree(kdnode *n) {
        if(n->is_point()){
           return vector<point>{n->get_point()};
        } else {
            vector<point> left_points = reportSubtree(n->get_left_child());
            vector<point> right_points = reportSubtree(n->get_right_child());
            left_points.insert(left_points.end(), right_points.begin(), right_points.end());
            return left_points;
        }
    }

public:
    void build(vector<point> const &points) {
        vector<point> points_sorted_x = {points}, points_sorted_y = {points};
        sort(points_sorted_x.begin(), points_sorted_x.end(),
             [](point const &a, point const &b) -> bool { return a.x < b.x; });
        sort(points_sorted_y.begin(), points_sorted_y.end(),
             [](point const &a, point const &b) -> bool { return a.y < b.y; });
        root = new kdnode{points_sorted_x, points_sorted_y, nullptr};
    }

    vector<point> search(boundingbox&bb) {
        return search(bb, root);
    }

    vector<point> search(boundingbox& bb, kdnode* node){
        if(node->get_boundingbox().inside(bb)){
            return reportSubtree(node);
        } else if(node->get_boundingbox().intersect(bb)){
            vector<point> left_inside = search(bb, node->get_left_child());
            vector<point> right_inside = search(bb, node->get_right_child());
            left_inside.insert(left_inside.end(), right_inside.begin(), right_inside.end());
            return left_inside;
        } else {
            return {};
        }
    }

    //TODO needs to be implemented so that the runtime is in the sqrt of n!!!
    int count(boundingbox range){
        return 0;
    }
};

int main() {
    ifstream cin ("C:\\Users\\Mortiferum\\CLionProjects\\Realtime-Rendering\\assignment4\\source\\input.txt");
    int num_points;
    float x_tmp,y_tmp, x_min, y_min, x_max, y_max;
    cin >> num_points;
    vector<point> points;

    while(num_points--){
        cin >> x_tmp >> y_tmp;
        points.push_back(point{x_tmp, y_tmp});
    }
    cin >> x_min >> y_min >> x_max >> y_max;
    kdtree tree{};
    tree.build(points);
    boundingbox bb{point{x_min,y_min}, point{x_max, y_max}};

    string command;
    cin >> command;

    cout << setprecision(2) << fixed;

    if(command == "PRINT") {
        vector<point> inside = tree.search(bb);
        sort(inside.begin(), inside.end(),
             [](point const& a, point const& b){
                 if(a.y < b.y)
                     return true;
                 else return a.y == b.y && a.x < b.x;
             });
        for (auto const &p : inside) {
            cout << p.x << " " << p.y << "\n";
        }
    } else if(command == "COUNT"){
        cout << tree.count(bb);;
    }
    return 0;
}