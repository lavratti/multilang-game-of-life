/*
 * Lucas Lavratti
 * 2022/06
 * 
 * Compilled with
 * g++ -Ofast -o game game.cpp -lm -lpthread -lX11 -ggdb3
 *
 */


#include "CImg.h"
#include <iostream>
#include <chrono>

using namespace cimg_library;

int main()
{

  auto t0 = std::chrono::system_clock::now();
  double elapsed_seconds = 0;

  int WINDOWS_SIZE_PX = 400;
  int LINES = 100;
  int cell_size = WINDOWS_SIZE_PX / LINES;
  long int LIM = exp2(32) - 1;
  long int iter = 0;
  int neighbors, nx, ny, ax, ay;
  bool board[LINES][LINES];
  bool aux_board[LINES][LINES];

  // Glider for testing
  board[6][2] = true;
  board[7][3] = true;
  board[8][1] = true;
  board[8][2] = true;
  board[8][3] = true;

  /* GUI startup */
  CImg<unsigned char> image(WINDOWS_SIZE_PX, WINDOWS_SIZE_PX, 1, 1);
  CImgDisplay disp(image);

  while (!(disp.is_closed() || iter >= LIM))
  {

    /* GUI handling */
    CImg<unsigned char> img(WINDOWS_SIZE_PX, WINDOWS_SIZE_PX, 1, 1);
    img.fill(0);
    for (int x = 0; x < LINES; x++)
    {
      for (int y = 0; y < LINES; y++)
      {
        /* Scaling */
        for (int i = 0; i < cell_size; i++)
        {
          for (int j = 0; j < cell_size; j++)
          {
            img[cell_size * x + i + WINDOWS_SIZE_PX * (cell_size * y + j)] = 255 * aux_board[x][y];
          }
        }
      }
    }
    disp.render(img);
    disp.paint();
    /* GUI handling end */

    /* Game of life behaviour*/
    /* Cleanup aux SLOW*/
    for (int x = 0; x < LINES; x++)
    {
      for (int y = 0; y < LINES; y++)
      {
        aux_board[x][y] = false;
      }
    }

    /* Look at every cell*/
    for (int x = 0; x < LINES; x++)
    {
      for (int y = 0; y < LINES; y++)
      {
        /* Look at neighbors*/
        neighbors = 0;
        for (int nx = -1; nx <= 1; nx++)
        {
          for (int ny = -1; ny <= 1; ny++)
          {
            if (!(nx == 0 && ny == 0))
            {
              /* Closed-space board border handling */
              if (x + nx == -1)
              {
                ax = LINES - 1;
              }
              else if (x + nx == LINES)
              {
                ax = 0;
              }
              else
              {
                ax = x;
              }

              if (y + ny == -1)
              {
                ay = LINES - 1;
              }
              else if (y + ny == LINES)
              {
                ay = 0;
              }
              else
              {
                ay = y;
              }
              /* Closed-space board border handling end*/

              if (board[ax + nx][ay + ny])
              {
                neighbors += 1;
              }
            }
          }
        }
        /* Life conditions */
        aux_board[x][y] = ((board[x][y] && neighbors == 2) || neighbors == 3);
      }
    }

    /* Copy aux_board to borad */
    for (int x = 0; x < LINES; x++)
    {
      for (int y = 0; y < LINES; y++)
      {
        board[x][y] = aux_board[x][y];
      }
    }
    /* Game of life behaviour end*/

    iter++;
  }

  /* Print stats */
  elapsed_seconds = (std::chrono::system_clock::now() - t0).count() / 1E9;
  std::cout << "Time: " << elapsed_seconds << "s" << std::endl;
  std::cout << "It: " << (double)iter << std::endl;
  std::cout << "It/s: " << (double)iter / elapsed_seconds << std::endl;

  return 0;
}
