from tqdm import tqdm
import pygame
import numpy as np


LINES = 100
WINDOWS_SIZE_PX = 400
LIM = 2**32 - 1
DEAD_COLOR = (0x0, 0x0, 0x0)
LIVE_COLOR = (0xFF, 0xFF, 0xFF)

board = np.full((LINES, LINES), False)
# Glider for testing
board[6][2] = True
board[7][3] = True
board[8][1] = True
board[8][2] = True
board[8][3] = True


pygame.init()
screen = pygame.display.set_mode((WINDOWS_SIZE_PX, WINDOWS_SIZE_PX))
screen.fill(DEAD_COLOR)
block_size = int(WINDOWS_SIZE_PX / LINES)

for _ in tqdm(range(int(LIM))):

    aux_board = np.full((LINES, LINES), False)
    # Calculate board
    for x in range(LINES):
        for y in range(LINES):
            
            neighbors = 0
            for nx in range(-1, 2):
                for ny in range(-1, 2):
                    if nx == 0 and ny == 0:
                        continue
                    else:
                        ax = (
                            LINES - 1
                            if x + nx == -1
                            else (0 if x + nx == LINES else x)
                        )
                        ay = (
                            LINES - 1
                            if y + ny == -1
                            else (0 if y + ny == LINES else y)
                        )
                        if board[ax + nx][ay + ny]:
                            neighbors += 1
            if (board[x][y] and neighbors == 2) or neighbors == 3:
                aux_board[x][y] = True
            else:
                aux_board[x][y] = False

    board = aux_board

    # Draw cells
    for x in range(LINES):
        for y in range(LINES):
            color = LIVE_COLOR if board[x][y] else DEAD_COLOR
            rect = pygame.Rect(x * block_size, y * block_size, block_size, block_size)
            pygame.draw.rect(screen, color, rect)

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()

    pygame.display.update()
