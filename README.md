# C++ Skip List Implementation

_You should probably favor of the more universal [C89 skip list library](https://github.com/Garfield1002/jrsl). Though feel free to continue using this library if you prefer fancy C++ notations_

> Skip lists are a data structure that can be used in place of balanced trees.
> Skip lists use probabilistic balancing rather than strictly enforced balancing
> and as a result the algorithms for insertion and deletion in skip lists are
> much simpler and significantly faster than equivalent algorithms for
> balanced trees.

This is an implementation of the data type described in [William Pugh's paper](https:www.epaperpress.com/sortsearch/download/skiplist.pdf).

## 💡 Why use Skip Lists

Skip Lists allow insertion, deletion, random access and search in O(log n) on average (and O(n) in worst case).
Skip lists are a simple data structure that can be used in place of balanced trees for most applications and are much less daunting.

## 📚 Usage

The idea behind single-header file libraries is that they're easy to distribute and deploy because all the code is contained in a single file.
The .hpp file acts as its own header files, i.e. it declares the functions and classes contained in the file but no code is getting compiled.

So in addition, you should select _exactly_ one C++ source file that actually instantiates the code, preferably a file you're not editing frequently.
This file should define `JHR_SKIP_LIST_IMPLEMENTATION` to actually enable the function definitions.

```cpp
#define JHR_SKIP_LIST_IMPLEMENTATION
#include "jhr_skip_list.hpp"

jhr::Skip_List<int> lst = jhr::Skip_List<int>();

int l[]{3, 6, 7, 9, 12, 19, 17, 26, 21, 25, 1, 2, 4, 5, 50, 100, 18};

for (size_t i = 0; i < 17; i++) {
lst.insert(new int{l[i]});
}

lst.DisplayList();

```

Skip List Methods

---

<table>
  <tr>
    <th>Function</th>
    <th>Effect</th>
  </tr>
  <tr>
    <td colspan="2">
        <b>Creation and deletion</b>
    </td>
  </tr>
  <tr>
    <td>(constructor)</td>
    <td>Create a skip list</td>
  </tr>
  <tr></tr>
  <tr>
    <td>(destructor)</td>
    <td>Destroy a skip list</td>
  </tr>
  <tr>
    <td colspan="2">
        <b>Size and capacity</b>
    </td>
  </tr>
  <tr>
    <td>length()</td>
    <td>Returns the number of elements in skip list</td>
  </tr>
  <tr></tr>
  <tr>
    <td>MaxLevel()</td>
    <td>Calculates the optimal maximum for the amount of levels</td>
  </tr>
  <tr>
    <td colspan="2">
        <b>Element access</b>
    </td>
  </tr>
  <tr>
    <td>[], at()</td>
    <td>Accesses the element at a particular index</td>
  </tr>
  <tr>
    <td colspan="2">
        <b>Modification</b>
    </td>
  </tr>
  <tr>
    <td>insert()</td>
    <td>Inserts an element in the skip list</td>
  </tr>
  <tr></tr>
  <tr>
    <td>remove()</td>
    <td>Removes an element from the skip list and returns it</td>
  </tr>
  <tr>
    <td colspan="2">
        <b>Searching</b>
    </td>
  </tr>
  <tr>
    <td>find()</td>
    <td>Finds the node associated to a pointer in the skip list</td>
  </tr>
  <tr>
    <td colspan="2">
        <b>Visualization</b>
    </td>
  </tr>
  <tr>
    <td>DisplayList()</td>
    <td>Prints a visual representation of the skip list</td>
  </tr>
</table>

## ⭐ Contribution

All contributions are welcome!
The code is currently following [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

## ⚖ License

This library is in the public domain. You can do anything you want with it. You have no legal obligation to do anything else, although I appreciate attribution.

It is also licensed under the MIT open source license, if you have lawyers who are unhappy with public domain. The source file includes an explicit dual-license for you to choose from.
