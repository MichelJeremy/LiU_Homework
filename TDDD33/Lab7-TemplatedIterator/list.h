#ifndef _LIST_H_
#define _LIST_H_

#include <iostream>
#include <string>


template <typename T>
class List
{
public:
   List();
  ~List() { delete first; }
  void insert(T const& d);
  List(List const&);
  List(List&&);
  List& operator=(List<T> const&);
  List& operator=(List<T>&&);

private:
  class Link
  {
    public:
      Link(T const& d, Link* n)
        : data(d), next(n) {}
      ~Link() { delete next; }
      T getData(){return data;}
      friend class List;

      static Link* clone(Link const*);

      template<typename T2>
      friend std::ostream& operator<<(std::ostream& out, const List<T2>& o);

    private:
      T data;
      Link* next;
  };
  friend class Iterator;
  friend class Link;
  Link* first;
public:
  using value_type = T;

  // The iterators constructor should be protected.
  // We want to create Iterators using only the public interface
  // begin / end.
    class Iterator {

      private:
        Iterator(Link* l) : current{l} {}
        friend class List;
        friend class Link;
        Link* current;
      public:
        Iterator()=delete;
        Iterator(const Iterator& t)=default;
        Link* getCurrent(){
          return current;
        }
        T operator* () const{     //dereferencing operator
          if (current!=nullptr) {
            return current->data; //It returns the contents
          }
        }
	// This works for the loop in the test.
        void operator++ () { //inc operator
          if (current!=nullptr) {
            current = current->next;
          }
        }
        void operator++ (int) {
            current = current->next;
        }
        bool operator!= (const Iterator& iterator) const{
            return current != iterator.current;
        }
    };

    Iterator begin() /*const*/{
      Iterator it{first};
      return  it;
    }
    Iterator end() /*const*/{
      Iterator it{nullptr};
      return it;
    }
};

#include "list.tcc"
#endif
