#ifndef TETRISGAMEWINDOW_H
#define TETRISGAMEWINDOW_H

#include <QWidget>
#include <QPoint>
#include "lib/Subject.h"

namespace Ui {
    class TetrisGameWindow;
}
class QTimer;

namespace restonce {
    class TetrisGame;
}



//
// 테트리스 게임 진행을 위한 main class임 
// 테트리스를 위한 다른 class를 모두 이 class에 집약하여 하나의 프로그램을 구현함
class TetrisGameWindow : public QWidget, public restonce::Observer
{
    Q_OBJECT

public:

    //프로그램 시작 시, 테트리스 플레이 및 ux/ui 생성을 위한 객체를 생성하는 역할을 수행함
    explicit TetrisGameWindow(QWidget *parent = 0);

    // 프로그램이 종료될 때, 멤버 변수의 객체를 해제하는데 사용함 
    ~TetrisGameWindow();

protected:
    // 테트리스 게임의 (보드, 블록)을 렌더링 하는데 사용.
    void paintEvent(QPaintEvent *) override final;

    // 키보드 이벤트로 블록을 제어 할 때 사용.
    void keyPressEvent(QKeyEvent *) override final;

    // 블록 착수 등의 사건이 발생했을 때, 변경 사항을 화면에 반영해주는 메소드.
    virtual void onSubjectChanged() override final;


private slots:

    // 게임 시작 버튼을 구현하기 위한 메소드
    void on_pushButton_clicked();
    void slot_timeout();


private:
    Ui::TetrisGameWindow *ui;

    // 테트리스 게임의 조작 방법과 규칙을 구현하는 class임
    restonce::TetrisGame *m_game;
    int m_boxSize = 24;
    QPoint m_basePosition = QPoint(10, 10);
    QPoint m_baseNextPosition = QPoint(200, 240);
    QTimer *m_timer;
};

#endif 
