#include "include.h"

#define EPS 1E-07


void get_bounding_box(BOX *box, POINT *poly, int N); 
int p_outside_bounding_box(BOX box, POINT p); 
double cross_pro(POINT p1, POINT p2, POINT p); 
int p_online(POINT p1,POINT p2, POINT p); 
int intersect(POINT A,POINT B, POINT C, POINT D);
int is_inside(POINT poly[],int NN, POINT q);              // is q inside the polygon


void get_bounding_box(BOX *box, POINT *poly, int N)
{
    int i;
    box->min_x = poly[0].x;
    box->max_x = poly[0].x;
    box->min_y = poly[0].y;
    box->max_y = poly[0].y;

    for (i=1;i<N;i++)
    {
        if  (poly[i].x > box->max_x) box->max_x=poly[i].x;
        if  (poly[i].x < box->min_x) box->min_x=poly[i].x;
        if  (poly[i].y > box->max_y) box->max_y=poly[i].y;  
        if  (poly[i].y < box->min_y) box->min_y=poly[i].y; 
    }
}

// return 1 if outside bonding box, otherwise 0
int p_outside_bounding_box(BOX box, POINT p)
{
    int flag = 0;

    if (p.x < box.min_x || p.x > box.max_x) flag = 1;
    if (p.y < box.min_y || p.y > box.max_y) flag = 1;

    return flag;
}


double cross_pro(POINT p1, POINT p2, POINT p) 
{ 
    return ((p1.x - p.x) * (p2.y - p.y) - (p2.x - p.x) * (p1.y - p.y) ); 
}


// return 1 if on-line
int p_online(POINT p1,POINT p2, POINT p)
{
    double area2;

    if  ((p.x == p1.x) && (p.y == p1.y)) return 1;
    if  ((p.x == p2.x) && (p.y == p2.y)) return 1;
    if  ((p.x >= p1.x) && (p.x >= p2.x)) return 0;
    if  ((p.x <= p1.x) && (p.x <= p2.x)) return 0;
    if  ((p.y >= p1.y) && (p.y >= p2.y)) return 0;
    if  ((p.y <= p1.y) && (p.y <= p2.y)) return 0;

    area2=cross_pro(p1,p2,p);

    if (fabs(area2)<EPS)return 1;

    return 0;
}

// if (t2==0) AB & CD are parallel
// if (t2==0 && t1==0) AB & CD are collinear.
// If 0<=r<=1 && 0<=s<=1, intersection exists
// r<0 or r>1 or s<0 or s>1 line segments do not intersect
// return 1 -- intersect
// return 0 -- not intersect
// return 0 --  (t2==0  && t1 != 0) parallel
// return -1 -- ( t2==0 && t1==0)  collinear.
int intersect(POINT A,POINT B, POINT C, POINT D)
{
    double r,s;
    double t1,t2,t3;

    t2 = (B.x - A.x) * (D.y - C.y) - (B.y-A.y) * (D.x - C.x);
    t1 = (A.y - C.y) * (D.x - C.x) - (A.x-C.x) * (D.y-C.y);

    if  ( (fabs(t2) < EPS) && (fabs(t1) < EPS) ) return -1;
    if  ( (fabs(t2) < EPS) && (fabs(t1) >= EPS) ) return 0;

    r = t1 / t2;
    if  ( (r < 0.0) || (r > 1.0)) return 0;
    t3 = (A.y-C.y)*(B.x-A.x)-(A.x-C.x)*(B.y-A.y);
    s = t3 / t2;
    if  ( (s < 0.0) || (s > 1.0)) return 0;

    return 1;
}

// return 0 -- outside the polygon
// return 1 -- inside the polygon
// return 2 -- online the polygon
int is_inside(POINT poly[],int NN, POINT q)
{
    BOX box;
    extern u32 timer_counter;
    int flag_box,flag_online,flag;
    int NN_cross=0;
    POINT qi[9],qk;   // not use 0
    int i,k;
    double dy,dx,dy2,dx2;
    double shift_rd=0.0;

    get_bounding_box(&box, poly,NN);                    //检查多边形区域范围

    //check 1 -- BoundingBox check
    flag_box = p_outside_bounding_box(box, q);          //检查点是否在范围内

    if(flag_box == 1)return 0;                          //返回点在多边形外部

    //check 2 -- Vertex and Online check
    flag_online = 0;
    for (i=0;i<NN;i++)
    {
        if(p_online(poly[i],poly[i+1],q) == 1)
        {
            flag_online = 1; 
            break;
        } 
    }

    if (flag_online == 1) return 2;                     //返回点在多边形边上

    // make qi[1]
    srand(timer_counter);                               //种随机数种子

    qi[1].x = box.min_x - 1.0;  qi[1].y = q.y;
    qi[2].x = box.max_x + 1.0;  qi[2].y = q.y;
    qi[3].y = box.min_y - 1.0;  qi[3].x = q.x;
    qi[4].y = box.max_y + 1.0;  qi[4].x = q.x;

    shift_rd =  (double)rand() / ((double)(RAND_MAX)+(double)(1)) * 2.0;
    qi[5].x = box.min_x - 1.0; qi[5].y=box.min_y - shift_rd;

    qi[6].x = box.max_x + shift_rd; qi[6].y=box.min_y - 1.0;

    shift_rd =  (double)rand() / ((double)(RAND_MAX)+(double)(1)) * 2.0;
    qi[7].x = box.max_x + 1.0; qi[7].y=box.max_y + shift_rd;

    qi[8].x = box.min_x - shift_rd; qi[8].y=box.max_y + 1.0;

    // which Ray
    k = 1;
    flag = 0;
    for (i=0;i<NN;i++)
    {
        if((poly[i].y == q.y)&&(poly[i].x < q.x))
        {
            flag=1;
            break;
        }
    }

    if (flag == 0) goto Lab_K;

    k=2;
    flag=0;
    for (i=0;i<NN;i++)
    {
        if((poly[i].y == q.y) && (poly[i].x > q.x))
        {
            flag=1;
            break;
        }
    }

    if (flag == 0) goto Lab_K;

    k=3;
    flag=0;
    for (i=0;i<NN;i++)
    {
        if((poly[i].x == q.x)&&(poly[i].y < q.y))
        {
            flag=1;
            break;
        }
    }

    if (flag == 0) goto Lab_K;

    k=4;
    flag=0;
    for (i=0;i<NN;i++)
    {
        if((poly[i].x == q.x) && (poly[i].y > q.y)) 
        {
            flag=1;
            break;
        }
    }

    if (flag == 0) goto Lab_K;

    k=5;
    flag=0;
    dx = q.x - qi[5].x;
    if(fabs(dx) < EPS) goto Lab_6;

    dy = q.y - qi[5].y;
    for (i=0;i<NN;i++)
    {
        dx2= poly[i].x - qi[5].x;
        dy2= poly[i].y - qi[5].y;
        if(fabs( dy * dx2 - dx * dy2) < EPS) 
        {
            flag=1;
            break;
        }
    }

    if (flag == 0) goto Lab_K;



Lab_6:
    k=6;
    flag = 0;
    dx = q.x - qi[6].x;
    if  ( fabs(dx) < EPS) goto Lab_7;

    dy = q.y - qi[6].y;

    for (i=0;i<NN;i++)
    {
        dx2= poly[i].x - qi[6].x;
        dy2= poly[i].y - qi[6].y;
        if ( fabs( dy * dx2 - dx * dy2) < EPS) 
        {
            flag=1;
            break;
        }
    }
    if (flag == 0) goto Lab_K;

Lab_7:
    k=7;
    flag = 0;
    dx = q.x - qi[7].x;
    if  ( fabs(dx) < EPS) goto Lab_8;
    dy = q.y - qi[7].y;

    for (i=0;i<NN;i++)
    {
        dx2= poly[i].x - qi[7].x;
        dy2= poly[i].y - qi[7].y;
        if ( fabs( dy * dx2 - dx * dy2) < EPS)
        {
            flag=1;
            break;
        }
    }

    if (flag == 0) goto Lab_K;

Lab_8:
    k=8;
    flag = 0;
    dx = q.x - qi[8].x;
    if(fabs(dx) < EPS) goto Lab_9;
    dy = q.y - qi[8].y;
    for (i=0;i<NN;i++)
    {
        dx2= poly[i].x - qi[8].x;
        dy2= poly[i].y - qi[8].y;

        if ( fabs( dy * dx2 - dx * dy2) < EPS)
        {
            flag=1;
            break;
        }
    }
    if (flag == 0) goto Lab_K;

Lab_9: 
    flag = 0;


Lab_K: 
    qk.x=qi[k].x;                                        
    qk.y=qi[k].y;

    for (i=0;i<NN;i++)
    {
        flag = intersect(poly[i],poly[i+1], qk, q);

        if (flag == 1) NN_cross++;
    }

    if(NN_cross % 2 == 1) 
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



signed char get_sign(float m)
{
    return m/fabs(m);
}

char is_in_line(JTT808t_RouteTurnPoint* item1,JTT808t_RouteTurnPoint* item2)       //是否在路线内 以item1、item2线宽最大值
{
    u8 max_width=item1->section_width>item2->section_width?item1->section_width:item2->section_width; //最大线宽
    POINT poly[4]={0},q={0};
    float x,y; //x轴 y轴
    u32 m,n;   //纬度差  经度差    已乘10的6次方

    q.x=gps_location.latitude;
    q.y=gps_location.longitude;      //对当前点赋值

    if(item2->turn_point_latitude!=item1->turn_point_latitude)
    {
        x=(max_width/2)/(sqrt(1+pow((fabs((int)item2->turn_point_longitude-(int)item1->turn_point_longitude))/(fabs((int)item2->turn_point_latitude-(int)item1->turn_point_latitude)),2)));
        y=x*(fabs((int)item2->turn_point_longitude-(int)item1->turn_point_longitude))/(fabs((int)item2->turn_point_latitude-(int)item1->turn_point_latitude));
    }
    else
    {
        x=0;
        y=max_width/2;
    }

    m=((y*180.0)/3.1415926)*(1000000.0/6371004.0);                                                                 //纬度差值
    n=((x*180.0)/3.1415926)*(1000000.0/6371004.0)/cos(((item1->turn_point_latitude/1000000)*3.1415926)/180);       //经度差值

    poly[0].x=item1->turn_point_latitude+m*get_sign((int)item2->turn_point_longitude-(int)item1->turn_point_longitude);
    poly[0].y=item1->turn_point_longitude-n*get_sign((int)item2->turn_point_latitude-(int)item1->turn_point_latitude);

    poly[1].x=item1->turn_point_latitude-m*get_sign((int)item2->turn_point_longitude-(int)item1->turn_point_longitude);
    poly[1].y=item1->turn_point_longitude+n*get_sign((int)item2->turn_point_latitude-(int)item1->turn_point_latitude);

    poly[2].x=item2->turn_point_latitude-m*get_sign((int)item2->turn_point_longitude-(int)item1->turn_point_longitude);
    poly[2].y=item2->turn_point_longitude+n*get_sign((int)item2->turn_point_latitude-(int)item1->turn_point_latitude);

    poly[3].x=item2->turn_point_latitude+m*get_sign((int)item2->turn_point_longitude-(int)item1->turn_point_longitude);
    poly[3].y=item2->turn_point_longitude-n*get_sign((int)item2->turn_point_latitude-(int)item1->turn_point_latitude);

    return is_inside(poly,4,q);
}
 
