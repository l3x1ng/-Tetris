#ifndef RESTONCE_SUBJECT_H
#define RESTONCE_SUBJECT_H

#include <set>

namespace restonce {


// 게임 진행 중 테트리스 블록의 이동과 같은 변경 사항을 포착하여, 화면에 반영하기 위한 추상 class
// Subject의 변화를 감지하는데 사용됨
class Observer
{
protected:
    Observer() = default;
    virtual ~Observer() = default;
public:
    // 추상 class의 순수 추상 함수 이므로 상속 후 구현 필요
    virtual void onSubjectChanged() = 0;
};



// 항상 변화 할 수 있는 객체로 이 객체의 변화가 Observer에 탐지됨
class Subject
{
public :
    // 사용 할 Observer를 m_observers 멤버 변수에 저장하기 위한 역할
    void attachObserver(Observer *o);

protected:
    Subject() = default;
    virtual ~Subject() = default;

    // Subject class가 변화 할 때, observer를 호출하여, 변경 사항을 ui에 반영하는 메소드
    void notifyObservers() ;
private:

    // observer 객체를 저장하는 멤버 변수
    std::set<Observer *> m_observers;
};



} 

#endif
