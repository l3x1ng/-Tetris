#include "TetrisGame.h"
#include "RandomBox.h"
#include <time.h>

namespace restonce {




// 테트리스 프로그램이 실행 될 때, 초기 상태를 세팅함
// 초기상태 = ( 블록 없이 빈 보드, 게임 상태 undo, lose) 등
TetrisGame::TetrisGame()
{
    // ↓ 게임 상태 변수 값 세팅
    m_gameStatus = GameStatus::undo;
    m_winStatus  = WinStatus::lose;

    m_rd.seed(time(NULL));
    for(int l = 0; l < LINE; ++l) {
        for(int r = 0; r < ROW; ++r) {
            m_map[l][r] = false;   // ← 블록 없이 빈 보드를 세팅함
        }
    }
}




// 게임을 플레이하기 전, 이전 플레이 흔적을 지우고, 상태 정보를 원상복구한 뒤, 
// 게임에 사용될 첫 번째 블록과 그 다음 블록을 생성하는 메소드
void TetrisGame::init()
{
    // ↓ 이전 플레이 흔적 정리
    for(int l=0; l<LINE; ++l) {
        for(int r=0; r<ROW; ++r) {
            m_map[l][r] = false;
            m_colorMap[l][r] = 0;
        }
    }

    // ↓ 게임 상태 변수 값 원상복구
    m_gameStatus = GameStatus::undo;
    m_winStatus  = WinStatus::lose;

    // ↓ 게임 시작 시 떨어지는 첫 번째 블록과 두 번째 블록을 생성한다.
    m_activebox  = std::make_shared<RandomBox>(*this, m_rd);
    m_nextBox    = std::make_shared<RandomBox>(*this, m_rd);
}




// 위의 init 메소드를 이용해 게임 플레이를 위한 초기 세팅을 마친 후, 상태 변수를 runing(게임 진행 중)으로 변경한다. 
void TetrisGame::start()
{
    if(m_gameStatus == GameStatus::runing) {
        throw std::logic_error("Game is runing !");
    }

    init(); // 게임 플레이를 위한 초기 세팅 및 블록 생성

    // 게임의 진행 상태를 runing으로 변경
    m_gameStatus = GameStatus::runing;
    notifyObservers();
}





// 시간 경과에 따른 블록 하강을 구현하는 메소드
// 블록의 하강을 시도해 보고, 하강 불가능 하다면, 블록을 해당 위치에 고정함
// 블록의 위치가 고정되었을 때, 지울 수 있는 라인이 생기면, 해당 라인을 삭제함.
void TetrisGame::timeout()
{
    if(m_gameStatus != GameStatus::runing) {return;}

    // 블록 하강 시도 (하강에 실패하면 if문 내부로 분기함)
    if(!m_activebox->down()) {
        
        // ↓ 하강에 실패한 경우 블록이 놓여진 라인을 순회하며, 모든 라인에 블록이 존재하는지 확인한다. 
        for(int line=LINE-1; line>=0; --line) {
            bool isFull = true;
            for(int row=0; row<ROW; ++row) {
                if(!this->exists(line, row)) {
                    isFull = false;
                    break;
                }
            }

            // ↓ 위에서 수행한 순회 결과 모든 라인에 블록이 존재한다면, 해당 라인의 블록을 삭제함
            if(isFull) {
                for(int l=line; l>=0; --l) {
                    for(int r=0; r<ROW; ++r) {
                        if(l ==0) {
                            m_map[l][r] = false;
                        } else {
                            m_map[l][r] = m_map[l-1][r];
                        }
                    }
                }
                ++ line;
            }
        }
        
        // ↓ 현자 사용중인 블록을 착수 했으므로 다음에 떨어질 블록을 세팅하고, 그 뒤에 사용될 새로운 블록을 생성한다.
        m_activebox = m_nextBox;
        m_nextBox=std::make_shared<RandomBox>(*this, m_rd);
        
        
        if(!m_activebox->valid()) {    
            m_gameStatus = GameStatus::stop;
            m_winStatus = WinStatus::lose;
        }
    }

    notifyObservers();
}



// 블록을 회전 시키는데 사용된다. 
void TetrisGame::transform()
{
    if(m_activebox->transform()) {
        notifyObservers();
    }
}


// 블록을 아래로 떨어뜨리는데 사용된다.
void TetrisGame::down(){ timeout(); }



// 블록을 왼쪽으로 이동 시키는데 사용된다.
void TetrisGame::left()
{
    if(m_activebox->left()) {
        notifyObservers();
    }
}


// 블록을 오른쪽이로 이동 시키는데 사용된다.
void TetrisGame::right()
{
    if(m_activebox->right()) {
        notifyObservers();
    }
}



// 게임을 잠시 멈추는데 사용된다.
void TetrisGame::stop()
{
    if(m_gameStatus == GameStatus::runing) {
        m_gameStatus = GameStatus::stop;
        m_winStatus = WinStatus::lose;
        notifyObservers();
    }
}



// 게임의 진행 상태를 받는데 사용한다.
// (게임 진행 중에만 호출 되어야 할 함수에 사용함)
TetrisGame::GameStatus TetrisGame::getGameStatus() const
{
    return m_gameStatus;
}




TetrisGame::WinStatus TetrisGame::getWinStatus() const
{
    if(m_gameStatus != GameStatus::stop) {
        throw std::logic_error("Game is not stop !");
    }
    return m_winStatus;
}



// 매개변수 ( line = 블록의 y축 위치  |  row = 블록의 x 축 위치 )
// 블록이 플레이 보드를 벗어나는지 확인한다. (블록 위치 유효성 검사용)
bool TetrisGame::valid(int line, int row) const
{
    return line >=0 && line < LINE && row >=0 && row < ROW;
    // 플레이 보드를 벗어나지 않으면 true, 벗어나면 false
}




// 매개변수 ( line = 블록의 y축 위치  |  row = 블록의 x 축 위치 )
// 특정 위치에 이미 놓여진 블록이 있는지 확인한다.  
bool TetrisGame::exists(int line, int row) const
{
    if(!valid(line, row)) {
        throw std::out_of_range("Game position not exists !");
    }

    // 블록이 놓여져 있는경우 true가 반환되며, 그렇지 않다면 false가 반환된다. 
    return m_map[line][row];
}




// 매개변수 ( line = 블록의 y축 위치  |  row = 블록의 x 축 위치 )
// 특정 위치에 어떤 색의 블록이 놓아져있는지 저장하는 역할 수행 (게임 보드에 블록을 그리기 위해 사용)
int TetrisGame::color(int line, int row) const
{
    return m_colorMap[line][row];
}


// 현재 떨어지고 있거나, 떨어뜨려야 할 블록을 반환함
std::shared_ptr<RandomBox> TetrisGame::getActiveBox() const
{
    return m_activebox;
}


// 블록의 위치와 색상 정보를 저장하는 역할을 수행함 
void TetrisGame::set(int line, int row, int color)
{
    m_map[line][row] = true;
    m_colorMap[line][row] = color;
}


// 다음에 사용될 블록을 반환함
std::shared_ptr<RandomBox> TetrisGame::getNextBox() const
{
    return m_nextBox;
}

}
