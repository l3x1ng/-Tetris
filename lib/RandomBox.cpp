#include "RandomBox.h"
#include "TetrisGame.h"

namespace restonce {


// 셀 이라고 불리는 □ 가 위치 해야 할 좌표 값을 이용해 객체를 생성한다.
Point::Point(int line, int row): m_line(line), m_row(row){}


// y죄표 값을 반환함
int Point::line() const { return m_line; }


// x 죄표 값을 반환한다.
int Point::row() const { return m_row; }


// y 좌표 값을 설정한다.
// 주로 블록이 떨어질 때, 다음 좌표 값을 설정 할 목적으로 사용
void Point::setLine(int line) { m_line = line; }


// x 좌표 값을 설정한다.
// 주로 블록을 좌우로 이동 할 때, 다음 좌표 값을 설정 할 목적으로 사용
void Point::setRow(int row) { m_row = row; }



struct Box {
    Point boxes[4];
    int next;
} ;


static Box dbs[] = {

//
//// 이 아래의 숫자 배열은 전부 블록의 위치를 저장한 좌표임
//// 4개의 네모가 모여 하나의 블록을 만들기 때문에 4개의 좌표 값이 있으며, 맨 우측 숫자는 회전 시켰을 때의 모양임


// ↓ ┓ 모양 블록이며, 회전 시 매번 모양이 변화하므로 4개의 좌표 값을 가진다.
{{{0, 0}, {1, 0}, {1, 1}, {1, 2}},1},
{{{2, 0}, {0, 1}, {1, 1}, {2, 1}},2},
{{{1, 0}, {1, 1}, {1, 2}, {2, 2}},3},
{{{0, 1}, {1, 1}, {2, 1}, {0, 2}},0},


// ↓ ┗ 모양 블록이며, 회전 시 매번 모양이 변화하므로 4개의 좌표 값을 가진다.
{{{1, 0}, {2, 0}, {1, 1}, {1, 2}},5},
{{{0, 1}, {1, 1}, {2, 1}, {2, 2}},6},
{{{1, 0}, {1, 1}, {0, 2}, {1, 2}},7},
{{{0, 0}, {0, 1}, {1, 1}, {2, 1}},4},


// ↓ ─ 모양 블록이며, 회전 시 모양이 한 번 변화하므로 2개의 좌표 값을 가진다.
{{{1, 0}, {1, 1}, {1, 2}, {1, 3}},9}, 
{{{0, 1}, {1, 1}, {2, 1}, {3, 1}},8},


// ↓ z 모양 블록 좌표이며, 회전 시 모양이 한 번 변화하므로 2개의 좌표 값을 가진다.
{{{0, 0}, {1, 0}, {1, 1}, {2, 1}},11}, // 10, z
{{{2, 0}, {1, 1}, {2, 1}, {1, 2}},10},


// ↓ z 모양 블록 좌표이며, 회전 시 모양이 한 번 변화하므로 2개의 좌표 값을 가진다.
{{{1, 0}, {2, 0}, {0, 1}, {1, 1}},13},  
{{{1, 0}, {1, 1}, {2, 1}, {2, 2}},12},


// ↓ □ 모양 블록 좌표이며, 회전해도 모양이 바뀌지 않으므로 하나의 좌표 값만 존재함
{{{0, 0}, {1, 0}, {0, 1}, {1, 1}},14}, 


// ↓ ┰ 모양 블록이며, 회전 시 매번 모양이 변화하므로 4개의 좌표 값을 가진다.
{{{1, 0}, {0, 1}, {1, 1}, {2, 1}},16}, 
{{{0, 0}, {0, 1}, {1, 1}, {0, 2}},17},
{{{0, 0}, {1, 0}, {2, 0}, {1, 1}},18},
{{{1, 0}, {0, 1}, {1, 1}, {1, 2}},15},
};



// 난수를 이용해 랜덤하게 블록을 생성한다.
RandomBox::RandomBox(TetrisGame &game_, std::mt19937 &rd): m_game(game_), m_basicPosition(0, 3)
{
    // ↓ 난수를 이용해 위 에서 정의한 블록 좌표 중 하나를 랜덤하게 선택한다.
    m_boxpos = rd() % (sizeof(dbs)/sizeof(dbs[0]));
    m_color = 1 +(rd() % 17);
}


// 특정 좌표에 블록이 놓여져 있는지 확인한다. 만약 블록이 놓여져있지 않다면 true
bool RandomBox::valid()
{
    for(Point const& p: getMyBoxes()) {
        if(!m_game.valid(p.line(), p.row()) || m_game.exists(p.line(), p.row())) {
            return false;
        }
    }
    return true;
}


// 블록을 한 칸 아래로 이동시킨다. 
bool RandomBox::down()
{
    for(Point const& p : getNextBoxes(1, 0)) {

        // ↓ 만약 한 칸 아래에 이미 블록이 존재하거나, 게임 보드의 가장 아래라면, 하강 시키지 않고, 블록을 착수한다.
        if(!m_game.valid(p.line(), p.row()) ||m_game.exists(p.line(), p.row())) {
            this->set();
            return false;
        }
    }

    // ↓ 아래로 한 칸 하강
    m_basicPosition.setLine(m_basicPosition.line() +1);
    return true;
}



// 블록을 한칸 왼쪽으로 이동시킨다. 만약 블록이 게임 보드의 왼쪽 끝에 있다면, 이동시키지 않는다. 
bool RandomBox::left()
{
    for(Point const& p : getNextBoxes(0, -1)) {
        
        // ↓ 블록 위치가 보드의 왼쪽 끝인지 확인한다.
        if(!m_game.valid(p.line(), p.row()) || m_game.exists(p.line(), p.row())) {
            return false;
        }
    }

    // ↓ 왼쪽으로 이동
    m_basicPosition.setRow(m_basicPosition.row() -1);
    return true;
}



// 블록을 한칸 오른쪽으로 이동시킨다. 만약 블록이 게임 보드의 오른쪽 끝 이라면, 이동시키지 않는다.
bool RandomBox::right()
{
    for(Point const& p : getNextBoxes(0, 1)) {

        // ↓ 블록의 위치가 오른쪽 끝인지 확인한다.
        if(!m_game.valid(p.line(), p.row()) || m_game.exists(p.line(), p.row())) {
            return false;
        }
    }

    // ↓ 오른쪽으로 이동
    m_basicPosition.setRow(m_basicPosition.row()+1);
    return true;
}



// 블록을 회전 시킨다. 만약 블록이 바닥에 닿았다면, 회전 시키지 않는다. 
bool RandomBox::transform()
{
    for(Point const& p : getNextBoxes()) {
        
        // ↓ 블록이 바닦에 닿는지 확인
        if(!m_game.valid(p.line(), p.row()) || m_game.exists(p.line(), p.row())) {
            return false;
        }
    }

    // 회전한 블록의 좌표값 적용
    m_boxpos = dbs[m_boxpos].next;
    return true;
}



// 블록의 상대좌표를 이용해 실제 게임 보드에서 블록이 보여져야 할 위치를 계산하여 반환한다.
// 블록이 현재 위치해야 할 좌표를 계산하는데 사용된다.
std::vector<Point> RandomBox::getMyBoxes() const
{
    std::vector<Point> mypoints;

    for(Point const& p : dbs[m_boxpos].boxes) {
        mypoints.push_back(Point(m_basicPosition.line() +p.line(), m_basicPosition.row() + p.row()));
    }
    return mypoints;
}



// 현재 떨어지고 있는 블록의 다음 좌표를 계산한다.
// 블록이 이동 할 위치 좌표를 미리 계산하여, 블록이 실제로 이동 가능 한지? 테스트 할 때 사용한다. 
std::vector<Point> RandomBox::getNextBoxes(int line_inc, int row_inc) const
{
    std::vector<Point> nextBoxes;

    for(Point const& p : getMyBoxes()) { 
        nextBoxes.push_back(Point(p.line()+line_inc, p.row() + row_inc));
    }
    return nextBoxes;
}



// 위 함수와 동일한 이름의 오버로딩 함수로 해당 함수는 회전한 블록의 좌표를 미리 계산하는데 사용된다.
// 해당 메소드로 좌표 값을 얻어 실제로 회전이 가능한지 테스트 하는데 사용된다. 
std::vector<Point> RandomBox::getNextBoxes() const
{
    std::vector<Point> nextBoxes;

    for(Point const& p : dbs[ dbs[m_boxpos].next ].boxes) {
        nextBoxes.push_back(Point(p.line()+m_basicPosition.line(), p.row() + m_basicPosition.row()));
    }
    return nextBoxes;
}



// 블록의 좌표를 저장 할 때 사용한다.
// 블록이 땅에 닿아서, 더 이상 움직 일 수 없을 때, 블록의 위치를 저장 할 목적으로 사용함 
void RandomBox::set()
{
    for(Point const& p : getMyBoxes()) {
        m_game.set(p.line(), p.row(), m_color);
    }
}


// 블록의 색을 반환한다.
int RandomBox::color() const
{
    return m_color;
}



bool RandomBox::inBody(int line, int row) const
{
    for(Point const& p : getMyBoxes()) {
        if(p.line() == line && p.row() == row) {
            return true;
        }
    }
    return false;
}

} 
