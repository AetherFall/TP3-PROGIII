#ifndef TP3_SLNODE_HPP
#define TP3_SLNODE_HPP

template <typename T>
class PQNode {
    public:
        T data;
        PQNode<T>* next;
        size_t priority;

        PQNode(T data, SLNode<T>* next = nullptr, size_t priority) {
            this->data = data;
            this->next = next;
            this->priority = priority;
        }
};

#endif
