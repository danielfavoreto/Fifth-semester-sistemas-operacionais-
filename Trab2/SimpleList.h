#ifndef SIMPLE_LINKED_LIST_H
#define SIMPLE_LINKED_LIST_H

template <class T>
class List {
    class Node {
        public:
            Node() : obj(nullptr), next(nullptr) {}
            Node(T *o) : obj(o), next(nullptr) {}
            Node(const Node& n) : obj(n.obj), next(nullptr) {}
            ~Node() {
                if (nullptr != obj) {
                    delete obj;
                }
                if (nullptr != next) {
                    delete next;
                }
            }
            T *obj;
            Node *next;
    };

    Node *first;
    Node *last;
    Node *current;

    // Node to iterate over list
    Node *iterNode;

    unsigned int len;
    bool iter;

    // append node
    void appendNode(Node *n) {
        if (nullptr != n) {
            // as we are going to insert the node at the end
            // we can safely set ne next pointer to nullptr
            n->next = nullptr;
            if (nullptr != last) {
                last->next = n;
                last = n;
            } else {
                first = current = last = n;
            }
            len++;
        }
    }

    // prepend node
    void prependNode(Node *n) {
        if (nullptr != n) {
            if (first != nullptr) {
                n->next = first;
                first = n;
            } else {
                first = current = last = n;
            }
            len++;
        }
    }

    // swap node elements
    void swap(Node *a, Node *b) {
        T *o = a->obj;
        a->obj = b->obj;
        b->obj = o;
        return;
    }

    Node* getFirst() {
        return first;
    }

    Node *getLast() {
        return last;
    }

    Node* getCurrent() {
        return current;
    }

    // find object node
    Node *findObjectNode(T *o) {
        Node *tmp = first;
        while (nullptr != tmp) {
            if (*o == *(tmp->obj)) {
                break;
            }
            tmp = tmp->next;
        } 
        return *tmp;
    }

    // it's not a destructor it justs makes all pointers equals to nullptr
    // and length is set to zero
    // it this way we are safe to use temporary lists
    // it's a private function just to be used inside sorting functions
    void dummyClear() {
            first = current = last = nullptr;
            len = 0;
        }

    public:

        // default constructor
        List() : first(nullptr), last(nullptr), current(nullptr), iterNode(nullptr), len(0), iter(false) {}

        // default destructor
        ~List() {
            Node *tmp = first;
            while(nullptr != tmp) {
                Node *tmp2 = tmp;
                tmp = tmp->next;
                tmp2->next = nullptr;
                delete tmp2;
            }
        }

        unsigned int length() {
            return len;
        }

        // insert element at the end
        void append(T *o) {
            Node *n = new Node(o);
            appendNode(n);

            return;
        }

        // insert element at the beginning
        void prepend(T *o) {
            Node *n = new Node(o);

            prependNode(n);

            return;
        }

        // find and remove object from list
        void remove(T *o) {
            if (0 < len) {
                if (*(first->obj) == *o) { // first element
                    Node *tmp = first;

                    if (first == current) {
                        current = current->next;
                    }

                    if (1 == len && first == last) {
                        first = last = current = nullptr;
                    } else {
                        first = first->next;
                    }

                    tmp->next = nullptr;
                    delete tmp;
                    len--;
                } else if (1 < len) {
                    Node *tmp = first->next;
                    Node *tmp2 = first;
                    while (nullptr != tmp->next) {
                        if (*(tmp->obj) == *o) { // element found
                            tmp2->next = tmp->next;

                            if (last == tmp) {
                                last = tmp2;
                            }
                            if (current == tmp) {
                                current = tmp2;
                            }

                            tmp->next = nullptr;
                            delete tmp;
                            len--;
                            break;
                        }
                        tmp2 = tmp;
                        tmp = tmp->next;
                    }
                }
            }
            return;
        }

        // remove function overloading
        void removeElement(bool (*isthis)(T *o)) {
            if (0 < len) {
                if (isthis(first->obj)) { // first element
                    Node *tmp = first;

                    if (first == current) {
                        current = current->next;
                    }

                    if (1 == len && first == last) {
                        first = last = current = nullptr;
                    } else {
                        first = first->next;
                    }

                    tmp->next = nullptr;
                    delete tmp;
                    len--;
                } else if (1 < len) {
                    Node *tmp = first->next;
                    Node *tmp2 = first;
                    while (nullptr != tmp) {
                        if (isthis(tmp->obj)) { // element found
                            tmp2->next = tmp->next;

                            if (last == tmp) {
                                last = tmp2;
                            }
                            if (current == tmp) {
                                current = tmp2;
                            }

                            tmp->next = nullptr;

                            delete tmp;
                            len--;
                            break;
                        }
                        tmp2 = tmp;
                        tmp = tmp->next;
                    }
                }
            }
            return;

        }

        // if found, a will be replaced by b
        void replace(T *a, T *b) {
            Node *tmp = nullptr;
            if ((tmp = findObjectNode(a)) != nullptr) { // founded node
                T *n = tmp->obj;
                tmp->obj = b;
                delete n;
            }
            return;
        }

        // replace an element based on a callback function
        void replace(T *o, int (*isTarget)(T *a,T *b)) {
            Node *tmp = first;
            while (nullptr != tmp) {
                if (isTarget(o, tmp->obj) == 0) {
                    T *n = tmp->obj;
                    tmp->obj = o;
                    delete n;
                    break;
                }
                tmp = tmp->next;
            }
            return;
        }

        // custom iterator, it returns the object pointer
        // the client should use some loop and waits until this function
        // returns a nullptr (end of the list)
        T* iterator() {
            T *object = nullptr;
            if (0 < len) {
                if (!iter) { // not iterating
                    iter = true;
                    iterNode = first;
                }

                if (nullptr != iterNode) {
                    // get the object address!
                    object = iterNode->obj;
                    iterNode = iterNode->next;
                } else {
                    iter = false;
                    iterNode = first;
                }
            }
            return object;
        }

        // operator overloading
        // in this case we can't copy
        // we don't know nothing about the objects here
        void operator=(List l) {
            if (0 < len || nullptr != first) {
                clear();
            }
            if (0 < l.len) {
                concat(&l);
            }
            // we captured the pointers from the l list
            // so we'll just set the pointers to nullptr
            // in thi way we will not loose data when the l list is destroyed
            // the correct way of doing this should be throught real copy
            // but this list uses pointers to the element inside the nodes and we don't have enough info to call the copy constructor
            // The next task is to find a way to use virtual copy constructors - am I right?
            l.dummyClear();
        }

        // append element to the end
        void operator+=(T *o) {
            append(o);
            return;
        }

        // gets the current object and 
        // updates the current pointer to the next element
        T *operator++(int) {
            // reset iteration flag
            // to avoid conflicts with iterator()
            iter = false;

            if (current != nullptr) {
                T *object = current->obj;
                current = current->next;
                return object;
            } else {
                // end of the list
                current = first;
            }
            return nullptr;
        }
/*
 *  Just a simple quick sort implemented here
 *  it's commented because the document says we should do only 1 sorting function
 *  it's the same algorithm, but it uses the operator >
 *  to make this list works with primitive data types (ints, floats, chars) one may uncomment this block
        // a simple quick sort
        // the pivot is the first element
        // the sorting argument indicates if it's an ascending or descending
        // true == ascending
        void sort(bool sorting) {
            if (1 < len && first != last) { // 2 elements or more
                // auxiliar lists
                List lesser;
                List greater;

                // the pivot element is the first one
                Node *pivot = first;
                // iteration starts at the second element
                Node *tmp = first->next;

                Node *tmp2 = nullptr;

                pivot->next = nullptr;

                while(nullptr != tmp) {
                    tmp2 = tmp;
                    tmp = tmp->next;
                    if (sorting) {
                        if (*(tmp2->obj) < *(pivot->obj)) { // get all minor elements
                            lesser.appendNode(tmp2);
                        } else { // get all elements greater or equal to the pivot
                            greater.appendNode(tmp2);
                        }
                    } else {
                        if (*(tmp2->obj) < *(pivot->obj)) { // get all minor elements
                            lesser.appendNode(tmp2);
                        } else { // get all elements greater or equal to the pivot
                            greater.appendNode(tmp2);
                        }
                    }
                }

                // recursive call to lesser list
                lesser.sort();
                // and append the pivot element to the end
                lesser.appendNode(first);
                // recursive call to greater list
                greater.sort();

                // join the sublist
                // concat the greater list to the end of lesser list
                lesser.concat(&greater);

                // clear the greater list
                // it's not a destructor it justs makes all pointers equals to nullptr
                greater.dummyClear();

                // clear "this" list
                // it's not a destructor it justs makes all pointers equals to nullptr
                dummyClear();

                // concat the lesser list
                concat(&lesser);
                // clear the greater list
                // it's not a destructor it justs makes all pointers equals to nullptr
                lesser.dummyClear();

            }
            iter = false;
            return;
        }

        // just an overloaded version of sort
        // we rely on operators overloading
        void sort() {
            // we call the sorting function using true as the default option
            sort(true);
        }
 *
 * end of quick sort
*/
        // it's just the same sort function, just overloaded here
        // we use an ordinary function to compare the elements in this case
        // so we don't rely on operators overloading here! But it's the same generic function
        // the sorting argument indicates if it's an ascending or descending
        void sort(int (*compare)(T *a, T *b), bool sorting) {
            if (1 < len && first != last) { // 2 elements or more
                // auxiliar lists
                List lesser;
                List greater;

                // the pivot element is the first one
                Node *pivot = first;
                // iteration starts at the second element
                Node *tmp = first->next;

                Node *tmp2 = nullptr;

                pivot->next = nullptr;

                while(nullptr != tmp) {
                    // compare function returns: -1 if left hand object is the smallest,
                    // 0 if they are equal, and 1 if the right hand object is the smallest
                    // the sorting flag indicates if are going to ascending or descending
                    tmp2 = tmp;
                    tmp = tmp->next;
                    if (sorting) {
                        if (compare(tmp2->obj, first->obj) == -1) { // get all minor elements
                            lesser.appendNode(tmp2);
                        } else { // get all elements greater or equal to the pivot
                            greater.appendNode(tmp2);
                        }
                    } else {
                        if (compare(tmp2->obj, first->obj) == 1) { // get all minor elements
                            lesser.appendNode(tmp2);
                        } else { // get all elements greater or equal to the pivot
                            greater.appendNode(tmp2);
                        }
                    }
                }

                // recursive call to lesser list
                lesser.sort(compare, sorting);
                // and append the pivot element to the end
                lesser.appendNode(pivot);
                // recursive call to greater list
                greater.sort(compare, sorting);

                // join the sublist
                // concat the greater list to the end of lesser list
                lesser.concat(&greater);

                // clear the greater list
                // it's not a destructor it justs makes all pointers equals to nullptr
                greater.dummyClear();

                // clear "this" list
                // it's not a destructor it justs makes all pointers equals to nullptr
                dummyClear();

                // concat the lesser list
                concat(&lesser);
                // clear the greater list
                // it's not a destructor it justs makes all pointers equals to nullptr
                lesser.dummyClear();

            }
            iter = false;
            return;
        }

        // the same sort function above
        // it's just for convenience, so we can use the same function without the boolean sorting flag
        void sort(int (*compare)(T *a, T *b)) {
            // we call the sorting function using true as the default option
            // default == ascending
            sort(compare, true);
        }

        // concatenates two lists
        void concat(List *l) {
            if (0 < l->length() &&  nullptr != l->getFirst() && nullptr != l->getLast()) {
                if (0 < len && nullptr != last) {
                    last->next = l->getFirst();
                    last = l->getLast();
                } else {
                    first = current = l->getFirst();
                    last = l->getLast();
                }
                len += l->length();
            }
            return;
        }

        // clear the entire list
        // it calls the nodes destructors
        void clear() {

            Node *tmp = first;
            Node *tmp2 = nullptr;
            first = current = last = nullptr;
            len = 0;
            while (nullptr != tmp) {
                tmp2 = tmp;
                tmp = tmp->next;
                delete tmp2;
            }
        }

        // returns a sublist based on a selection function
        // the selection function must copy the object
        List sublist(T* (*selection)(T *a)) {
            List nl;
            if (0 < len) { // not an empty list
                Node *tmp = first;
                while (nullptr != tmp) {
                    T *nobj = nullptr;
                    if ((nobj = selection(tmp->obj)) != nullptr ) {
                        nl.append(nobj);
                    }
                    tmp = tmp->next;
                }
            }
            return nl;
        }

        // returns a pointer to an element inside the list, based on an index
        // it just consider the sequence of the elements
        // if the index is out of range, return a null pointer
        T* at(long int index) {
            if (0 < index) {
                Node *tmp = first;
                int i = 1;
                while(nullptr != tmp && i < index) {
                    tmp = tmp->next;
                    i++;
                }
                if (tmp != nullptr) {
                    return tmp->obj;
                }
            }
            return nullptr;
        }

        // get the first element from the list
        // and remove it
        T* poll() {
            T *o = nullptr;
            if (nullptr != first) {
                o = first->obj;
                Node *tmp = first;
                if (current == first) {
                    current = first->next;
                    first = first->next;
                } else {
                    first = first->next;
                }
                if (last == tmp) {
                    last = first;
                }
                tmp->next = nullptr;
                tmp->obj = nullptr;
                len--;
                delete tmp;
            }
            return o;
        }
};
#endif