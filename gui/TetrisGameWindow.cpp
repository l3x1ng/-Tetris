#include "TetrisGameWindow.h"
#include "ui_TetrisGameWindow.h"
#include "lib/TetrisGame.h"
#include "lib/RandomBox.h"
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>



// 프로그램이 시작되면, 해당 생성자가 작동한다.
// 테트리스 플레이를 위한 객체를 생성하고, ui를 그리기 위한 객체 및 변수를 초기화 함
TetrisGameWindow::TetrisGameWindow(QWidget *parent) : QWidget(parent), ui(new Ui::TetrisGameWindow)
{
    ui->setupUi(this);
    m_game = new restonce::TetrisGame;
    m_timer = new QTimer(this);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_timeout()));
    this->setFixedSize(this->size());
    m_game->attachObserver(this);
}



void TetrisGameWindow::slot_timeout(){ m_game->timeout(); }



// 프로그램이 종료 될 때 소멸자가 호출되며, 멤버 변수에 저장된 객체를 해제하는데 사용함
TetrisGameWindow::~TetrisGameWindow()
{
    delete ui;
    delete m_game;
}



// start 버튼을 클릿 헸을 때, 테트리스 게임이 시작 되도록 한다.
void TetrisGameWindow::on_pushButton_clicked()
{
    m_timer->start(1000);
    m_game->start();
}




// 게임 플레이를 위한 버튼, 떨어지는 블록, 게임 보드 등 ux/ui를 그린다.
void TetrisGameWindow::paintEvent(QPaintEvent *)
{

    // ↓ 게임 시작(start) | 멈춤 (pause) | 등의 버튼을 생성한다. 
    switch(m_game->getGameStatus())
    {
    case restonce::TetrisGame::GameStatus::runing:
        ui->label->setText("Start");
        ui->pushButton->setEnabled(false);
        break;
    case restonce::TetrisGame::GameStatus::stop:
        ui->label->setText("Pause");
        ui->pushButton->setEnabled(true);
        break;
    case restonce::TetrisGame::GameStatus::undo:
        ui->label->clear();
        ui->pushButton->setEnabled(true);
        break;
    }


    /// ↓ 게임 진행을 위한 게임 보드를 생성한다. 
    QPainter painter(this);
    QPoint p2(m_basePosition.x()+ m_boxSize*restonce::TetrisGame::ROW+1, m_basePosition.y() -1);
    QPoint p3(m_basePosition.x()-1, m_basePosition.y() +m_boxSize*restonce::TetrisGame::LINE+1);
    QPoint p4(m_basePosition.x() + m_boxSize*restonce::TetrisGame::ROW+1, m_basePosition.y() + m_boxSize*restonce::TetrisGame::LINE+1);
    QPoint p1(m_basePosition.x()-1, m_basePosition.y()-1);

    painter.drawLine(p1, p2);
    painter.drawLine(p2, p4);
    painter.drawLine(p4, p3);
    painter.drawLine(p1, p3);
    /// ↑ 게임 진행을 위한 게임 보드를 생성한다. 


    // ↓ 게임 보드의 각 칸을 순회하며, 각 칸에 착수된 블록이 있는지? 블록의 색은 무었인지? 확인한 후, 일치하는 색상의 블록을 생성함.
    for(int l=0; l<restonce::TetrisGame::LINE; ++l) {
        for(int r=0; r<restonce::TetrisGame::ROW; ++r) {
            QPoint p(m_basePosition.x() + r*m_boxSize, m_basePosition.y() + l*m_boxSize);

            int color = 0;

            // ↓ 게임 보드에 착수된 블록의 색을 가져온다.
            if(m_game->exists(l, r)) { color = m_game->color(l, r); } 
            

            // ↓ 현재 떨어지는 블록의 색상 정보를 가져온다.
            else if(m_game->getActiveBox() && m_game->getActiveBox()->inBody(l, r))
            {
                color = m_game->getActiveBox()->color();
            }

            if(color <= 0) continue;

            // ↓ 위 조건문으로 가져온 색상 정보를 이용해 일치하는 색상의 사진을 로드하여 ux/ui에 반영한다.
            QString imgpath = QString::asprintf(":/boxes/images/box%d.jpg", color);
            painter.drawImage(p, QImage(imgpath));
        }
    }


    // 다음에 떨어질 블록을 블록 대기 화면에 출력한다.
    std::shared_ptr<restonce::RandomBox> nextBox = m_game->getNextBox();
    if(nextBox) {
        QString imgpath = QString::asprintf(":/boxes/images/box%d.jpg", nextBox->color());

        for(restonce::Point const& p : nextBox->getMyBoxes()) {
            painter.drawImage(QPoint(m_baseNextPosition.x() +m_boxSize*p.row(), m_baseNextPosition.y() + m_boxSize*p.line()), QImage(imgpath));
        }
    }
}



// 방향키를 입력 했을 때, 블록의 위치와 모양을 변경한다.
// ← 좌측 이동 | → 우측 이동 | ↓ 블록 하강 | ↑ 블록 회전
void TetrisGameWindow::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Down:
        m_game->down();
        break;
    case Qt::Key_Left:
        m_game->left();
        break;
    case Qt::Key_Right:
        m_game->right();
        break;
    case Qt::Key_Up:
        m_game->transform();
        break;
    }
}



// 게임 진행 중, 변화를 탐지하여, 변경 사항을 ux/ui에 적용하는데 사용함
// (게임 로직에서 블록이 1칸 이동 할 때, 실제 ux/ui 에서도 1칸 이동한 블록의 모습을 보여주기 위해 사용)
void TetrisGameWindow::onSubjectChanged()
{
    repaint();
}
