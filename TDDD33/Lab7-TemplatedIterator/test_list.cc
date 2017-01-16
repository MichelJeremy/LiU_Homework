#include <iostream>
#include "list.h"
using namespace std;

#define SOLUTION 1 // Change to 1 when completely done

int main()
{
#if SOLUTION == 0

  // Minimal code to use the list
  // You are recommended to start with your template implementation
  // and convert this code to instantiate it.
  // Move on to the iterator once it compiles.
  List<int> list;
  decltype(list)::value_type number;
  cout << "Enter numbers, finish by Ctrl-D " << endl;
  while (cin >> number)
  {
    list.insert(number);

  }

  

#else
  // example of using the list with integers
  {
    List<int> list;
    decltype(list)::value_type number;

    cout << "Enter numbers, finish by Ctrl-D " << endl;
    while (cin >> number)
    {
      list.insert(number);
    }

    for ( decltype(list)::Iterator it{ list.begin() };
          it != list.end(); ++it)
    {
      cout << *it << " ";
    }
    cout << endl;

    for ( auto i : list )
    {
      cout << i << " ";
    }
    cout << endl;

    cout << list << endl;
  }

  cin.clear();

  // example of using the list with std::string
  // ( notice the overwhelming similarity to above code? )
  {
    List<std::string> list;
    decltype(list)::value_type word;

    cout << "Enter words, finish by Ctrl-D " << endl;
    while (cin >> word)
    {
      list.insert(word);
    }

    for ( decltype(list)::Iterator it{ list.begin() };
          it != list.end(); ++it)
    {
      cout << *it << " ";
    }
    cout << endl;


    for ( auto w : list )
    {
      cout << w << " ";
    }
    cout << endl;

    cout << list << endl;
  }
#endif

  return 0;
}
