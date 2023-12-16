#include <graphics.h>
#include <math.h>
#include <iostream>

using namespace std;

int scale = 1;
int curColor = 0;
void putpixelEllipse(const int &x0, const int &y0, const int &x, const int &y)
{
  bar(x0+x, y0+y, x0+x+scale, y0+y+scale);
  bar(x0+x, y0-y, x0+x+scale, y0-y+scale);
  bar(x0-x, y0-y, x0-x+scale, y0-y+scale);
  bar(x0-x, y0+y, x0-x+scale, y0+y+scale);
}
void drawEllipse(int x0, int y0, int rx, int ry) {
  int x = 0, y = ry;
  int dy = 2*ry*ry*x;
  int dx = 2*rx*rx*y;
  int p1 = ry*ry + rx*rx/4 - rx*rx*ry;
  putpixelEllipse(x0, y0, x, y);
  while(dy < dx)
  {
    ++x;
    dy += 2*ry*ry;
    if(p1 < 0) p1 += ry*ry + dy;
    else{
      --y;
      dx -= 2*rx*rx;
      p1 += ry*ry + dy - dx;
    }
    putpixelEllipse(x0, y0, x, y);
  }
  int p2 = (x+0.5)*(x+0.5)*ry*ry + (y-1)*(y-1)*rx*rx - rx*rx*ry*ry;
  while(y>=0)
  {
    --y;
    dx -= 2*rx*rx;
    if(p2 > 0) p2 += rx*rx - dx;
    else{
      ++x;
      dy += 2*ry*ry;
      p2 += rx*rx + dy - dx;
    }
    putpixelEllipse(x0, y0, x, y);
  }
}
void putpixelCircle(const &cx, const &cy, const int &x, const int &y)
{
  bar(cx + x, cy + y, cx + x + scale, cy + y + scale);
  bar(cx + y, cy + x, cx + y + scale, cy + x + scale);
  bar(cx + y, cy - x, cx + y + scale, cy - x + scale);
  bar(cx + x, cy - y, cx + x + scale, cy - y + scale);
  bar(cx - x, cy - y, cx - x + scale, cy - y + scale);
  bar(cx - y, cy - x, cx - y + scale, cy - x + scale);
  bar(cx - y, cy + x, cx - y + scale, cy + x + scale);
  bar(cx - x, cy + y, cx - x + scale, cy + y + scale);
}
void drawCircle(const int &cx, const int &cy, const int &r) 
{
  int p = 1.25 - r;
  int x = 0, y = r;
  putpixelCircle(cx, cy, x, y);
  while(x <= y)
  {
    ++x;
    if(p >= 0) p += 2*x-2*y+5, --y;
    else p += 2*x + 3;
    if(x > y) break;
    putpixelCircle(cx, cy, x, y);
  }
}

void drawLine(int x1, int y1, int x2, int y2)
{
  bool steep = abs(y2 - y1) > abs(x2 - x1);
  if(steep) swap(x1, y1), swap(x2, y2);
  if(x1 > x2) swap(x1, x2), swap(y1, y2);
  int dx = x2 - x1, dy = abs(y2 - y1);
  int ystep =  y1 < y2 ? 1 : -1;
  int p = -dx;
  dy *= 2; dx *= 2;
  for(int x = x1, y = y1; x <= x2; x++)
  {
    if(steep) bar(y, x, y+scale, x+scale);
    else bar(x, y, x+scale, y+scale);
    p += dy;
    if(p >= 0) y += ystep, p -= dx;
  }
}

struct paint
{
  int DX= 0, DY = 0;
  int xSize=81, ySize=0;
  int xColor = 321, yColor=0;
  void lined(const int &x1, const int &y1, const int &x2, const int &y2)
  {line(x1+DX, y1+DY, x2+DX, y2+DY);}
  void rectangled(const int &x1, const int &y1, const int &x2, const int &y2)
  {rectangle(x1+DX, y1+DY, x2+DX, y2+DY);}
  void floodfilled(const int &x, const int &y, const int &border, const int &color)
  {setfillstyle(SOLID_FILL, color);
   floodfill(x+DX, y+DY, border);}
  void putpixeled(const int &x, const int &y, const int &color)
  {putpixel(x+DX, y+DY, color);}
  void bared(const int &left, const int &top, const int &right, const int &bottom)
  {bar(left+DX, top+DY, right+DX, bottom+DY);}
  void getmouseclickleft(int &x, int &y){
    while (1){
      if(ismouseclick(WM_LBUTTONDOWN)){
        getmouseclick(WM_LBUTTONDOWN, x, y);
        break;
  }}}
  void getmouseclickleft1(const int &x1, const int &y1, int &x2, int &y2){
    int c = getpixel(x1, y1);
    if(c == 0) putpixel(x1, y1, 15);
    else putpixel(x1, y1, 0);
    getmouseclickleft(x2, y2);
    putpixel(x1, y1, c);
  }
  string getStatus(int x, int y)
  {
    x /= 40, y /= 40;
    int value = y * 16 + x;
    switch(value)
    {
      case 0: return "pencil";
      case 1: return "erase";
      case 4: return "line";
      case 5: return "circle";
      case 6: return "square";
      case 7: return "polygon";
      case 20: return "ellipse";
      case 21: return "rectangle";
      case 16: return "color_picker";
      case 17: return "clear";
      default: return ".";
    }
    return ".";
  }
  void intersection(const int &x1, const int &y1, int &x2, int &y2)
  {
      int a = (y2 - y1), b = (x1 - x2), c = x2*y1-y2*x1;
      y2 = 81, x2 = (-b*81-c)/a;
  }
  void drawLineMouse(const int &x1, const int &y1)
  {
    int x2, y2;
    getmouseclickleft(x2, y2);
    if(y2<81) intersection(x1, y1, x2, y2);
    drawLine(x1, y1, x2, y2);
  }
  void drawCircleMouse(const int &x1, const int &y1)
  {

    int x2, y2, radius;
    getmouseclickleft1(x1, y1, x2, y2);
    radius = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    radius = min(radius, (640-x1));
    radius = min(radius, (480-y1-scale));
    radius = min(radius, abs(y1-81));
    radius = min(radius, x1);
    drawCircle(x1, y1, radius);

  }
  void drawEllipseMouse(const int &x1, const int &y1)
  {
    int x2, y2, radius;
    getmouseclickleft1(x1, y1, x2, y2);
    drawEllipse(x1, y1, min(min(x1-scale, 640-x1-scale), abs(x2-x1-scale)), min(min(480-y1-scale, y1-81), abs(y2-y1)));
  }
  void drawSquareMouse(int x1, int y1)
  {
    int x2, y2; getmouseclickleft1(x1, y1, x2, y2);
    y2 = max(y2, 81);
    int len = min(abs(x2-x1),abs(y2-y1));
    if(len < abs(y2-y1)) y2 = y1 + len*(y1<y2?1:-1);
    if(len < abs(x2-x1)) x2 = x1 + len*(x1<x2?1:-1);
    drawLine(x1, y1, x2, y1);
    drawLine(x1, y1, x1, y2);
    drawLine(x2, y1, x2, y2);
    drawLine(x1, y2, x2, y2);
  }
  void drawPolygonMouse(const int &x0, const int &y0)
  {
    int xp, yp, xc = x0, yc = y0;
    while(1)
    {
      if(ismouseclick(WM_LBUTTONDOWN)){
        xp = xc, yp = yc;
        getmouseclick(WM_LBUTTONDOWN, xc, yc);
        if(yc<81) intersection(xp, yp, xc, yc);
        drawLine(xp, yp, xc, yc);
      }
      else if(GetAsyncKeyState(VK_RBUTTON)){
        drawLine(xc, yc, x0, y0);
        return;
      }
    }
  }
  void drawRectangleMouse(int x1, int y1)
  {
    int x2, y2; getmouseclickleft1(x1, y1, x2, y2);
    if(x1 > x2) swap(x1, x2); if(y1 > y2) swap(y1, y2);
    y1 = max(y1, 81);
    y2 = min(y2, 480-scale);
    x2 = min(x2, 640-scale);
    drawLine(x1, y1, x2, y1);
    drawLine(x1, y1, x1, y2);
    drawLine(x2, y1, x2, y2);
    drawLine(x1, y2, x2, y2);
  }
  int xSta = 0, ySta = 0;
  void drawSelectedSatus(int x, int y)
  {
    x /= 40;
    y /= 40;
    setcolor(0);
    rectangle(x*40+1, y*40+1, x*40+39, y*40+39);
    setcolor(15);
    line(x*40+10, y*40+1, x*40+30, y*40+1);
    line(x*40+10, y*40+39, x*40+30, y*40+39);
    line(x*40+1, y*40+10, x*40+1, y*40+30);
    line(x*40+39, y*40+10, x*40+39, y*40+30);
    setcolor(curColor);
  }
  void drawDesectedStatus(int x, int y)
  {
    x /= 40;
    y /= 40;
    setcolor(15);
    rectangle(x*40+1, y*40+1, x*40+39, y*40+39);
    setcolor(curColor);
  }
  int areaOption(const int &x, const int &y){
    if(x <= 80) return 1;
    else if(x <= 160) return 2;
    else if(x <= 320) return 3;
    else return 4;
  }
  int getScale(int x, int y)
  {
    x /= 40, y /= 40; x -= 2;
    return y*2+x+1;
  }
  void drawPencilIcon()
  {
    DX = DY = 0;
    rectangled(16, 4, 24, 8);
    floodfilled(20, 6, 0, 13);
    rectangled(16, 8, 24, 28);
    floodfilled(20, 10, 0, 6);
    lined(20,33,24,28);
    lined(20,33,16,28);
  }
  void drawEraserIcon()
  {
    DX = 1*40, DY = 0;
    rectangled(10, 4, 30, 13);
    rectangled(10, 13, 30, 32);
    floodfilled(20, 16, 0, 13);
  }
  void drawClearIcon()
  {
    DX = 1*40, DY = 1*40;
    setfillstyle(SOLID_FILL, RED);scale = 3;
    drawLine(10+DX, 10+DY, 30+DX, 30+DY);
    drawLine(30+DX, 10+DY, 10+DX, 30+DY);
    setfillstyle(SOLID_FILL, 0);scale = 1;
  }
  void drawPickerIcon()
  {
    DX = 0, DY = 40;
    rectangled(18, 3, 22, 8);
    lined(15, 8, 26, 8);
    putpixeled(18, 3, 15);
    putpixeled(22, 3, 15);
    rectangled(17, 8, 23, 28);
    lined(17, 15, 23, 15);
    putpixeled(17,28, 15);
    putpixeled(23,28, 15);
    rectangled(19, 28, 21, 32);
    putpixeled(20,32, 15);
    putpixeled(20,33, 0);
  }
  void drawSizeIcon()
  {
    DX = 2*40, DY = 0;
    setcolor(0);
    lined(5, 20, 35, 20);
    setfillstyle(SOLID_FILL, 0);
    DX = 3*40, DY = 0;
    bared(4, 20, 36, 22);
    DX = 2*40, DY = 40;
    bared(4, 20, 36, 23);
    DX = 3*40, DY = 40;
    bared(4, 20, 36, 24);
  }
  void drawColorPalete()
  {
    for(int y = 0; y < 2; ++y)
      for(int x = 0; x < 8; ++x)
      {
        setfillstyle(SOLID_FILL, y*8+x);
        DX = 320 + x * 40-x;
        DY = y * 40-y;
        bared(x+3,y+3,x+38,y+38);
      }
  }
  void drawLineIcon()
  {
    DX = 4 * 40, DY = 0;
    lined(10, 10, 35, 30);
  }
  void drawCircleIcon()
  {
    setfillstyle(SOLID_FILL, 0);
    DX = 5 * 40, DY = 0;
    drawCircle(20+DX, 20, 15);
  }
  void drawSquareIcon()
  {
    DX = 6 * 40, DY = 0;
    rectangle(5+DX, 5+DY, 35+DX, 35+DY);
  }
  void drawPolygonIcon()
  {
    DX = 7 * 40, DY = 0;
    lined(10, 5, 25, 5);
    lined(25, 5, 20, 25);
    lined(20, 25, 35, 25);
    lined(35, 25, 35, 35);
    lined(35, 35, 15, 30);
    lined(15, 30, 10, 5);
  }
  void drawEllipseIcon()
  {
    DX = 4 * 40, DY = 40;
    drawEllipse(20+DX, 20+DY, 15, 10);
  }
  drawRectangleIcon()
  {
    DX = 5*40, DY = 40;
    drawLine(5+DX, 10+DY, 35+DX, 10+DY);
    drawLine(5+DX, 30+DY, 35+DX, 30+DY);
    drawLine(5+DX, 10+DY, 5+DX, 30+DY);
    drawLine(35+DX, 10+DY, 35+DX, 30+DY);
  }
  void drawIcon()
  {
    drawPencilIcon();
    drawEraserIcon();
    drawPickerIcon();
    drawClearIcon();
    drawSizeIcon();
    drawColorPalete();
    drawLineIcon();
    drawCircleIcon();
    drawSquareIcon();
    drawPolygonIcon();
    drawEllipseIcon();
    drawRectangleIcon();
  }
  void initalizate()
  {
    setcolor(LIGHTGRAY);
    line(0, 80, 640, 80);
    for(int i = 0; i <= 640; i += 40)
    line(i, 0, i, 80);
    line(0, 0, 640, 0);
    line(0, 40, 640, 40);
    line(80, 0, 80, 80);
    line(160, 0, 160, 80);
    line(400, 0, 400, 80);
    setcolor(BLACK);
    drawIcon();
  }

  void run()
  {
    setfillstyle(SOLID_FILL, 0);
    initalizate();
    drawSelectedSatus(xSta, ySta);
    drawSelectedSatus(xSize, ySize);
    drawSelectedSatus(xColor, yColor);
    int x = 0, y = 0;
    string currentStatus = "pencil";
    curColor = 0;
    setcolor(0);
    POINT pnt;
    int mx, my;
    while(!kbhit())
    {
      if(ismouseclick(WM_LBUTTONDOWN))
      {
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if(y < 80)
        {
          if(areaOption(x, y)%2==1)
          {
            drawDesectedStatus(xSta, ySta);
            xSta = x, ySta = y;
            drawSelectedSatus(xSta, ySta);
            currentStatus = getStatus(x, y);

            if(currentStatus == "pencil")
            {
              setfillstyle(SOLID_FILL, curColor);
            }
            else if(currentStatus == "erase")
            {
              setfillstyle(SOLID_FILL, 15);
            }
            else if(currentStatus == "clear")
            {
              setfillstyle(SOLID_FILL, 15);
              bar(0, 81, 640, 480);
              setfillstyle(SOLID_FILL, curColor);
            }
          }
          else if(areaOption(x, y) == 2)
          {
            scale = getScale(x, y);
            drawDesectedStatus(xSize, ySize);
            drawSelectedSatus(x, y);
            xSize=x, ySize=y;
          }
          else
          {
            drawDesectedStatus(xColor, yColor);
            drawSelectedSatus(x, y);
            xColor = x, yColor = y;
            curColor = (y/40)*8 + (x-320)/40;

            setfillstyle(SOLID_FILL, curColor);
          }
        }
        else
        {
          if(currentStatus == "color_picker")
          {
            curColor = getpixel(x, y);
            setfillstyle(SOLID_FILL, curColor);
            bar(18, 16+40, 23, 28+40);
            drawDesectedStatus(xColor, yColor);
            yColor = (curColor / 8)*40;
            xColor = (curColor % 8)*40+320;
            drawSelectedSatus(xColor, yColor);
          }
          else if(currentStatus != "pencil" && currentStatus != "erase"){
            setfillstyle(SOLID_FILL, curColor);
            if(currentStatus == "line") drawLineMouse(x, y);
            else if(currentStatus == "circle") drawCircleMouse(x, y);
            else if(currentStatus == "square") drawSquareMouse(x, y);
            else if(currentStatus == "polygon") drawPolygonMouse(x, y);
            else if(currentStatus == "rectangle") drawRectangleMouse(x, y);
            else if(currentStatus == "ellipse") drawEllipseMouse(x, y);
          }
        }
      }
      if(GetAsyncKeyState(VK_LBUTTON) && (currentStatus == "pencil" || currentStatus == "erase"))
      {
        GetCursorPos(&pnt);
        ScreenToClient(GetForegroundWindow(), &pnt);
        mx = pnt.x;
        my = pnt.y;
        if(my > 80) bar(mx, my, mx+scale, my+scale);
      }
    }
  }
};

int main()
{
    initwindow(640, 480);
    setbkcolor(WHITE);
    cleardevice();
    paint p;
    p.run();
    getch();
    closegraph();
    return 0;
}
