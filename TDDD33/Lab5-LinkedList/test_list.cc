// This test program uses a test framework supporting TDD and BDD.
// You are not required to use the framework, but encouraged to.
// Code:
// https://github.com/philsquared/Catch.git
// Documentation:
// https://github.com/philsquared/Catch/blob/master/docs/tutorial.md
// You ARE however required to implement all test cases outlined here,
// even if you do it by way of your own function for each case.  You
// are recommended to solve the cases in order, and rerun all tests
// after you modify your code.
// This define lets Catch create the main test program
// (Must be in only one place!)
#define CATCH_CONFIG_MAIN
#include "Catch/include/catch.hpp"
#include "linked_list.h"
#include <random>

//=======================================================================
// Test cases
//=======================================================================



TEST_CASE( "Create an empty list" ) {
  Sorted_List l{};


  REQUIRE( l.is_empty() == true );
  REQUIRE( l.size() == 0 );
  l.insert(1);
  l.insert(2);
  Sorted_List l2(move(l));
  l2.print_list();
  REQUIRE( l.is_empty() == true );  // move constructor tested
}




// Solve one TEST_CASE or WHEN at a time!
//
// Move this comment and following #if 0 down one case at a time!
// Make sure to close any open braces before this comment.
// The #if 0 will disable the rest of the file.
TEST_CASE( "Insert an item in an empty list" ) {
  Sorted_List l{};
  l.insert(5);
  REQUIRE( l.is_empty() == false );
  REQUIRE( l.size() == 1 );
  //l.dest_list();
}


SCENARIO( "Empty lists" ) {

  GIVEN( "An empty list" ) {
    Sorted_List l{};

    REQUIRE( l.is_empty() == true );
    REQUIRE( l.size() == 0 );

    WHEN( "an item is inserted" ) {
      // insert an item
      l.insert(5);
      THEN( "the size increase and the item is first in the list" ) {
        REQUIRE( l.is_empty() == false );
        REQUIRE( l.size() != 0 );
  //      REQUIRE( /* test that item is first in list */ );
        REQUIRE(l.is_first_on_list(5) == true);
      }
    }
    WHEN( "an item is removed" ) {
      // remove an ite
      l.remove(5);
      THEN( "the list is still empty" ) {
        // add your REQUIRE statements
        REQUIRE( l.is_empty() == true );
        REQUIRE( l.size() == 0 );
      }
    }

    WHEN( "the list is copied to a new list" ) {

      // copy your list to a new variable (constructor)
      Sorted_List l2{};
      THEN( "the new list is also empty" ) {
        // add your REQUIRE statements
        REQUIRE( l2.is_empty() == true );
        REQUIRE( l2.size() == 0 );
      }
    }


      WHEN( "the list is copied to an existing non-empty list" ) {
      // create and fill a list to act as the existing list
      Sorted_List l2{};
      l2.insert(5);
      l2.insert(2);
      REQUIRE( l2.is_empty() == false );
      //copy (assign) your empty list to the existing
      Sorted_List l1{};
      l2.copylist(l1);


      THEN( "the existing list is also empty" ) {
        // add your REQUIRE statements
        REQUIRE( l2.is_empty() == true );
        REQUIRE( l2.size() == 0 );
      }

    }
  }
}
//}}}




SCENARIO( "Single item lists" ) {

  GIVEN( "A list with one item in it" ) {
    // create the given scenario
    Sorted_List l{};
      l.insert(3);
    REQUIRE(l.is_empty() == false);
    WHEN( "a smaller item is inserted" ) {
      l.insert(2);
      REQUIRE(l.size() == 2);

      REQUIRE(l.size() == 2);
      l.print_list();
      l.remove(2);
      l.remove(3);

      REQUIRE(l.size() == 0);
      l.insert(8);
      l.insert(4);


      THEN( /* describe what will happen */ "The list is sorted during the insert " ) {
        REQUIRE(l.size() == 2);

      }

    WHEN( "a larger item is inserted" ) {
      l.insert(10);
      std::cout << "the list values" << std::endl;
      l.print_list();
      l.dest_list();
      std::cout << "la taille " <<l.size() << std::endl;
      REQUIRE(l.is_empty() == true);
      //l.insert(9);
      /* describe what will happen */
      THEN(  "The list is sorted during the insert " ) {
        l.print_list();
        //REQUIRE(l.size() == 3);
        cout<< "Size = "<<l.size();
      }
    }
    WHEN( "an item is removed" ) {

      THEN( /* describe what will happen */ "The list is sorted during the insert " ) {
          REQUIRE(l.size() == 2);
      }
    }
    WHEN( "the list is copied to a new list" ) {
      THEN( ""/* describe what will happen */ ) {
        Sorted_List l2{l};
        cout << "print list l2 copy" << endl;
        l2.print_list();
        cout << "print list l" << endl;
        l.print_list();
      }
    WHEN( "the list is copied to an existing non-empty list" ) {
      THEN( ""/* describe what will happen */ ) {
        Sorted_List l3{};
        Sorted_List l2{};
        l2.insert(333);
        l2.insert(555);
        l3.insert(3);
        l3.insert(4);
        l3.insert(23);
        l3.copylist(l2);
        //l3->head=copy(l2->head);
        REQUIRE(l2.size() == l3.size());
        //l3.copy(l2);
      }
    }
//  }}}}
}}}}


SCENARIO( "Multi-item lists" ) {

  GIVEN( "A list with five items in it" ) {
    Sorted_List l{};
    l.insert(8);
    l.insert(12);
    l.insert(17);
    l.insert(11);
    l.insert(22);

    int value = -1;
    // create the given scenario and all THEN statements
    // and all REQUIRE statements
    WHEN( "an item smaller than all other is inserted" ) {
      THEN("it is put at the beginning") {
        l.insert(2);
        l.get(0, value);
        REQUIRE(value == 2);
        l.print_list();
      }
    }
    WHEN( "an item larger than all other is inserted" ) {
      THEN("it is inserted at the end") {
        l.insert(100);
        l.get(l.size()-1, value);
        REQUIRE(value == 100);
        l.print_list();
      }
    }
    WHEN( "an item smaller than all but one item is inserted" ) {
      THEN ("It is inserted at the second position") {
        l.insert(9);
        l.get(1, value);
        REQUIRE(value == 9);
        l.print_list();
      }
    }
    WHEN( "an item larger than all but one item is inserted" ) {
      THEN("It is inserted at the before-last position") {
        l.insert(20);
        l.get(l.size() - 2, value);
        REQUIRE(value == 20);
        l.print_list();
      }
    }
    WHEN( "an item is removed" ) {
      THEN("There are now 4 items in the list") {
        l.remove(8);
        REQUIRE(l.size() == 4);
        l.print_list();
      }
    }
    WHEN( "all items are removed" ) {
      THEN("Size becomes zero") {
        l.remove(8);
        l.remove(12);
        l.remove(17);
        l.remove(11);
        l.remove(22);
        REQUIRE(l.size() == 0);
        l.print_list();
      }
    }
    WHEN( "the list is copied to a new list" ) {
      Sorted_List l2{l};
      THEN("Second list contains the same values") {
        REQUIRE(l2.size() == 5);
        l2.print_list();
      }
    }
    WHEN( "the list is copied to an existing non-empty list" ) {
      Sorted_List l2{};
      THEN("Size of second list is five, and contains all the first list's values")
      l2.insert(5);
      l2.insert(10);
      l2.copylist(l);
      REQUIRE(l2.size() == 5);
      l2.print_list();
    }
  }
}

SCENARIO( "Lists can be copied" ) {
  GIVEN( "A list with five items in it, and a new copy of that list" ) {
    Sorted_List l{};
    l.insert(5);
    l.insert(10);
    l.insert(15);
    l.insert(20);
    l.insert(25);

    Sorted_List l2{l};
    WHEN( "the original list is changed" ) {
      l.insert(30);
      THEN( "the copy remain unmodified" ) {
        REQUIRE (l.size() == 6);
        REQUIRE (l2.size() == 5);
        l.print_list();
        cout << endl;
        l2.print_list();
      }
    }

    WHEN( "the copied list is changed" ) {
      l2.insert(30);
      THEN( "the original remain unmodified" ) {
        REQUIRE(l2.size() == 6);
        REQUIRE(l.size() == 5);
        l.print_list();
        cout << endl;
        l2.print_list();
      }
    }
  }
}

SCENARIO( "Lists can be heavily used" ) {

  GIVEN( "A list with 1000 random items in it" ) {

    Sorted_List l{};

    // create the given list with 1000 random items
    std::random_device random;
    std::uniform_int_distribution<int> uniform(1,1000);

    for (int i{0}; i < 1000; ++i)
    {
      int randomValue = uniform(random); // generate a random number
      l.insert(randomValue);
    }

    WHEN( "the list have 1000 random numbers inserted" ) {
      for (int i{0}; i < 1000; ++i)
      {
        int randomValue = uniform(random); // generate a random number
        l.insert(randomValue);
      }
      REQUIRE(l.size() == 2000);
      //l.print_list();
    }

    WHEN( "the list have 1000 random numbers removed" ) {
      for (int i{0}; i < 1000; ++i)
      {
        int randomValue = uniform(random); // generate a random number
        l.remove(randomValue);
      }
    }
  }
}

Sorted_List trigger_move(Sorted_List l)
{
  l.insert(100);
  return l;
}

SCENARIO( "Lists can be passed to functions" ) {

  GIVEN( "A list with 5 items in it" ) {

    Sorted_List given{};
    given.insert(5);
    given.insert(10);
    given.insert(15);
    given.insert(20);
    given.insert(25);

    WHEN( "the list is passed to trigger_move()" ) {

      Sorted_List l{ trigger_move(given) };

      THEN( "the given list remain and the result have the change" ) {
        REQUIRE(given.size() == 5);
        cout << "List l :" << endl;
        l.print_list();

        REQUIRE(l.size() == 6);
        cout << "List given: " << endl;
        given.print_list();
      }
    }
  }
}


#if 0

// To really verify that your "important five" are called you should
// add cout-statements in each during development (comment out before
// demonstration).

// In addition you must of course verify that the list is printed
// correct and that no memory leaks occur during use. You can solve
// the printing part on your own. Here's how to run the (test) program
// when you check for memory leaks:
//
// $ valgrind --tool=memcheck a.out

// Finally you need to check that you can do all operations that make
// sens also on a immutable list (eg all operations but insert):
void use_const_list(Sorted_List const& l)
{
  // perform every operation that do not modify the list here
  return l;
}

#endif
