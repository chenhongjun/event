template<typename MSGTYPE>
std::mutex Observer<MSGTYPE>::mutex_;
template<typename MSGTYPE>
std::list<std::weak_ptr<Observer<MSGTYPE>>> Observer<MSGTYPE>::observer_list_;