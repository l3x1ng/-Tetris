#include "lib/Subject.h"

namespace restonce {


// 사용 할 Observer를 멤버 변수에 저장하는 역할을 수행
void Subject::attachObserver(Observer *o)
{
    m_observers.insert(o);
}



// Subject class가 변화 할 때, observer를 호출하여, 변경 사항을 ui에 반영
void Subject::notifyObservers()
{
    for(Observer *o : m_observers)
    {
        o->onSubjectChanged();
    }
}

} 
