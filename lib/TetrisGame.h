#ifndef RESTONCE_TETRISGAME_H
#define RESTONCE_TETRISGAME_H

#include <memory>
#include <random>
#include "Subject.h"

namespace restonce {

class RandomBox;


// 테트리스 게임의 조작 방법과 규칙을 정의한 class임.
class TetrisGame : public Subject
{
public:

    // 사용자의 게임 진행 상태를 알리는 열거형임.
    enum class GameStatus {
        undo,   // 게임을 하지 않을 떄
        runing, // 게임을 하고 있을 때
        stop    // 게임을 멈추었을 때
    };

    // 게임 결과를 표현하는 열거형, 
    // 테트리스 특성 상 lose만 사용됨
    enum class WinStatus {
        win, 
        lose
    };

    // 테트리스 게임보드의 크기
    // 가로 10칸, 세로 20칸
    enum {
        ROW = 10,
        LINE = 20
    };

    // 생성자이며, 테트리스 보드를 빈 상태로 생성하는 로직이 구현됨.
    TetrisGame();
    
    // 게임이 종료 되었거나 실행되기 전 상테에서만 버튼으로 호출 가능, 메소드 호출시 게임 시작
    void start();

    // 시간에 따라 블록이 떨어지고, 줄을 삭제하는 등의 로직이 구현됨
    void timeout();

    // 블록을 회전 시킬 때, 화명을 갱신하기 위해 사용하는 메소드임
    void transform();

    // 이 아래 3개의 메소드는 블록을 이동 시킬 때 호출하는 메소드임 
    void down();
    void left();
    void right();
    
    // 게임을 잠시 멈추는 기능을 수행하는 메소드임
    void stop();
    
    // 블록을 착수 할 위치에 다른 블록이 존재하는지 확인하는 메소드임.
    bool exists(int line, int row) const;

    // 블록의 생상을 모니터링 하여, 게임 보드에 정확한 그래픽을 그려내는데 사용함.
    int color(int line, int row) const;

    // 블록 위치 정보를 계산하여, 정확한 위치에 블록을 착수 시킬 수 있도록 함
    bool valid(int line, int row) const;

    // 착수한 블록의 위치와 색상을 저장하는데 사용함.
    void set(int line, int row, int color);

    GameStatus getGameStatus() const;
    WinStatus getWinStatus() const;

    // 현재 떨어지고 있는 블록을 반환하는 메소드임
    std::shared_ptr<RandomBox> getActiveBox() const;

    // 다음에 사용 될 블록을 반환하는 메소드임
    std::shared_ptr<RandomBox> getNextBox() const;
private:

    // 게임을 시작 할 때, start 메소드 이 전 호출됨
    // 게임 진행을 위해 보드를 지우고, 첫 블록과 다음 블록을 생성하는 역할
    void init();

private:
    // 현재 게임의 진행 상태를 저장하는 변수임.
    GameStatus m_gameStatus;

    // 게임 종료 후, 게임 결과를 저장하는 변수(win lose)
    WinStatus m_winStatus;

    // 게임 보드에 착수된 블록의 위치정보를 저장하는 변수
    bool m_map[LINE][ROW] ;

    // 게임 보드에 착수된 블록의 색상 정보를 저장하는 변수
    int  m_colorMap[LINE][ROW] ;

    // 현재 화면에서 떨어지고 있는 블록을 저장하는 변수
    std::shared_ptr<RandomBox> m_activebox, m_nextBox;
    
    // 랜덤한 블록을 생성하기 위해 사용되는 난수 생성기임
    std::mt19937 m_rd;
};

} 

#endif 
