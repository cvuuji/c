#define INF "robot04.dat"
#define OUF "map04.dat"

// ----------------------------------------------------------------------------
// wxApp
// ----------------------------------------------------------------------------
//#include "swqMain.h"
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <windows.h>
#include <wx/msgdlg.h>
#include <wx/dirdlg.h>

//(*InternalHeaders(swqFrame)
#include <wx/intl.h>
#include <wx/menu.h>
#include <wx/string.h>
//*)
class swqApp : public wxApp
{
public:
    virtual bool OnInit();
    //friend int driverProgram();

//protected:
//    class wxMediaPlayerFrame* m_frame;
};


// ----------------------------------------------------------------------------
// swqFrame
// ----------------------------------------------------------------------------

class BasicDrawPane : public wxPanel
{

public:
    BasicDrawPane(wxFrame* parent);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void paintNow2(float a[], float b[], int n1, int n2, float aa, float bb, int co);
    void paintNow3(int n1, int n2);

    void render(wxDC& dc);
    void render2(wxDC&  dc, float a[], float b[], int n1, int n2, float aa, float bb, int co);
    void render3(wxDC&  dc, int n1, int n2);
    //render2(wxDC&  dc, float a[], float b[], int n1, int n2, float aa, float bb, int co)

    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */

    DECLARE_EVENT_TABLE()
};



class swqFrame : public wxFrame
{
public:
    int driverProgram();
    void pf();
    void run();
    // Ctor/Dtor
    swqFrame(const wxString& title);
    ~swqFrame();

    // Menu event handlers
    void OnReload(wxCommandEvent& event);
    void OnLoadRobot(wxCommandEvent& event);
    void OnLoadMap(wxCommandEvent& event);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    // Close event handlers
    void OnClose(wxCloseEvent& event);

    //test
    void OnMyEvent(wxCommandEvent& event);
    void OnRun(wxCommandEvent& event);

    //LAYOUT canvas and buttons
    wxFlexGridSizer* sizer;// = new wxFlexGridSizer(1);
    //sizer->AddGrowableCol(0);
    //this->SetSizer(sizer);

    //canvas
    BasicDrawPane * drawPane;// = new BasicDrawPane(this);
    //drawPane = new BasicDrawPane(this);

    //buttons
    wxBoxSizer* vertsizer;// = new wxBoxSizer(wxHORIZONTAL);
    wxButton* m_runButton;
    wxButton* m_moveButton;
    wxButton* m_rotateButton;
    wxButton* m_pfButton;
    wxButton* m_naButton;

    wxStaticText* tt;
    wxString file1;
    wxString file2;

};
bool swqApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    // SetAppName() lets wxConfig and others know where to write
    SetAppName("wxSample");
    swqFrame *frame =
        new swqFrame("wxWidgets Sample");

    frame->Show(true);
    frame->file1=wxEmptyString;
    frame->file2=wxEmptyString;
    frame->driverProgram();
    return true;
}
wxIMPLEMENT_APP(swqApp);

enum
{
    wxID_BUTTONPREV,

    idMenuQuit,
    idMenuAbout,
    idButtonRun,
    idMenuReload,
    idMenuLoadRobot,
    idMenuLoadMap
};

/*const long testId = wxNewId();
wxDECLARE_EVENT(MY_EVENT, wxCommandEvent);
wxDEFINE_EVENT(MY_EVENT, wxCommandEvent);*/

BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)

    EVT_PAINT(BasicDrawPane::paintEvent)
    //EVT_PAINT(testId, MY_EVENT,BasicDrawPane::paintEvent)

END_EVENT_TABLE()

#define c_size 128*5
BasicDrawPane::BasicDrawPane(wxFrame* parent) :
wxPanel(parent,-1,wxPoint(-1,-1),wxSize(c_size,c_size))
{
}
void BasicDrawPane::paintEvent(wxPaintEvent & evt)
{
    return;//ignore events
    wxPaintDC dc(this);
    render(dc);
}
void BasicDrawPane::paintNow()//float a[], float b[], int n1, int n2, float aa, float bb, int co
{
    wxClientDC dc(this);
    render(dc);
}
void BasicDrawPane::paintNow2(float a[], float b[], int n1, int n2, float aa, float bb, int co)
{
    wxClientDC dc(this);
    render2(dc,a,b,n1,n2,aa,bb,co);

}
void BasicDrawPane::render(wxDC&  dc)
{
    static int wid = 40;
    dc.DrawText(wxT("Testing"), wid, 60);
    wid += 5;
}
int fti(float x){//float to int
    return (int) round(x);
}
//draw polygon
void BasicDrawPane::render2(wxDC&  dc, float a[], float b[], int n1, int n2, float aa, float bb, int co)
{
    //const wxString& dir = wxDirSelector("Choose a folder");
    //dc.DrawText(wxT("Testing2"), 60, 60);
    wxPoint vert[100];
    int counter = 0;

    int x,y;
    x = fti((a[n1]+aa)*5);
    y = fti((b[n1]+bb)*5);
    vert[counter++] = wxPoint(x,640-y);
    int i;
    for (i = n1+1; i < n2+1; i++) {
        //lastx = x; lasty = y;
        x = fti((a[i]+aa)*5);
        y = fti((b[i]+bb)*5);
        //line (lastx+V,640-lasty+V, x+V, 640-y+V);
        vert[counter++] = wxPoint(x,640-y);
    }
    //lastx = fti((a[n1]+aa)*5);
    //lasty = fti((b[n1]+bb)*5);
    //line (lastx+V,640-lasty+V, x+V, 640-y+V);
    dc.DrawPolygon(counter,vert);
    //Refresh();
    //Update();
    //ClearBackground();
}

void BasicDrawPane::paintNow3(int n1, int n2)
{
    wxClientDC dc(this);
    render3(dc,n1,n2);

}
void BasicDrawPane::render3(wxDC&  dc, int n1, int n2)
{
    dc.DrawCircle( wxPoint(n1,n2), 1 /* radius */ );
}

BasicDrawPane *activeCanvas;
swqFrame::swqFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1080,720))
{
    //wb
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem3;
    wxMenuItem* MenuItem4;
    wxMenuItem* MenuItem5;

    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();

    MenuItem3 = new wxMenuItem(Menu1, idMenuReload, _("read robot/map files"), _("r"), wxITEM_NORMAL);
    Menu1->Append(MenuItem3);
    MenuItem4 = new wxMenuItem(Menu1, idMenuLoadRobot, _("load robot file"), _("robot"), wxITEM_NORMAL);
    Menu1->Append(MenuItem4);
    MenuItem5 = new wxMenuItem(Menu1, idMenuLoadMap, _("load map file"), _("map"), wxITEM_NORMAL);
    Menu1->Append(MenuItem5);

    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);

    Bind(wxEVT_MENU, &swqFrame::OnQuit, this,
         idMenuQuit);
    Bind(wxEVT_MENU, &swqFrame::OnAbout, this,
         idMenuAbout);
    Bind(wxEVT_MENU, &swqFrame::OnReload, this,
         idMenuReload);
    Bind(wxEVT_MENU, &swqFrame::OnLoadRobot, this,
         idMenuLoadRobot);
    Bind(wxEVT_MENU, &swqFrame::OnLoadMap, this,
         idMenuLoadMap);


    //panel
    sizer = new wxFlexGridSizer(2);
    //sizer->AddGrowableRow(0);
    this->SetSizer(sizer);

    //drawPane = new BasicDrawPane(this);
    activeCanvas = new BasicDrawPane(this);

    vertsizer = new wxBoxSizer(wxVERTICAL);//wxHORIZONTAL wxVERTICAL

    m_runButton = new wxButton();
    m_runButton->Create(this, idButtonRun, "run");
    m_moveButton = new wxButton();
    m_moveButton->Create(this, wxID_BUTTONPREV, "move");
    m_rotateButton = new wxButton();
    m_rotateButton->Create(this, wxID_BUTTONPREV, "rotate");
    m_pfButton = new wxButton();
    m_pfButton->Create(this, wxID_BUTTONPREV, "potential field");
    m_naButton = new wxButton();
    m_naButton->Create(this, wxID_BUTTONPREV, "-");
    //m_runButton->SetToolTip("Previous");
    vertsizer->Add(m_runButton, 0, wxTOP, 5);
    vertsizer->Add(m_moveButton, 0, wxTOP, 5);
    vertsizer->Add(m_rotateButton, 0, wxTOP, 5);
    vertsizer->Add(m_pfButton, 0, wxTOP, 5);
    vertsizer->Add(m_naButton, 0, wxTOP, 5);
    //SetSizer(vertsizer);

    sizer->Add(activeCanvas, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    sizer->Add(vertsizer, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    Bind(wxEVT_BUTTON, &swqFrame::OnRun, this,
         idButtonRun);

    //debug view
    //wxStaticText* tt;
    tt = new wxStaticText();
    tt->Create(this, wxID_ANY, title, wxPoint(650,5));
}
swqFrame::~swqFrame()
{
}

void swqFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void swqFrame::OnAbout(wxCommandEvent& event)
{
    //wxString msg = wxbuildinfo(2);
    wxMessageBox("TODO");
}

void swqFrame::OnRun(wxCommandEvent& event)
{
    pf();
    run();
}

void swqFrame::OnReload(wxCommandEvent& event)
{
    activeCanvas->ClearBackground();
    driverProgram();
}
void swqFrame::OnLoadRobot(wxCommandEvent& event)
{
    //const wxString& robotFile = wxFileSelector("Choose a file");
    //tt->SetLabel("robot file :"+robotFile);
    file1 = wxFileSelector("Choose a file");
    tt->SetLabel("robot file :"+file1);
}
void swqFrame::OnLoadMap(wxCommandEvent& event)
{
    file2 = wxFileSelector("Choose a file");
    tt->SetLabel("map file :"+file2);
}
//test
wxString workingdir()
{
    char buf[256];
    GetCurrentDirectoryA(256, buf);
    return wxString(buf) + '\\';
}

void swqFrame::OnMyEvent(wxCommandEvent& event)
{
	const wxString& dir = wxDirSelector("Choose a folder");
}

//test2
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

robotc_t rc;
oc_t oc;
int pp[128][128];

//num convert
int fti(float x);
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
    int os=0,res,i,j;
    for(;os<obj.c;os++){
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
    int os=0,res,i,j;
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
    //BasicDrawPane::render(wxDC&  dc);
    //GetEventHandler()->ProcessEvent( BasicDrawPane::paintNow() );
    //activeCanvas->paintNow(a,b,n1,n2,aa,bb,co);

    /*int lastx, lasty,x,y;
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
    line (lastx+V,640-lasty+V, x+V, 640-y+V);*/
}

void show(robotc_t rob,oc_t obj){
    int i,j,k;
    //float oa;
    for(i=rob.c;i>0;i--){
        j=0;
        k=0;
        while(rob.cc[i-1].z[j] != -1){
            //polygon3(rob.cc[i-1].x,rob.cc[i-1].y,k,k+rob.cc[i-1].z[j]-1,rob.cc[i-1].locx,rob.cc[i-1].locy,2);
            activeCanvas->paintNow2(rob.cc[i-1].x,rob.cc[i-1].y,k,k+rob.cc[i-1].z[j]-1,rob.cc[i-1].locx,rob.cc[i-1].locy,2);
            k = k+rob.cc[i-1].z[j];
            j++;
        }
    }
    for(i=obj.c;i>0;i--){
        j=0;
        k=0;
        while(obj.cc[i-1].z[j] != -1){
            //polygon3(obj.cc[i-1].x,obj.cc[i-1].y,k,k+obj.cc[i-1].z[j]-1,obj.cc[i-1].locx,obj.cc[i-1].locy,1);
            activeCanvas->paintNow2(obj.cc[i-1].x,obj.cc[i-1].y,k,k+obj.cc[i-1].z[j]-1,obj.cc[i-1].locx,obj.cc[i-1].locy,1);
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
            //polygon3(rob->cc[i-1].x,rob->cc[i-1].y,k,k+rob->cc[i-1].z[j]-1,rob->cc[i-1].locx,rob->cc[i-1].locy,2);
            activeCanvas->paintNow2(rob->cc[i-1].x,rob->cc[i-1].y,k,k+rob->cc[i-1].z[j]-1,rob->cc[i-1].locx,rob->cc[i-1].locy,2);
            k = k+rob->cc[i-1].z[j];
            j++;
        }
    }
    for(i=obj->c;i>0;i--){
        j=0;
        k=0;
        while(obj->cc[i-1].z[j] != -1){
            //polygon3(obj->cc[i-1].x,obj->cc[i-1].y,k,k+obj->cc[i-1].z[j]-1,obj->cc[i-1].locx,obj->cc[i-1].locy,1);
            activeCanvas->paintNow2(obj->cc[i-1].x,obj->cc[i-1].y,k,k+obj->cc[i-1].z[j]-1,obj->cc[i-1].locx,obj->cc[i-1].locy,1);
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
            //polygon3(obj->cc[i-1].x,obj->cc[i-1].y,k,k+obj->cc[i-1].z[j]-1,obj->cc[i-1].locx,obj->cc[i-1].locy,1);
            activeCanvas->paintNow2(obj->cc[i-1].x,obj->cc[i-1].y,k,k+obj->cc[i-1].z[j]-1,obj->cc[i-1].locx,obj->cc[i-1].locy,1);
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
        //polygon3(rob.cc[1].x,rob.cc[1].y,k,k+rob.cc[0].z[j]-1,rob.cc[0].locxx,rob.cc[0].locyy,6);
        activeCanvas->paintNow2(rob.cc[1].x,rob.cc[1].y,k,k+rob.cc[0].z[j]-1,rob.cc[0].locxx,rob.cc[0].locyy,6);
        k = k+rob.cc[0].z[j];
        j++;
    }
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
    int j,k;
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

int swqFrame::driverProgram(){
    FILE *fPtr;
    int a[1999],i,j,x,y,z,v,w,u;
    char cc[499];
    //int pp[128][128];
    robotc_t rc2;
    rc = rc2;
    rc.c = 1;
    for(i=0;i<1999;i++) a[i]=0;
    //get comment lines
    if (file1 != wxEmptyString)
        fPtr = fopen(file1, "r");
    else
        fPtr = fopen(INF, "r");
    if (!fPtr) {
        return (-1);
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
    if (file1 != wxEmptyString)
        fPtr = fopen(file1, "r");
    else
        fPtr = fopen(INF, "r");
    if (!fPtr) {
        return (-1);
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

    //oc_t oc;
    oc.c = 1;
    //obstacles
    for(i=0;i<1999;i++) a[i]=0;
    //get comment lines
    if (file2 != wxEmptyString)
        fPtr = fopen(file2, "r");
    else
        fPtr = fopen(OUF, "r");
    if (!fPtr) {
        return (-1);
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
    if (file2 != wxEmptyString)
        fPtr = fopen(file2, "r");
    else
        fPtr = fopen(OUF, "r");
    if (!fPtr) {
        return (-1);
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
    ini(&rc,&oc);
    rotate5(&rc,rc.cc[0].locaa-rc.cc[0].loca);
    show4(rc);
    show(rc,oc);
    return 0;
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
//void pf(robotc_t rob,oc_t obj,int p[][128]){
void swqFrame::pf(){
    //int (*p)[128][128];
    //p = &pp;
    robotc_t *rob;
    rob = &rc;
    oc_t *obj;
    obj = &oc;

    int i,j,x,y;
    for(i=1;i<127;i++)
        for(j=1;j<127;j++)
            pp[i][j]=254;
    //find 255
    for(i=0;i<128;i++){
        for(j=0;j<128;j++){
            if(i==0 || i==127 || j==0 || j==127) pp[i][j] = 255;
            else if(whichOb(i,j,*obj) != -1)
                pp[i][127-j] = 255;
        }
    }

    //p[20][70] = 0;
    x = round(rob->cc[0].locxx);
    y = round(128 - (rob->cc[0].locyy));
    pp[x][y] = 0;
    pff(pp,0);
}
//void run(robotc_t *rob,oc_t *obj,int p[][128]){
void swqFrame::run(){
    //int (*p)[128][128];
    //p = &pp;
    robotc_t *rob = &rc;
    //rob = rc;
    oc_t *obj = &oc;
    //obj = oc;

    list_t root[255];
    list_t root2[255];
    int i,px,py,pv,test=-1,test2=-1,test3=-1,test4=-1,test5=-1;
    int cdr=-1,cdr2=-1,cdr3=-1,cdr4=-1,cdr5=-1,z;
    float x,y;
    //setcolor(4);
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
    pv = pp[px][py];
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
        wxString tt2;
        tt2.Printf("%d %d %d %d\n",i,n1->x,n1->y,n1->ang);
        tt->SetLabel(tt2);
        //activeCanvas->paintNow3(n1->x*5,n1->y*5);
        //Sleep(50);
        //circle(n1->x*5,n1->y*5,2);

        px = n1->x;
        py = n1->y;
        rotate(rob,n1->ang);

        if(py < 127){
            pv = pp[px][py+1];
            rob->cc[1].locx = (float)px;
            rob->cc[1].locy = (float)128-py-1;
            cdr = cd(rob,obj);
            if(pv < 255) test=ptest(root[pv].top,root2[pv].top,px,py+1,n1->ang);
        }

        if(py > 1){
            pv = pp[px][py-1];
            rob->cc[1].locx = (float)px;
            rob->cc[1].locy = (float)128-py+1;
            cdr2 = cd(rob,obj);
            if(pv < 255) test2=ptest(root[pv].top,root2[pv].top,px,py-1,n1->ang);
        }

        if(px < 127){
            pv = pp[px+1][py];
            rob->cc[1].locx = (float)px+1;
            rob->cc[1].locy = (float)128-py;
            cdr3 = cd(rob,obj);
            if(pv < 255) test3=ptest(root[pv].top,root2[pv].top,px+1,py,n1->ang);
        }

        if(px > 1){
            pv = pp[px-1][py];
            rob->cc[1].locx = (float)px-1;
            rob->cc[1].locy = (float)128-py;
            cdr4 = cd(rob,obj);
            if(pv < 255) test4=ptest(root[pv].top,root2[pv].top,px-1,py,n1->ang);
        }

        pv = pp[px][py];
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

        pv = pp[px][py+1];
        if(cdr == 0 && test == 0) simpleInsert(&root[pv],px,py+1,n1->ang,n1);
        pv = pp[px][py-1];
        if(cdr2 == 0 && test2 == 0) simpleInsert(&root[pv],px,py-1,n1->ang,n1);
        pv = pp[px+1][py];
        if(cdr3 == 0 && test3 == 0) simpleInsert(&root[pv],px+1,py,n1->ang,n1);
        pv = pp[px-1][py];
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
        wxString info_t;
        info_t.Printf("%d %d",n1->x,n1->y);
        tt->SetLabel(info_t);
        n1 = n1->parent;
        Sleep(50);
        //cleardevice();
        rob->cc[0].locx = (float)n1->x;
        rob->cc[0].locy = (float)128-n1->y;
        rob->cc[0].loca = (float)10*n1->ang;

        activeCanvas->ClearBackground();
        rotate2(rob);
        show2(rob,obj);
        //show4(*rob);
    }
    //rotate(rob,fti((360.0-rob->cc[0].loca)/10.0));
    rob->cc[0].loca = rob->cc[0].locaa;
    rotate2(rob);
    activeCanvas->ClearBackground();
    show2(rob,obj);
    //thePanel();
    //rob->cc[0].locaa = 0.0;
    rob->cc[0].loca = rob->cc[0].locaa = 0.0;

    rotate5(rob,rob->cc[0].locaa);
}

