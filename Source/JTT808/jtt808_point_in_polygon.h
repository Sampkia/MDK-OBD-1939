#ifndef  JTT808_POINT_IN_POLYGON_H_
#define  JTT808_POINT_IN_POLYGON_H_


typedef struct 
{
    double x;
    double y;
}POINT;

typedef struct
{
    double min_x; 
    double min_y; 
    double max_x; 
    double max_y;
}BOX;

int is_inside(POINT poly[],int NN, POINT q); 
char is_in_line(JTT808t_RouteTurnPoint* item1,JTT808t_RouteTurnPoint* item2);

#endif //JTT808_POINT_IN_POLYGON_H_


