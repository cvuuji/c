#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <graphics.h>

#define OP1 11
#define OP2 12
#define OP3 13
#define OP4 20
#define OP5 21
#define QUIT 99
#define V 0
#define INF "robot04.dat"
#define OUF "map04.dat"
typedef struct o_s{
    float x[50];
    float y[50];
    int z[15];
    float locx;
    float locy;
    float loca;
} o_t;
typedef struct {
    o_t cc[200];
    int c;
} oc_t;
typedef struct robot_s{
    float x[100];
    float y[100];
    int z[30];
    float cx[10];
    float cy[10];
    float locx;
    float locy;
    float loca;
    float locxx;
    float locyy;
    float locaa;
    int cp; //num control points
} robot_t;
typedef struct {
    robot_t cc[2];
    int c;
} robotc_t;
typedef struct node_s{
    int x;
    int y;
    int ang;
    struct node_s* link;
    struct node_s* parent;
}node_t;
typedef struct list_s{
    struct node_s* top;
}list_t;

void thePanel(){
    setcolor(15);
    setbkcolor(0);
    rectangle(650,50,750,100);
    outtextxy(670,70,"run");
    rectangle(650,100,750,150);
    outtextxy(670,120,"move");
    rectangle(650,150,750,200);
    outtextxy(670,170,"rotate");
    rectangle(650,200,750,250);
    outtextxy(670,220,"pf");
    rectangle(650,250,750,300);
    outtextxy(670,270,"exit");
}
int getOp(int x, int y){
    if(x>650 && x<750 && y>50 && y<100) return OP1;
    else if(x>650 && x<750 && y>100 && y<150) return OP2;
    else if(x>650 && x<750 && y>150 && y<200) return OP3;
    else if(x>650 && x<750 && y>200 && y<250) return OP4;
    else if(x>650 && x<750 && y>250 && y<300) return OP5;
    return QUIT;
}
int fti(float x){//float to int
    return (int) round(x);
}
int pnpoly(int nvert, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = 0, j = nvert-1; i < nvert; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
     (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}
int pnpoly2(int n1, int n2, float *vertx, float *verty, float testx, float testy)
{
  int i, j, c = 0;
  for (i = n1, j = n2; i < n2+1; j = i++) {
    if ( ((verty[i]>testy) != (verty[j]>testy)) &&
     (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
       c = !c;
  }
  return c;
}
int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
    float p2_x, float p2_y, float p3_x, float p3_y)
{
    float s1_x, s1_y, s2_x, s2_y,m;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    m = -s2_x * s1_y + s1_x * s2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (m);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (m);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
        return 1;

    return 0; // No collision
}
float ra(float a){
    float pi = acos(-1);
    return a/180.0*pi;
}
void ini(robotc_t *rob,oc_t *obj){
    int i,j,k;
    float na,nx,ny;
    for(i=rob->c;i>0;i--){
        na = ra(rob->cc[i-1].loca);
        j=0;
        k=0;
        while(rob->cc[i-1].z[j] != -1){
            k += rob->cc[i-1].z[j];
            j++;
        }
        for(j=0;j<k;j++){
            nx = rob->cc[i-1].x[j];
            ny = rob->cc[i-1].y[j];
            rob->cc[i-1].x[j] = nx*cos(na)-ny*sin(na) - rob->cc[i-1].cx[0];
            rob->cc[i-1].y[j] = nx*sin(na)+ny*cos(na) - rob->cc[i-1].cy[0];
        }
        //printf("%d",cos(na));
        nx = rob->cc[i-1].cx[0];
        ny = rob->cc[i-1].cy[0];
        na = ra(rob->cc[i-1].locaa-rob->cc[i-1].loca);
        rob->cc[i-1].locxx += nx*cos(na)-ny*sin(na);
        rob->cc[i-1].locyy += nx*sin(na)+ny*cos(na);
        rob->cc[i-1].locx += rob->cc[i-1].cx[0];
        rob->cc[i-1].locy += rob->cc[i-1].cy[0];
    }
    for(i=obj->c;i>0;i--){
        na = ra(obj->cc[i-1].loca);
        j=0;
        k=0;
        while(obj->cc[i-1].z[j] != -1){
            k += obj->cc[i-1].z[j];
            j++;
        }
        for(j=0;j<k;j++){
            nx = obj->cc[i-1].x[j];
            ny = obj->cc[i-1].y[j];
            obj->cc[i-1].x[j] = nx*cos(na)-ny*sin(na);
            obj->cc[i-1].y[j] = nx*sin(na)+ny*cos(na);
        }
    }
}
int whichOb(int x,int y,oc_t obj){
    int os=0,res,i,j,k;
    for(os;os<obj.c;os++){
        i=0;
        j=0;
        while(obj.cc[os].z[i] != -1){
            res=pnpoly2(j,j+obj.cc[os].z[i]-1,obj.cc[os].x,obj.cc[os].y,round(x)-obj.cc[os].locx,round(y)-obj.cc[os].locy);
            j = j+obj.cc[os].z[i]-1;
            i++;
            if(res != 0) return os;
        }
    }
    return -1;
}
int isRobot(int x,int y,robotc_t rob){
    int os=0,res,i,j,k;
    i=0;
    j=0;
    while(rob.cc[os].z[i] != -1){
        res=pnpoly2(j,j+rob.cc[os].z[i]-1,rob.cc[os].x,rob.cc[os].y,round(x)-rob.cc[os].locx,round(y)-rob.cc[os].locy);
        j = j+rob.cc[os].z[i]-1;
        i++;
        if(res != 0) return 0;
    }
    return -1;
}
void polygon3 (float a[], float b[], int n1, int n2, float aa, float bb, int co) {
    int lastx, lasty,x,y;
    setcolor(co);
    x = fti((a[n1]+aa)*5);
    y = fti((b[n1]+bb)*5);
    int i;
    for (i = n1+1; i < n2+1; i++) {
        lastx = x; lasty = y;
        x = fti((a[i]+aa)*5);
        y = fti((b[i]+bb)*5);
        line (lastx+V,640-lasty+V, x+V, 640-y+V);
    }
    lastx = fti((a[n1]+aa)*5);
    lasty = fti((b[n1]+bb)*5);
    line (lastx+V,640-lasty+V, x+V, 640-y+V);
}
void rotate(robotc_t *rob,int a){
    int i,j,k;
    float na,nx,ny;
    for(i=rob->c;i>0;i--){
        na = ra(float(a*10));
        j=0;
        k=0;
        while(rob->cc[i-1].z[j] != -1){
            k += rob->cc[i-1].z[j];
            j++;
        }
        for(j=0;j<k;j++){
            nx = rob->cc[i-1].x[j];
            ny = rob->cc[i-1].y[j];
            rob->cc[i].x[j] = nx*cos(na)-ny*sin(na);
            rob->cc[i].y[j] = nx*sin(na)+ny*cos(na);
        }
    }
}
void rotate2(robotc_t *rob){
    int i,j,k;
    float na,nx,ny;
    for(i=rob->c;i>0;i--){
        na = ra(rob->cc[i-1].loca);
        j=0;
        k=0;
        while(rob->cc[i-1].z[j] != -1){
            k += rob->cc[i-1].z[j];
            j++;
        }
        for(j=0;j<k;j++){
            nx = rob->cc[i].x[j];
            ny = rob->cc[i].y[j];
            rob->cc[i-1].x[j] = nx*cos(na)-ny*sin(na);
            rob->cc[i-1].y[j] = nx*sin(na)+ny*cos(na);
        }
    }
}
void show(robotc_t rob,oc_t obj){
    int i,j,k;
    float oa;
    for(i=rob.c;i>0;i--){
        j=0;
        k=0;
        while(rob.cc[i-1].z[j] != -1){
            polygon3(rob.cc[i-1].x,rob.cc[i-1].y,k,k+rob.cc[i-1].z[j]-1,rob.cc[i-1].locx,rob.cc[i-1].locy,2);
            k = k+rob.cc[i-1].z[j];
            j++;
        }
    }
    for(i=obj.c;i>0;i--){
        j=0;
        k=0;
        while(obj.cc[i-1].z[j] != -1){
            polygon3(obj.cc[i-1].x,obj.cc[i-1].y,k,k+obj.cc[i-1].z[j]-1,obj.cc[i-1].locx,obj.cc[i-1].locy,1);
            k = k+obj.cc[i-1].z[j];
            j++;
        }
    }
}
void show2(robotc_t *rob,oc_t *obj){
    int i,j,k;
    for(i=rob->c;i>0;i--){
        j=0;
        k=0;
        while(rob->cc[i-1].z[j] != -1){
            polygon3(rob->cc[i-1].x,rob->cc[i-1].y,k,k+rob->cc[i-1].z[j]-1,rob->cc[i-1].locx,rob->cc[i-1].locy,2);
            k = k+rob->cc[i-1].z[j];
            j++;
        }
    }
    for(i=obj->c;i>0;i--){
        j=0;
        k=0;
        while(obj->cc[i-1].z[j] != -1){
            polygon3(obj->cc[i-1].x,obj->cc[i-1].y,k,k+obj->cc[i-1].z[j]-1,obj->cc[i-1].locx,obj->cc[i-1].locy,1);
            k = k+obj->cc[i-1].z[j];
            j++;
        }
    }
}
void show3(oc_t *obj){
    int i,j,k;
    for(i=obj->c;i>0;i--){
        j=0;
        k=0;
        while(obj->cc[i-1].z[j] != -1){
            polygon3(obj->cc[i-1].x,obj->cc[i-1].y,k,k+obj->cc[i-1].z[j]-1,obj->cc[i-1].locx,obj->cc[i-1].locy,1);
            k = k+obj->cc[i-1].z[j];
            j++;
        }
    }
}
void show4(robotc_t rob){
    int j,k;
    j=0;
    k=0;
    while(rob.cc[0].z[j] != -1){
        polygon3(rob.cc[1].x,rob.cc[1].y,k,k+rob.cc[0].z[j]-1,rob.cc[0].locxx,rob.cc[0].locyy,6);
        k = k+rob.cc[0].z[j];
        j++;
    }
}
void rotate3(robotc_t *rob){
    int i,j,k;
    float na,nx,ny;
    for(i=rob->c;i>0;i--){
        na = ra(rob->cc[i-1].loca);
        j=0;
        k=0;
        while(rob->cc[i-1].z[j] != -1){
            k += rob->cc[i-1].z[j];
            j++;
        }
        for(j=0;j<k;j++){
            nx = rob->cc[i-1].x[j];
            ny = rob->cc[i-1].y[j];
            rob->cc[i-1].x[j] = nx*cos(na)-ny*sin(na);
            rob->cc[i-1].y[j] = nx*sin(na)+ny*cos(na);
        }
    }
    rob->cc[0].loca = 0.0;
}
void rotate4(oc_t *obj,int n){
    int i,j,k;
    float na,nx,ny;
    na = ra(obj->cc[n].loca);
    j=0;
    k=0;
    while(obj->cc[n].z[j] != -1){
        k += obj->cc[n].z[j];
        j++;
    }
    for(j=0;j<k;j++){
        nx = obj->cc[n].x[j];
        ny = obj->cc[n].y[j];
        obj->cc[n].x[j] = nx*cos(na)-ny*sin(na);
        obj->cc[n].y[j] = nx*sin(na)+ny*cos(na);
    }
}
void rotate5(robotc_t *rob,float oa){
    int i,j,k;
    float na,nx,ny;
    if(oa < 0.0) oa += 360.0;
    for(i=rob->c;i>0;i--){
        na = ra(oa);
        j=0;
        k=0;
        while(rob->cc[i-1].z[j] != -1){
            k += rob->cc[i-1].z[j];
            j++;
        }
        for(j=0;j<k;j++){
            nx = rob->cc[i-1].x[j];
            ny = rob->cc[i-1].y[j];
            rob->cc[i].x[j] = nx*cos(na)-ny*sin(na);
            rob->cc[i].y[j] = nx*sin(na)+ny*cos(na);
        }
    }
}
int mouseMonitor2(robotc_t *rob,oc_t *obj){
    int x,y,ox,oy,id=-1,id2=-1,e=1;
    static int op=12;
    //setbkcolor(7);
    while(1){
        delay(100);
        if (ismouseclick(WM_LBUTTONDOWN)){
            getmouseclick(WM_LBUTTONDOWN, x, y);
            id=whichOb((x-V)/5,128-(y-V)/5,*obj);
            id2=isRobot((x-V)/5,128-(y-V)/5,*rob);
            printf("click : (%d,%d)\n", x, y);
            ox = x;
            oy = y;
            if(id+id2 > -2 && op==12 && y<650){
                while(!ismouseclick(WM_LBUTTONUP)){
                    delay(100);
                    cleardevice();
                    getmouseclick(WM_MOUSEMOVE, x, y);
                    if(x != -1 && y != -1 && id != -1 && y < 650){
                        obj->cc[id].locx = (float)x/5;
                        obj->cc[id].locy = (float)128-y/5;
                    }
                    else if(x != -1 && y != -1 && y < 650)
                        {
                        rob->cc[0].locx = (float)x/5;
                        rob->cc[0].locy = (float)128-y/5;
                    }
                    show2(rob,obj);
                    printf("clicking : (%d,%d)\n", x, y);
                }
                thePanel();

                break;
            }
            else if(id+id2 > -2 && op==13 && y<650){
                while(!ismouseclick(WM_LBUTTONUP)){
                    delay(100);
                    cleardevice();
                    getmouseclick(WM_MOUSEMOVE, x, y);
                    if(x != -1 && y != -1 && id != -1 && y < 650){
                        obj->cc[id].loca = (float)(x+y-ox-oy)/10;
                        rotate4(obj,id);
                    }
                    else if(x != -1 && y != -1 && y < 650)
                        {
                        rob->cc[0].loca = (float)(x+y-ox-oy)/10;
                        rob->cc[0].locaa -= rob->cc[0].loca;
                        rotate3(rob);
                    }
                    show2(rob,obj);
                    printf("clicking : (%d,%d)\n", x, y);
                }
                thePanel();

                break;
            }
        }
        if (ismouseclick(WM_LBUTTONUP)){
            getmouseclick(WM_LBUTTONUP, x, y);
            if(x > 650)
                op=getOp(x,y);
            if(op==OP1)
                e=0;
            else if(op==OP2)
                e=1;
            else if(op==OP3)
                e=2;
            else if(op==OP4)
                e=3;
            else if(op==OP5)
                e=-1;
            printf("left clickup : (%d,%d)_%d\n", x, y,op);
            break;
        }
    }
    return e;
}
void simpleInsert(list_t *s,int x,int y,int ang,node_t *z){
    node_t * n1;
    node_t * c;
    n1 = (node_t *) malloc(sizeof(node_t));
    n1->x = x;
    n1->y = y;
    n1->link=NULL;
    n1->parent=z;
    n1->ang = ang;
    if((s->top) == NULL)
        s->top = n1;
    else{
        c = s->top;
        s->top = n1;
        n1->link = c;
    }
}
int cd(robotc_t *rob,oc_t *obj){//collision detect
    int i,j,k,l,m,n,p,q,r;
    float a1,a2,a3,a4;
    float b1,b2,b3,b4;
    for(i=obj->c-1;i>=0;i--){
        l=0;
        m=0;
        while(obj->cc[i].z[l] != -1){
            n=obj->cc[i].z[l];
            for(j=0;j<obj->cc[i].z[l];j++){
                a1=obj->cc[i].x[m]+obj->cc[i].locx;
                a2=obj->cc[i].y[m]+obj->cc[i].locy;
                if(j==0){
                    a3=obj->cc[i].x[m+n-1]+obj->cc[i].locx;
                    a4=obj->cc[i].y[m+n-1]+obj->cc[i].locy;
                }
                else{
                    a3=obj->cc[i].x[m-1]+obj->cc[i].locx;
                    a4=obj->cc[i].y[m-1]+obj->cc[i].locy;
                }
                m++;
                r=0;
                p=0;
                while(rob->cc[0].z[r] != -1){
                    q=rob->cc[0].z[r];
                    for(k=0;k<rob->cc[0].z[r];k++){
                        b1=rob->cc[1].x[p]+rob->cc[1].locx;
                        b2=rob->cc[1].y[p]+rob->cc[1].locy;
                        if(k==0){
                            b3=rob->cc[1].x[p+q-1]+rob->cc[1].locx;
                            b4=rob->cc[1].y[p+q-1]+rob->cc[1].locy;
                        }
                        else{
                            b3=rob->cc[1].x[p-1]+rob->cc[1].locx;
                            b4=rob->cc[1].y[p-1]+rob->cc[1].locy;
                        }
                        p++;

                        if(get_line_intersection(a1,a2,a3,a4,b1,b2,b3,b4) == 1){
                            //printf("%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",a1,a2,a3,a4,b1,b2,b3,b4);
                            return 1;
                        }
                    }
                    r++;
                }
            }
            l++;
        }
    }
    return 0;
}
int ptest(node_t *n1,node_t *n2,int px,int py,int pa){//test if visited
    node_t * c;
    c = n1;
    while(c != NULL){
        if(c->x == px && c->y == py && c->ang == pa) return 1;
        c = c->link;
    }
    c = n2;
    while(c != NULL){
        if(c->x == px && c->y == py && c->ang == pa) return 1;
        c = c->link;
    }
    return 0;
}
void run(robotc_t *rob,oc_t *obj,int p[][128]){
    list_t root[255];
    list_t root2[255];
    int i,j,px,py,pv,test,test2,test3,test4,test5,cdr,cdr2,cdr3,cdr4,cdr5,z;
    float x,y;
    setcolor(4);
    //printf("%.2f %.2f\n",rob->cc[0].loca,rob->cc[0].locaa);
    rob->cc[0].locaa -= rob->cc[0].loca;
    while(rob->cc[0].locaa > 360.0) rob->cc[0].locaa -= 360.0;
    while(rob->cc[0].locaa < 0.0) rob->cc[0].locaa += 360.0;
    for(i=0;i<255;i++) root[i].top = NULL;
    for(i=0;i<255;i++) root2[i].top = NULL;
    node_t *n1,*n2,*n0;
    x = rob->cc[0].locx;
    y = rob->cc[0].locy;
    px = round(x);
    py = round(128-y);
    //printf("%d %d\n",px,py);
    pv = p[px][py];
    rob->cc[1].locx = (float)px;
    rob->cc[1].locy = (float)128-py;
    rotate(rob,0);
    cdr = cd(rob,obj);
    if(cdr != 0) return;
    //test2 = pv;
    simpleInsert(&root[pv],px,py,0,NULL);
    //printf("%d x:%d\n",pv,root[pv].top->x);
    while(1){
        //first
        //delay(50);
        for(i=0;i<255;i++){
            if((root[i].top) != NULL){
                n1 = root[i].top;
                break;
            }
        }
        if(i == 255) break;// fail
        if((n1->link) != NULL) root[i].top = n1->link;
        else root[i].top = NULL;
        //add n1 to closed
        if((root2[i].top) == NULL){
            root2[i].top = n1;
            n1->link = NULL;//...
        }
        else{
            n2 = root2[i].top;
            root2[i].top = n1;
            n1->link = n2;
        }

        //insert
        printf("%d %d %d %d\n",i,n1->x,n1->y,n1->ang);
        circle(n1->x*5,n1->y*5,2);
        px = n1->x;
        py = n1->y;
        rotate(rob,n1->ang);

        if(py < 127){
            pv = p[px][py+1];
            rob->cc[1].locx = (float)px;
            rob->cc[1].locy = (float)128-py-1;
            cdr = cd(rob,obj);
            if(pv < 255) test=ptest(root[pv].top,root2[pv].top,px,py+1,n1->ang);
        }

        if(py > 1){
            pv = p[px][py-1];
            rob->cc[1].locx = (float)px;
            rob->cc[1].locy = (float)128-py+1;
            cdr2 = cd(rob,obj);
            if(pv < 255) test2=ptest(root[pv].top,root2[pv].top,px,py-1,n1->ang);
        }

        if(px < 127){
            pv = p[px+1][py];
            rob->cc[1].locx = (float)px+1;
            rob->cc[1].locy = (float)128-py;
            cdr3 = cd(rob,obj);
            if(pv < 255) test3=ptest(root[pv].top,root2[pv].top,px+1,py,n1->ang);
        }

        if(px > 1){
            pv = p[px-1][py];
            rob->cc[1].locx = (float)px-1;
            rob->cc[1].locy = (float)128-py;
            cdr4 = cd(rob,obj);
            if(pv < 255) test4=ptest(root[pv].top,root2[pv].top,px-1,py,n1->ang);
        }

        pv = p[px][py];
        if(pv == 0){
            x = (float)n1->ang*10;
            x += 360.0;
            while(x > rob->cc[0].locaa) x -= 360.0;
            printf("%.2f %.2f\n",x,rob->cc[0].locaa);
            if((rob->cc[0].locaa - x < 20.1 && rob->cc[0].locaa > x) ||
               (x - rob->cc[0].locaa < 20.1 && rob->cc[0].locaa < x)) break;
            rob->cc[1].locx = (float)px;
            rob->cc[1].locy = (float)128-py;
            if(rob->cc[0].locaa < x+180.0){
                z=n1->ang+2;
                test5=ptest(root[pv].top,root2[pv].top,px,py,z);
                if(test5 == 0) simpleInsert(&root[pv],px,py,z,n1);
            }
            else{
                z=n1->ang-2;
                test5=ptest(root[pv].top,root2[pv].top,px,py,z);
                if(test5 == 0) simpleInsert(&root[pv],px,py,z,n1);
            }
        }
        else if(cdr == 1 || cdr2 == 1 || cdr3 == 1 || cdr4 == 1){
            rob->cc[1].locx = (float)px;
            rob->cc[1].locy = (float)128-py;
            z = n1->ang+2;
            if(z > 17) z -= 36;
            else if(z < -18) z += 36;
            rotate(rob,z);
            cdr5 = cd(rob,obj);
            test5=ptest(root[pv].top,root2[pv].top,px,py,z);
            if(cdr5 == 0 && test5 == 0) simpleInsert(&root[pv],px,py,z,n1);

            z = n1->ang-2;
            if(z > 17) z -= 36;
            else if(z < -18) z += 36;
            rotate(rob,z);
            cdr5 = cd(rob,obj);
            test5=ptest(root[pv].top,root2[pv].top,px,py,z);
            if(cdr5 == 0 && test5 == 0) simpleInsert(&root[pv],px,py,z,n1);
        }
        //if(pv == 0) break;

        pv = p[px][py+1];
        if(cdr == 0 && test == 0) simpleInsert(&root[pv],px,py+1,n1->ang,n1);
        pv = p[px][py-1];
        if(cdr2 == 0 && test2 == 0) simpleInsert(&root[pv],px,py-1,n1->ang,n1);
        pv = p[px+1][py];
        if(cdr3 == 0 && test3 == 0) simpleInsert(&root[pv],px+1,py,n1->ang,n1);
        pv = p[px-1][py];
        if(cdr4 == 0 && test4 == 0) simpleInsert(&root[pv],px-1,py,n1->ang,n1);
    }
    if(i==255) return;

    n0=NULL;
    n2=n1->parent;
    while(n2 != NULL){
        n1->parent = n0;
        n0 = n1;
        n1 = n2;
        n2 = n2->parent;
    }
    n1->parent = n0;
    test2 = 0;
    test3 = 0;
    while(rob->cc[0].z[test2] != -1){
        test3 += rob->cc[0].z[test2];
        test2++;
    }
    for(test2=0;test2<test3;test2++){
        rob->cc[1].x[test2] = rob->cc[0].x[test2];
        rob->cc[1].y[test2] = rob->cc[0].y[test2];
    }
    //root[i].top = n1;
    while((n1->parent) != NULL){
        //printf("%d %d\n",n1->x,n1->y);
        n1 = n1->parent;
        delay(50);
        cleardevice();
        rob->cc[0].locx = (float)n1->x;
        rob->cc[0].locy = (float)128-n1->y;
        rob->cc[0].loca = (float)10*n1->ang;

        rotate2(rob);
        show2(rob,obj);
    }
    //rotate(rob,fti((360.0-rob->cc[0].loca)/10.0));
    rob->cc[0].loca = rob->cc[0].locaa;
    rotate2(rob);
    cleardevice();
    show2(rob,obj);
    thePanel();
    //rob->cc[0].locaa = 0.0;
    rob->cc[0].loca = rob->cc[0].locaa = 0.0;

    rotate5(rob,rob->cc[0].locaa);
}
void pff(int s[][128],int c){
    int i,j,z=0;

    for(i=1;i<127;i++)
        for(j=1;j<127;j++){
            if(s[i][j] == 254 &&
              (s[i-1][j] == c ||
               s[i+1][j] == c ||
               s[i][j-1] == c ||
               s[i][j+1] == c))
            {
                s[i][j] = c+1;
                z++;
            }

        }
    if(z == 0) return;
    pff(s,c+1);
}
void pf(robotc_t rob,oc_t obj,int p[][128]){
    //int p[128][128];
    int i,j,x,y;
    for(i=1;i<127;i++)
        for(j=1;j<127;j++)
            p[i][j]=254;
    //find 255
    for(i=0;i<128;i++){
        for(j=0;j<128;j++){
            if(i==0 || i==127 || j==0 || j==127) p[i][j] = 255;
            else if(whichOb(i,j,obj) != -1)
                p[i][127-j] = 255;
        }
    }

    //p[20][70] = 0;
    x = round(rob.cc[0].locxx);
    y = round(128-rob.cc[0].locyy);
    //x = round(rob.cc[0].cx[0]);
    //y = round(128-rob.cc[0].cy[0]);
    p[x][y] = 0;
    pff(p,0);
}
void showPf(int p[][128]){
    int i,j;
    for(i=0;i<128;i++)
        for(j=0;j<128;j++){
            if(p[i][j] == 254){
                setcolor(12);
                circle(i*5+V,j*5+V,2);
            }
            else if(p[i][j] == 255){
                setcolor(0);
                circle(i*5+V,j*5+V,2);
            }
            else if(p[i][j] == 0){
                setcolor(11);
                circle(i*5+V,j*5+V,2);
            }
            else if(p[i][j] < 40){
                setcolor(10);
                circle(i*5+V,j*5+V,2);
            }
            else if(p[i][j] < 80){
                setcolor(9);
                circle(i*5+V,j*5+V,2);
            }
            else if(p[i][j] < 120){
                setcolor(8);
                circle(i*5+V,j*5+V,2);
            }
            else{
                setcolor(7);
                circle(i*5+V,j*5+V,2);
            }
        }
}

int main(){
    FILE *fPtr;
    int a[1999],i,j,x,y,z,v,w,u;
    char cc[499];
    int pp[128][128];
    //float test,test2;
    robotc_t rc;
    rc.c = 1;
    for(i=0;i<1999;i++) a[i]=0;
    //get comment lines
    fPtr = fopen(INF, "r");
    if (!fPtr) {
        printf("gg...\n");
        exit(1);
    }
    i=0;
    while (fgets(cc, 50, fPtr) != NULL) {
        if(cc[0] == '#' || cc[0] == '\n')
            a[i]++;
        i++;
    }
    fclose(fPtr);
    //data input
    fPtr = fopen(INF, "r");
    if (!fPtr) {
        printf("gg...\n");
        exit(1);
    }
    i=0;
    j=0;
    z=0;
    v=0;
    w=0;
    while (!feof(fPtr)) {
        while(a[i] == 1) {fgets(cc, 50, fPtr);printf("%d-%s",i,cc);i++;}
        if(a[i] != 1){
            if(j%8 == 0){
                fscanf(fPtr,"%d\n",&x); //robots
                //printf("x%d\n",x);
                j++;
            }
            else if(j%8 == 1){
                fscanf(fPtr," %d\n",&y); //polygons
                rc.cc[0].z[y] = -1;
                //printf("y%d\n",y);
                j++;
                w=0;
            }
            else if(j%8 == 2){
                fscanf(fPtr,"%d\n",&rc.cc[0].z[w]); //number of vertices
                //printf("%d %d %d\n",x,y,rc.cc[0].z[w]);
                w++;
                j++;
            }
            else if(j%8 == 3){
                fscanf(fPtr,"%f %f\n",&rc.cc[0].x[z],&rc.cc[0].y[z]); //vertices
                //printf("3..%.2f %.2f\n",rc.cc[0].x1[z],rc.cc[0].y1[z]);
                z++;
                v++;
                if(v == rc.cc[0].z[w-1]){
                    j++;
                    v=0;
                    if(rc.cc[0].z[w] != -1) j -= 2;
                    else z=0;
                }
            }
            else if(j%8 == 4){
                fscanf(fPtr,"%f %f %f\n",&rc.cc[0].locx,&rc.cc[0].locy,&rc.cc[0].loca);
                //printf("4..%.2f %.2f\n",rc.cc[0].locx,rc.cc[0].locy);
                j++;
            }
            else if(j%8 == 5){
                fscanf(fPtr,"%f %f %f\n",&rc.cc[0].locxx,&rc.cc[0].locyy,&rc.cc[0].locaa);
                j++;
            }
            else if(j%8 == 6){
                fscanf(fPtr," %d\n",&rc.cc[0].cp); //num control points
                j++;
            }
            else if(j%8 == 7){
                fscanf(fPtr,"%f %f\n",&rc.cc[0].cx[z],&rc.cc[0].cy[z]); //control point
                z++;
                if(z == rc.cc[0].cp){
                    rc.cc[0].cp = z;
                    j++;
                    z=0;
                    break;// 1 robot only
                }
            }
        }
        i++;
    }
    fclose(fPtr);

    oc_t oc;
    oc.c = 1;
    //obstacles
    for(i=0;i<1999;i++) a[i]=0;
    //get comment lines
    fPtr = fopen(OUF, "r");
    if (!fPtr) {
        printf("gg...\n");
        exit(1);
    }
    i=0;
    while (fgets(cc, 50, fPtr) != NULL) {
        if(cc[0] == '#' || cc[0] == '\n')
            a[i]++;
        i++;
    }
    fclose(fPtr);
    //data input
    fPtr = fopen(OUF, "r");
    if (!fPtr) {
        printf("gg...\n");
        exit(1);
    }
    i=0;
    j=0;
    z=0;
    v=0;
    w=0;
    x=0;
	u=0;
    while (!feof(fPtr)) {
        while(a[i] == 1) {fgets(cc, 50, fPtr);printf("%d-%s",i,cc);i++;}
        if(a[i] != 1){
            if(j%5 == 0){
                fscanf(fPtr,"%d\n",&oc.c); //obstacles
                printf("0:%d\n",oc.c);
                j++;
				u++;
            }
            else if(j%5 == 1){
                fscanf(fPtr," %d\n",&y); //polygons
                oc.cc[x].z[y] = -1;
                //printf("1:%d\n",y);
                j++;
                w=0;
                //z=0;
            }
            else if(j%5 == 2){//number of vertices
                fscanf(fPtr," %d\n",&oc.cc[x].z[w]);
                //printf("2:%d\n",oc.cc[x].z[w]);
                w++;
                j++;
            }
            else if(j%5 == 3){
                fscanf(fPtr,"%f %f\n",&oc.cc[x].x[z],&oc.cc[x].y[z]); //vertices
                //printf("3:%.2f %.2f\n",oc.cc[x].x[z],oc.cc[x].y[z]);
                z++;
                v++;
                if(v == oc.cc[x].z[w-1]){
                    j++;
                    v=0;
                    if(oc.cc[x].z[w] != -1) j -= 2;
                    else z=0;
                }
            }
            else if(j%5 == 4){
                fscanf(fPtr,"%f %f %f\n",&oc.cc[x].locx,&oc.cc[x].locy,&oc.cc[x].loca);
                //printf("4:%.2f\n",oc.cc[x].locx);
                x++;
                j+=2;
				if(u == oc.c) break;
            }
        }
        i++;
    }
    fclose(fPtr);

    //start draw
    initwindow(800,650,"zzz");
    //printf("%.2f %.2f %.2f %.2f\n",oc.cc[2].locx,oc.cc[2].x[4],oc.cc[1].x[1],oc.cc[1].x[2]);
    ini(&rc,&oc);
    rotate5(&rc,rc.cc[0].locaa-rc.cc[0].loca);
    show4(rc);
    show(rc,oc);

    thePanel();
    //pf(oc);
    v=0;
    w=0;
    //pf(oc,pp);
    //list_t root1 = {NULL};
    while(v != -1){
        v=mouseMonitor2(&rc,&oc);
        show4(rc);
        show(rc,oc);
        if(v == 3){
            pf(rc,oc,pp);
            if(w%2 == 0) showPf(pp);
            else {
                cleardevice();
                show(rc,oc);
                thePanel();
            }
            w++;
        }
        else if(v==0){
            pf(rc,oc,pp);
            run(&rc,&oc,pp);
        }
    }

    //getch();
    closegraph();

    return 0;
}
