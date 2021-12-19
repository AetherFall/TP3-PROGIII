#ifndef TP3_PRIORITYQUEUE_HPP
#define TP3_PRIORITYQUEUE_HPP

#include "PQNode.hpp"
#include <cstdlib>

template <typename T>
class PriorityQueue {
    private:
        SLNode<T>* first;
        size_t count;

    public:
        PriorityQueue() {
            this->first = nullptr;
            this->count = 0;
        }

        ~PriorityQueue() {
            while(first)
                pop();
        }

        void push(T data, size_t priority) {
            if (first) {
                if (first->priority > priority)
                    first = new PQNode<T>(data, first, priority);

                else {
                    PQNode<T>* temp = first;

                    while (temp->next != nullptr && temp->next->priority <= priority)
                        temp = temp->next;

                    runner->next = new PQNode<T>(data, runner->next, priority);
                }
            }

            else
                first = new PQNode<T>(data, priority);

            count++;
        }

        void pop() {
            if(first) {
                PQNode<T>* temp = first;
                first = first->next;
                delete temp;

                count--;
            }
        }

        T front() { return (first) ? first->data : 0; }

        size_t frontPriority() { return (first) ? first->priority : 0; }

        size_t size() { return count; }
};


#endif //TP3_PRIORITYQUEUE_HPP
