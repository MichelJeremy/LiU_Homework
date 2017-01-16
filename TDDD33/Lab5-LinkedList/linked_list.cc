#include<iostream>
#include<cstdio>
#include<iomanip>
#include "linked_list.h"


using namespace std;

/* instanciate a new empty list */
Sorted_List::Sorted_List(){
  head =nullptr;
};


// Copy operator
Sorted_List& Sorted_List::operator=( const Sorted_List& other ) {
      head = copy(other.head);
      return *this;
  }


Sorted_List::~Sorted_List(){
  //head->empty_list(head);
  int val;
  //cout << "Size before: " << size() << std::endl;
  for (int i = size()-1; i >= 0; i--) {
    get(i, val);
    cout << val << " " << i <<endl;
    remove(val);
  }
  //cout << "Size after: " << size() << std::endl;

}

/* instanciate a new sorted list with one int value*/
Sorted_List::Sorted_List(int val){
  Link *insert;
  insert = new struct Link;
  insert -> value = val;
  insert -> next = nullptr;
  head = head->insert_s(nullptr,insert);
}

/*
 * checks whether the list is empty or not
 * returns true if list is empty, false otherwise
 */
bool Sorted_List::is_empty(){
  return (head == nullptr);
}

/* print the list on cout*/
void Sorted_List::print_list(){
    head->print_list_s(head);
}

/* checks whether value is first on list*/
bool Sorted_List::is_first_on_list(int val){
  return head->value == val;
}

/*
 * gets the value located at the 'pos' position
 * and updates the int& val parameter with the value at this position
 */
void Sorted_List::get(int pos, int &val) {
  Link *it{head};
  int act_pos = 0;

  while (it != nullptr) {
    if (pos == act_pos) {
      val = it->value;
      break;
    } else {
      it = it->next;
      act_pos++;
    }
  }
}

/* returns the size of the list */
int Sorted_List::size(){
  int size{0};
  Link *it{head};
  while(it != nullptr)
  {
    it = it->next;
    size ++;
  }
  return size;
}

/* insert a value 'val' in the list while sorting it*/
void Sorted_List::insert(int val){
  Link *insert;
  insert = new struct Link;
  insert -> value = val;
  insert -> next = nullptr;

        head = head->insert_s(head,insert);
}

/*deep copy the list to another one while instanciating it*/
Sorted_List::Sorted_List(Sorted_List const & other) : head{copy(other.head)}{}

/*copy definition for deep copy*/
Sorted_List::Link* Sorted_List::copy(Sorted_List::Link* const other){
    if (other == nullptr) {
      return nullptr;
    }
    return new Link{other->value,Sorted_List::copy(other->next)};
}


/* alternative way to copy list if it is already instanciated */
void Sorted_List::copylist(Sorted_List other){
    dest_list();
    head=copy(other.head);
}


//Move constructor
Sorted_List::Sorted_List(Sorted_List const && other){
    dest_list();
    head=copy(other.head);
    other.~Sorted_List();
}


Sorted_List::Link * Sorted_List::Link::insert_s(Link *current, Link *insert){
  // if nullptr, we are at the end of the list
  if (current == nullptr){
    return insert;
  }
  else{
    // checks next value, if greater then we are at the right position
    // so insert here
    if(current-> value > insert->value){
      insert->next = current;
      return insert;
    }
    else{
        if (current->next !=nullptr){
          if (current->next->value > insert->value) {
            insert->next = current->next;
            current->next = insert;
            return current;
          }
          else{
              current ->next = insert_s(current->next,insert);
          }
        }
        else{
          current->next = insert;
          return current;
        }
      //current ->next = insert_s(current->next,val);
    }}
  }

  Sorted_List::Link* Sorted_List::Link::remove_s (Link *current,int val){
    Link *to_remove;
    if (current != nullptr){
      // if current is not nullptr and next value is equal to the value we want
      // to remove, remove it
      if (current->value == val) {
        to_remove = current;
        current = current->next;
        to_remove -> next = nullptr;
        delete to_remove;
        return current;
      }
      else{
        // go to next value
        current ->next = remove_s(current->next, val);
        return current;
      }
    }
    else{
      return current;
  }
  }
void Sorted_List::Link::print_list_s (Link *head){
      if (head != nullptr) {
        cout << head->value <<endl;
        return print_list_s(head->next);
      }
  }

bool Sorted_List::Link::is_on_the_list(int val){
      if(value == val)
      {
        return true;
      }
      else
      {
        if(next != nullptr)
        {
          return next->is_on_the_list(val);
        }
        return false;
      }
  }
void Sorted_List::Link::empty_list(Link *head){
    Link *to_remove;
    while (head !=nullptr) {
      to_remove = head;
      head = head->next;
      to_remove->next = nullptr;
      delete to_remove;
    }
  }
/* removes an element 'val' in the list */
void Sorted_List::remove(int val){
  head = head->remove_s(head,val);
}

void Sorted_List::dest_list(){
  int val;
  //cout << "Size before: " << size() << std::endl;
  for (int i = size()-1; i >= 0; i--) {
    get(i, val);
    cout << val << " " << i <<endl;
    remove(val);
  }
}
