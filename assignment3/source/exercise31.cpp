//
// Created by Anton on 17.12.2019.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

#define us unsigned  short

using namespace std;

struct point{
    short x,y;
    string id;
};

bool operator == (point const& a, point const& b){
    return a.x == b.x && a.y == b.y && a.id == b.id;
}

struct segment{
    string id;
    point a,b;
};

struct event{
    bool intersection;
    string id_a, id_b;
    point p;
};

bool operator > (segment const& s1, segment const& s2){
    return (s1.a.y > s2.a.y && s1.a.y > s2.b.y) || (s1.b.y > s2.a.y && s1.b.y > s2.b.y);
}

void linesweep(map<string,segment>& segments, vector<point>& points){
    sort(points.begin(), points.end(),[](point const& a, point const& b) -> bool{return a.y > b.y;});
    vector<segment> active_segments;
    queue<event> event_queue;
    for(auto const& p : points){
        event_queue.push(event{false,NULL,NULL, p});
    }
    while(!event_queue.empty()){
        event current_event = event_queue.front();
        if(current_event.intersection){
            //TODO implement this in the correct way
            //swap the two intersecting nodes in the active_segment list check for intersections while doing so
            vector<segment>::iterator intersecting_segment_1_it;
            vector<segment>::iterator intersecting_segment_2_it;

            for(auto it = active_segments.begin(); it != active_segments.end(); it++){
                if(it->id == current_event.id_a){
                    intersecting_segment_1_it = it;
                } else if(it->id == current_event.id_b){
                    intersecting_segment_2_it = it;
                }
            }
            //most likely not the correct way to do it!
            swap(intersecting_segment_1_it, intersecting_segment_2_it);

        } else {
            segment current_segment = segments[current_event.p.id];
            if(current_segment.b == current_event.p){
                //if the point in the queue is the lower point of the segment remove the segment from the list of active segments
                vector<segment>::iterator segment_it;
                for(auto it = active_segments.begin(); it != active_segments.end(); it++){
                    if(it->id == current_event.p.id){
                        segment_it = it;
                        break;
                    }
                }
                active_segments.erase(segment_it);
            } else {
                /*
                    TODO implement this in the correct way
                    add the segment to the active segment list in the right position and check for intersections with neighbours
                    the right position is defined by the orientation of the point that should be processed in relation
                    to the active segments. If the point p is left of an segment a the segment of the point p should be left of the segment a
                    if an intersection exists create an intersection event with the intersecting segment ids and push ist to the queue
                */
                active_segments.push_back(current_segment);
            }
        }
        event_queue.pop();
    }
}

int main (){
    us n;
    short x0,y0,x1,y1;
    string id;
    map<string,segment> segments;
    vector<point> points;
    ifstream cin ("C:\\Users\\Anton\\CLionProjects\\Realtime-Rendering\\assignment3\\source\\input.txt");
    cin >> n;
    while(n--){
        cin >> id >> x0 >> y0 >> x1 >> y1;
        points.push_back({x0,y0,id});
        points.push_back({x1,y1,id});
        if(y0 > y1){
            segments.insert({id,{id,{x0,y0,id},{x1,y1,id}}});
        } else {
            segments.insert({id,{id,{x1,y1,id},{x0,y0,id}}});
        }
    }
    linesweep(segments, points);
    return 0;
}