#include<iostream>
#include<cstdio>
#include<iomanip>
#include<string>
#include<random>
using namespace std;

/** Pass! **/

/** Complementary comments
 *
 * 8-8: You have an enormous memoryleak,
 * valgrind says this:
 * definitely lost: 19,923,952 bytes in 1,245,247 blocks
 * indirectly lost: 41,664 bytes in 2,604 blocks
 *
 * 8-8: You have to define, and implement the six special
 * member functions:
 * Constructor, Copy constructor, Copy assignment operator,
 * Move constructor, and Move operator.
 *
 * 2-1: Seperate definition and implementation, i.e.
 * implementation of Node methods should be in the
 * linked_list.cc file or another, sparate .cc file
 *
 **/

class Sorted_List{
public:
   Sorted_List(); // default constructor
   ~Sorted_List();
   Sorted_List(Sorted_List const &); // deep copy constructor
   Sorted_List(Sorted_List const &&); // Move constructor
   Sorted_List& operator=( const Sorted_List& other); //Copy assignment operator
   void dest_list();
   void copylist(Sorted_List other); // alternative way to copy list
   Sorted_List(int val); // instanciate list with one int value
   bool is_empty(); // checks whether list is empty
   int size(); // returns size of list
   void insert(int val); // insert value in list
   bool is_first_on_list(int val); // checks first value of list
   void print_list(); // print list in cout
   void remove(int val); // remove value in list (once)
   void get(int pos, int &val); // updates &val with value at position 'pos'
private:
    struct Link{
      int value;
      Link *next;
      //  insert subfunction
      Link * insert_s (Link *current, Link *insert);
      // remove subfunction
      Link* remove_s (Link *current,int val);
      void empty_list(Link *head);
    // print subfunction
      void print_list_s (Link *head);
    // subfunction for checking whether value is on the list
      bool is_on_the_list(int val);
    };
    Link* copy(Link* const other); // copy function
    Link* move(Link* const other); // Move operator
    Link *head{};
};
