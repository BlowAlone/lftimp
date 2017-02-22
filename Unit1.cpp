//---------------------------------------------------------------------------

#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define WIDTH 1000
#define HEIGHT 450
#define PI 3.14159265358979
#define ABS(x) ((x) >= 0 ? (x) : -(x))

TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

class object
{
protected:
  double x, y;
  object (double x1, double y1): x(x1), y(y1){}
  object ( void ){}
  virtual void Draw (void)
  {
  }
};

class ball: private object
{
  friend void Collision (void);
private:
  const int r;
  const double maxv;
  double vx, vy;
public:
  ball( void ): object(50, 70), r(30), vx(4), vy(5), maxv(sqrt(16 + 25))
  {
  }
  void Move( void )
  {
    if (x <= 0 || x >= Form1->PaintBox1->Width - r)
      vx *= -1;
    if (y <= 0 || y >= Form1->PaintBox1->Height - r)
      vy *= -1;
    x += vx;
    y += vy;
  }
  void Draw( void )
  {
    Form1->PaintBox1->Canvas->Brush->Color = clBlack;
    Form1->PaintBox1->Canvas->Ellipse(x,y,x + r,y + r);
  }
} *ball1 = new ball();

class arcon: private object
{
  friend void Collision (void);
private:
  const int r;
public:
  arcon (void): object(WIDTH / 2 - 50, HEIGHT - 50), r(100){}
  void Draw (void)
  {
    Form1->PaintBox1->Canvas->Brush->Color = clBlue;
    Form1->PaintBox1->Canvas->Ellipse(x,y,x + r,y + r);
  }
  void Move (POINT *p)
  {
    x = p->x;
    if (x < 0)
      x = 0;
    else if (x > WIDTH - r)
      x = WIDTH - r;
  }

} *arcon1 = new arcon();

void Collision (void)
{
  static bool IsCol = true;
  /*double a = arcon1->Width / 2, b = arcon1->Height / 2, c = sqrt(a * a - b * b);
  double x1 = arcon1->x + a - c, x2 = arcon1->x + a + c, y = HEIGHT - 15;
  double len = sqrt((y - ball1->y) * (y - ball1->y) + (x1 - ball1->x) * (x1 - ball1->x)) +
               sqrt((y - ball1->y) * (y - ball1->y) + (x2 - ball1->x) * (x2 - ball1->x));*/
  double x1 = arcon1->x + arcon1->r / 2, x2 = ball1->x + ball1->r / 2,
         y1 = arcon1->y + arcon1->r / 2, y2 = ball1->y + ball1->r / 2;
  double len2 = (x1 - x2) * (x1 - x2) +
                (y1 - y2) * (y1 - y2);
  if (len2 <= (ball1->r / 2 + arcon1->r / 2) * (ball1->r / 2 + arcon1->r / 2) && !IsCol)
  {
    double nx = x2 - x1, ny = y2 - y1,
      nlen = sqrt(nx * nx + ny * ny);
    /*double alpha = acos((- ball1->vx * vecx - ball1->vy * vecy) / ball1->maxv / veclen),
      beta = acos(vecx / veclen);*/
    double cosa = (nx / nlen), sina = -(ny / nlen);
    double nvx = ball1->maxv * (ball1->vx * nx + ball1->vy * ny) / ball1->maxv / nlen,
      nvy = sqrt(ball1->maxv * ball1->maxv - nvx * nvx);
    if (cosa < - ball1->vx / ball1->maxv)
      nvy = - nvy;
    ball1->vx = - nvx * cosa + nvy * sina;
    ball1->vy = nvx * sina + nvy * cosa;
    IsCol = true;
  }
  else if (len2 > (ball1->r / 2 + arcon1->r / 2) * (ball1->r / 2 + arcon1->r / 2))
    IsCol = false;
}

POINT p;

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  Form1->Width = WIDTH + 10;
  Form1->Height = HEIGHT + 31;
  Form1->PaintBox1->Width = WIDTH;
  Form1->PaintBox1->Height = HEIGHT;
  Form1->PaintBox1->Canvas->Brush->Color = clWhite;
  Form1->PaintBox1->Canvas->Rectangle(0, 0, Form1->PaintBox1->Width, Form1->PaintBox1->Height);
  ball1->Draw();
  arcon1->Draw();
  ball1->Move();
  arcon1->Move(&p);
  GetCursorPos(&p);
  Collision();
}
//---------------------------------------------------------------------------
