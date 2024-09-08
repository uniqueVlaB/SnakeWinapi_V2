#pragma once

#include "Libraries.h"

enum DIRECTION {
	RIGHT, LEFT, UP, DOWN
};

class Field {
private:
	vector <vector<RECT>> m_coords;
public:
	void set(int left, int top, int right, int bottom,int shift, int x, int y, HWND hWnd ) {
		HDC hdc;
		RECT rect;
		int tempX = x / shift;
		int tempY = y / shift;
		rect.top = top;
		rect.bottom = bottom;
		m_coords.clear();
		m_coords.resize(tempY);
		//i строка, b столбец
			for (int i = 0; i < tempY; i++) {
			m_coords[i].resize(tempX);
			rect.left = left;
			rect.right = right;
			for (int b = 0; b<tempX;b++) {
				m_coords[i][b] = rect;
				rect.left += shift;
				rect.right += shift;
			}
			rect.bottom += shift;
			rect.top += shift;
		}
			
	}
	void setBK(HBRUSH bkColor, HDC hdc) {
		
		for (int i = 0; i < m_coords.size(); i++) {
			for (int b = 0; b < m_coords[0].size(); b++) {
				
				FillRect(hdc, &m_coords[i][b], bkColor);
			}
		}
		
	}
	vector<vector<RECT>> getCoords() {
		return m_coords;
	}
};

class Snake {
protected:
	POINT previous, temp;
	vector<POINT>m_snake;
	DIRECTION snakeDir;
public:
	void create(vector<vector<RECT>> coords, int length, HDC hdc, HBRUSH headColor, HBRUSH bodyColor) {
		
		POINT point;
		point.x = 5;
		point.y = 5;
		m_snake.clear();
	    m_snake.push_back(point);
		for (int i = 0; i < length;i++) {
			point.y -= 1;
			m_snake.push_back(point);
		}
		for (int i = 0; i < m_snake.size();i++) {
			if (i==0) {
				
				FillRect(hdc, &coords[m_snake[i].x][m_snake[i].y], headColor);
			}
			else {
				FillRect(hdc, &coords[m_snake[i].x][m_snake[i].y], bodyColor);
			}
		}
		snakeDir = RIGHT;
		
	}
	void move(HDC hdc, vector<vector<RECT>> coords, HWND hWnd, HBRUSH headColor, HBRUSH bodyColor, HBRUSH bkColor) {
		
		if (snakeDir == LEFT && m_snake[0].y <= 0) {
			m_snake[0].y = coords[0].size() - 1;
		}
		else if (snakeDir == RIGHT && m_snake[0].y >= coords[0].size() - 1) {
			m_snake[0].y = 0;
		}
		else if (snakeDir == UP && m_snake[0].x <= 0) {
			m_snake[0].x = coords.size() - 1;
		}
		else if (snakeDir == DOWN && m_snake[0].x >= coords.size() - 1) {
			m_snake[0].x = 0;
		}
		for (int i = 0; i < m_snake.size(); i++) {
			FillRect(hdc, &coords[m_snake[i].x][m_snake[i].y], bkColor);
			if (i == 0) {
				if (snakeDir == LEFT) {
					m_snake[i].y -= 1;
				}
				else if (snakeDir == RIGHT) {
					m_snake[i].y += 1;
				}
				else if (snakeDir == UP) {
					m_snake[i].x -= 1;
				}
				else if (snakeDir == DOWN) {
					m_snake[i].x += 1;
				}
				previous = m_snake[i];
			}
			else {
				temp = m_snake[i];
				m_snake[i] = previous;
				previous = temp;
				FillRect(hdc, &coords[m_snake[i].x][m_snake[i].y], bodyColor);
			}
		}
		FillRect(hdc, &coords[m_snake[0].x][m_snake[0].y], headColor);
	}
	void changeDirection(int direction) {
		switch (direction)
		{
		case 0:
			if (snakeDir != 1) {
				snakeDir = RIGHT;
			}
			break;
		case 1:
			if (snakeDir != 0) {
				snakeDir = LEFT;
			}
			break;
		case 2:
			if (snakeDir != 3) {
				snakeDir = UP;
			}
			break;
		case 3:
			if (snakeDir != 2) {
				snakeDir = DOWN;
			}
			break;
		default:
			break;
		}
	}
	POINT getHead(vector<vector<RECT>> coords) {
		return m_snake[0];
	}
	void add(HDC hdc, HBRUSH bodyColor, vector<vector<RECT>> coords) {
		m_snake.push_back(previous);
		temp = m_snake[m_snake.size()-1];
		m_snake[m_snake.size() - 1] = previous;
		previous = temp;
		FillRect(hdc, &coords[m_snake[m_snake.size() - 1].x][m_snake[m_snake.size() - 1].y], bodyColor);
	}
	bool collided(vector<vector<RECT>> coords, HDC hdc) {
		for (int i = 2; i < m_snake.size();i++) {
			if (m_snake[i].x == m_snake[0].x && m_snake[i].y == m_snake[0].y) {
				return true;
			}
		}
		return false;
	}
	vector<POINT> getSnake() {
		return m_snake;
	}
};

class Apple{
private:
	POINT m_apple;
public:
	void create(HDC hdc, vector<vector<RECT>> coords, HBRUSH appleColor, vector<POINT> snake) {
		m_apple.x = rand() % coords.size();
		m_apple.y = rand() % coords[0].size();
		for (int i = 0; i < snake.size();i++) {
			if (m_apple.x == snake[i].x && m_apple.y == snake[i].y) {
				create(hdc,coords,appleColor,snake);
			}
		}
			FillRect(hdc, &coords[m_apple.x][m_apple.y], appleColor);
	}
	bool checkEaten(POINT head, vector<vector<RECT>> coords, HDC hdc) {
		if (head.x == m_apple.x && head.y == m_apple.y) {	
			return true;
		}
		return false;
	}
	void paint(HDC hdc, vector<vector<RECT>> coords, HBRUSH appleColor) {
		FillRect(hdc, &coords[m_apple.x][m_apple.y], appleColor);
	}
};