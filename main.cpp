#define _USE_MATH_DEFINES


#include <graphics.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <math.h>

using namespace std;

// рисовка линии встроенной функцией line
void paint_line(int coord[])
{
    line(coord[0], coord[1], coord[2], coord[3]);
}

//перерисовка линии в выбранном цвете
void paint_new_selected(int primary_selected[]){
    setcolor(2);
    paint_line(primary_selected);
    setcolor(15);
}

void plotPixel(int coord[], int dx,
               int dy, int decide, int color=15)
{

    int x1 = coord[0];
    int x2 = coord[2];
    int y1 = coord[1];
    int y2 = coord[3];
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    putpixel(x2, y2, color);
    while(x1 != x2 || y1 != y2)
   {
        putpixel(x1, y1, color);
        int error2 = error * 2;
        if(error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }
}

// поворот линии на angle градусов
void rotate(int* coord, int angle)
{
    int x1 = coord[0];
    int x2 = coord[2];
    int y1 = coord[1];
    int y2 = coord[3];

    double theta = ((angle * 3.14) / 180);

    coord[2] = x1 + ((x2 - x1) * cos(theta)) - ((y2 - y1) * sin(theta));
    coord[3] = y1 + ((x2 - x1) * sin(theta)) + ((y2 - y1) * cos(theta));
}

//начало алгоритма Брезенхэма
void start_alg_Bresenham(int coord[], int color=15){
    int dx = abs(coord[2] - coord[0]);
    int dy = abs(coord[3] - coord[1]);

    if (dx > dy) {
        plotPixel(coord, dx, dy, 0, color);
    }
    else {
        plotPixel(coord, dy, dx, 1, color);
    }
}

// отображение выбранной прямой
void show_selected(int ab, int cd){
    if (cd)
        outtext("Selected: CD");
    else
        outtext("Selected: AB");
}

//увеличение прямой
void scale_line(int* coord){
    int x1 = coord[0];
    int x2 = coord[2];
    int y1 = coord[1];
    int y2 = coord[3];
    int dx = (x2 - x1)*0.5;
    int dy = (y2 - y1)*0.5;
    coord[2] = x2 + dx;
    coord[3] = y2 + dy;
}

// уменьшение прямой
void zoom_out(int* coord){
    int x1 = coord[0];
    int x2 = coord[2];
    int y1 = coord[1];
    int y2 = coord[3];
    int dx = (x2 - x1)*0.5;
    int dy = (y2 - y1)*0.5;
    coord[2] = x2 - dx;
    coord[3] = y2 - dy;
}

int main( )
{
    int i;
    char c;
    int selected_ab = 1;
    int selected_cd = 0;
    // Initialize the graphics window.
    initwindow(500, 500);
    int line_ab_coord[4]{100, 100, 100, 200}; //x1 y1 x2 y2
    int main_point_ab[2]{0, 0};
    int line_cd_coord[4]{200, 100, 200, 200};
    paint_new_selected(line_ab_coord);
    start_alg_Bresenham(line_cd_coord);
    outtext("Selected: AB");
    outtextxy(0, 20, "Arrows to move. R - rotate; I - Increase the size; O - Reduce the size; Tab - Change selected line");
    do
    {
      c = (char) getch( );
      clearviewport();
      if (c != 0){
        if (c == 9){
            if (selected_ab){
                selected_ab = 0;
                selected_cd = 1;
                start_alg_Bresenham(line_cd_coord, 2);
                paint_line(line_ab_coord);
            }
            else{
                selected_ab = 1;
                selected_cd = 0;
                paint_new_selected(line_ab_coord);
                start_alg_Bresenham(line_cd_coord);
            }
        }
        else if (c == 'i' || c == 'I'){
            if (selected_cd){
                    scale_line(line_cd_coord);
                    start_alg_Bresenham(line_cd_coord, 2);
                    paint_line(line_ab_coord);
                }
            else{
                    scale_line(line_ab_coord);
                    paint_new_selected(line_ab_coord);
                    start_alg_Bresenham(line_cd_coord);
                }
        }
        else if (c == 'o' || c == 'O'){
            if (selected_cd){
                    zoom_out(line_cd_coord);
                    start_alg_Bresenham(line_cd_coord, 2);
                    paint_line(line_ab_coord);
                }
            else{
                    zoom_out(line_ab_coord);
                    paint_new_selected(line_ab_coord);
                    start_alg_Bresenham(line_cd_coord);
                }
        }
        else if (c == 'r' || c == 'R'){
            if (selected_cd)
                {
                rotate(line_cd_coord, 10);
                start_alg_Bresenham(line_cd_coord, 2);
                paint_line(line_ab_coord);
            }
            else
            {
                rotate(line_ab_coord, 10);
                paint_new_selected(line_ab_coord);
                start_alg_Bresenham(line_cd_coord);
            }

        }
        show_selected(selected_ab, selected_cd);
        outtextxy(0, 20, "Arrows to move. R - rotate; I - Increase the size; O - Reduce the size; Tab - Change selected line");
        if (selected_cd)
            {
                start_alg_Bresenham(line_cd_coord, 2);
                paint_line(line_ab_coord);
            }
            else
            {
                paint_new_selected(line_ab_coord);
                start_alg_Bresenham(line_cd_coord);
            }

      }
      else
      {   // Process one of the special keys:
        c = (char) getch( );
        show_selected(selected_ab, selected_cd);
        outtextxy(0, 20, "Arrows to move. R - rotate; I - Increase the size; O - Reduce the size; Tab - Change selected line");
        switch (c)
        {
            case KEY_UP: {
                if (selected_cd){
                    line_cd_coord[1] -= 30;
                    line_cd_coord[3] -= 30;
                    start_alg_Bresenham(line_cd_coord, 2);
                    paint_line(line_ab_coord);
                }
                else{
                    line_ab_coord[1] -= 30;
                    line_ab_coord[3] -= 30;
                    paint_new_selected(line_ab_coord);
                    start_alg_Bresenham(line_cd_coord);
                }

                break;
                }
            case KEY_LEFT:    {
                if (selected_cd){
                    line_cd_coord[0] -= 30;
                    line_cd_coord[2] -= 30;
                    start_alg_Bresenham(line_cd_coord, 2);
                    paint_line(line_ab_coord);
                }
                else{
                    line_ab_coord[0] -= 30;
                    line_ab_coord[2] -= 30;
                    paint_new_selected(line_ab_coord);
                    start_alg_Bresenham(line_cd_coord);
                }
                break;
                }
            case KEY_RIGHT:   {
                if (selected_cd){
                    line_cd_coord[0] += 30;
                    line_cd_coord[2] += 30;
                    start_alg_Bresenham(line_cd_coord, 2);
                    paint_line(line_ab_coord);
                }
                else{
                    line_ab_coord[0] += 30;
                    line_ab_coord[2] += 30;
                    paint_new_selected(line_ab_coord);
                    start_alg_Bresenham(line_cd_coord);
                }
                break;
                }
            case KEY_DOWN:    {
                if (selected_cd){
                    line_cd_coord[1] += 30;
                    line_cd_coord[3] += 30;
                    start_alg_Bresenham(line_cd_coord, 2);
                    paint_line(line_ab_coord);
                }
                else{
                    line_ab_coord[1] += 30;
                    line_ab_coord[3] += 30;
                    paint_new_selected(line_ab_coord);
                    start_alg_Bresenham(line_cd_coord);
                }

                break;
                }
            default: cout << "Unknown extended key." << endl;
        }
          }
            }   while ((c != 'x') && (c != 'X'));

            closegraph( );
    }
