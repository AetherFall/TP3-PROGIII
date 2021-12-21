#ifndef TP3_PRIORITYQUEUE_HPP
#define TP3_PRIORITYQUEUE_HPP

#include "PQNode.hpp"
#include <cstdlib>

template <typename T>
class PriorityQueue {
    private:
        PQNode<T>* first;
        size_t count;

    public:
        PriorityQueue() {
            this->first = nullptr;
            this->count = 0;
        }

        ~PriorityQueue() {
            while(this->first)
                pop();
        }

        /*
         * J'utilise le principe que 1 est la priorité la plus importante et ce qui vient après est moins important.
         */
        void push(T data, size_t priority) {
            if (this->first) {

                //Nouvel ajout est plus prioritaire
                if (priority < this->first->priority)
                    this->first = new PQNode<T>(data, priority, this->first);

                else {
                    PQNode<T>* temp = this->first;

                    while (temp->next && priority >= temp->next->priority)
                        temp = temp->next;

                    temp->next = new PQNode<T>(data, priority, temp->next);
                }
            }

            else
                this->first = new PQNode<T>(data, priority);

            count++;
        }

        void pop() {
            if(this->first) {
                PQNode<T>* temp = this->first;
                this->first = this->first->next;
                delete temp;

                count--;
            }
        }

        T front() { return (this->first) ? this->first->data : 0; }
        size_t getPriority() { return (this->first) ? this->first->priority : 0; }
        size_t size() { return count; }
};


#endif //TP3_PRIORITYQUEUE_HPP
