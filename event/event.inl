template<typename MSGTYPE>
std::map<std::weak_ptr<Event>,
    typename Observer<MSGTYPE>::EventObserverList,
    typename Observer<MSGTYPE>::Complate> Observer<MSGTYPE>::event_observer_list_map_;
template<typename MSGTYPE>
std::mutex Observer<MSGTYPE>::map_mutex_;
