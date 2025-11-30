#ifndef RESTONCE_RANDOMBOX_H
#define RESTONCE_RANDOMBOX_H

#include <vector>
#include <random>

namespace restonce {
class TetrisGame;



// 테트리스 블록을 구성하는 네모 칸 하나 하나를 표현하기 위한 class
// □□□□ ← 이런 블록이면 4개의 Point class로 이루어짐 
class Point{
public :
    // □ Point가 위치해야 할 좌표 값을 이용해 객체를 생성하는 생성자 
    Point(int line, int row);

    // point의 y좌표 값을 반환하는데 사용 
    int line() const;  

    // point의 x좌표 값을 반환하는데 사용
    int row() const;  

    // point의 y 좌표 값을 재설정 하며, 주로 블록이 떨어질 때, y좌표 값을 변경하기 위해 사용
    void setLine(int line);

    // point의 x 좌표 값을 재설정 하며, 주로 블록이 좌우로 이동 할 때, x 좌표 값을 변경하기 위해 사용
    void setRow(int row);
private:

    // y      , x      좌표 값을 담는 변수
    int m_line, m_row;
};



// Point class를 모아 하나의 블록을 만들고, 해당 블록을 제어하기 위한 class
class RandomBox{
public:
    // 랜덤한 모양의 블록을 생성하는 생성자임
    RandomBox(TetrisGame& game_, std::mt19937& rd);
    
    bool valid();
    
    // 블록을 회전 시키는 메소드임
    bool transform();

    // ↓ 블록의 위치를 이동 시킬 때 사용하는 메소드임
    bool down();
    bool left();
    bool right();
    
    bool inBody(int line, int row) const;
    
    // 블록이 땅에 닿았을 때, 해당 블록의 위치 정보를 저장하기 위한 메소드임
    void set();

    // 블록의 색상 정보를 반환하기 위한 메소드임
    int color() const;
    
    // 블록이 현재 어느 위치에 그려져야 하는지 블록의 좌표 값을 반환하는 메소드임
    std::vector<Point> getMyBoxes() const;
private:

    // 블록이 이동 할 때, 작동하는 메소드임
    // 블록이 상하좌우로 이동 하기 전, 블록의 이동 위치를 미리 계산하여, 실제로 이동 가능한지? 테스트 해보는 목적의 메소드
    std::vector<Point> getNextBoxes(int line_inc, int row_inc) const;

    // 블록이 회전 할 때, 작동하는 메소드임
    // 블록이 회전 하기 전, 블록의 회전 좌표를 미리 계산하여 반환 함, 좌표 값을 이용해 블록을 회전 시킬 수 있는 상황인지 판단하는데 사용.
    std::vector<Point> getNextBoxes() const;
private:

    // ↓ 게임 보드에 어떤 블록이 놓여져 있는지 블록의 위치 정보를 기록하는 변수
    TetrisGame& m_game;

    // ↓ 블록의 상태 좌표,(블록의 모양을 좌표 값으로 저장하는 변수)
    Point m_basicPosition;
    
    // ↓ 블록의 회전 상태를 저장하는 변수
    int m_boxpos; 

    // ↓ 블록의 색
    int m_color;
};


}

#endif 
