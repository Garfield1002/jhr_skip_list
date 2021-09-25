// jhr_skip_list - v1.0 - public domain skip list implementation
//                                    no warranty implied; use at your own risk
//
// A C++ implementation of William Pugh's Skip Lists with width
//
// "Skip lists are a data structure that can be used in place of balanced trees.
// Skip lists use probabilistic balancing rather than strictly enforced
// balancing and as a result the algorithms for insertion and deletion in skip
// lists are much simpler and significantly faster than equivalent algorithms
// for balanced trees."
//
//
// ============================== RESEARCH PAPER ==============================
// https://www.epaperpress.com/sortsearch/download/skiplist.pdf
//
//
// ================================== USAGE ===================================
//```cpp
//   #define JHR_SKIP_LIST_IMPLEMENTATION
//   #include "jhr_skip_list.hpp"
//
//   jhr::Skip_List<int> lst = jhr::Skip_List<int>();
//
//   int l[]{3, 6, 7, 9, 12, 19, 17, 26, 21, 25, 1, 2, 4, 5, 50, 100, 18};
//
//   for (size_t i = 0; i < 17; i++) {
//     lst.insert(new int{l[i]});
//   }
//
//   lst.DisplayList();
//```
//
// =============================== CONTRIBUTORS ===============================
// Jack Royer (base file)
//
//
// ================================= LICENSE ==================================
// See end of file for license information.
//

#ifndef JHR_SKIP_LIST_H
#define JHR_SKIP_LIST_H

// =============================== DOCUMENTATION ==============================
// This is an implementation of Skip Lists with widths.
//
// Why use Skip Lists ?
// --------------------
// Skip Lists allow insertion, deletion, random access and search in O(log n)
// on average (and O(n) in worst case).
// Skip lists are a simple data structure that can be used in place of balanced
// trees for most applications and are much less daunting.
//
// Single File
// -----------
// We are using the standard single file trick. If you just include it normally,
// you just get the header file function definitions. To get the code, you
// include it from a C++ file and define JHR_SKIP_LIST_IMPLEMENTATION first.
// (see USAGE)
//
// Supported Types
// ---------------
// Template types are used throughout the file.
// This type needs to have both < and == operators.
//
// Skip List Methods
// -----------------
//  | Function        | Effect                                                |
//  |             Creation and deletion                                       |
//  | (constructor) | Create a skip list TODO COPY                            |
//  | (desctructor) | Destroy a skip list                                     |
//  |             Size and capacity                                           |
//  | length()      | Returns the number of elements in skip list             |
//  | MaxLevel()    | Calculates the optimal maximum for the amount of levels |
//  |             Element access                                              |
//  | [], at()      | Accesses the element at a particular index              |
//  |             Modification                                                |
//  | insert()      | Inserts an element in the skip list                     |
//  | remove()      | Removes an element from the skip list and deletes it    |
//  |             Searching                                                   |
//  | find()        | Finds the node associated to a pointer in the skip list |
//  |             Visualization                                               |
//  | DisplayList() | Prints a visual representation of the skip list         |
// ============================================================================

#include <cmath>  // for log
#include <cstdlib>
#include <iostream>
#include <string>

namespace jhr {
template <typename T>
class Skip_Node;

template <typename T>
struct Skip_Link {
  size_t width{1};
  Skip_Node<T>* node{nullptr};
};

// A node for an item inside the skip list
template <typename T>
class Skip_Node {
 private:
  size_t level_;

 public:
  T* const ptr_;

  // Array of links to different nodes
  Skip_Link<T>* forward_;

  Skip_Node(T* const& ptr, size_t level)
      : ptr_{ptr}, forward_{new Skip_Link<T>[level]}, level_{level} {
    // Initialize the link array
    for (size_t i = 0; i < level; i++) {
      forward_[i] = Skip_Link<T>();
    }
  }

  ~Skip_Node() {
    if (forward_) delete[] forward_;
  };
};

// TODO description
template <typename T>
class Skip_List {
 private:
  // Maximum level for this skip list
  const size_t kMaxLevel_{16};

  // The probabilty to add a new level.
  // p_ is a probability so we must have 0<= p_ <= 1
  const float p_{0.5f};

  // Current highest level of skip list
  size_t level_{1};

  size_t width_{0};

  // Pointer to first node, this node does not contain any data
  Skip_Node<T>* head_{new Skip_Node<T>(nullptr, kMaxLevel_)};

  // Returns a random level. This level is always smaller than `kMaxLevel_`.
  size_t RandomLevel();

  // Creates a new node, wraps the node initializer.
  inline Skip_Node<T>* CreateNode(T* const& ptr, size_t level) {
    return new jhr::Skip_Node<T>(ptr, level);
  }

  static std::string CenterString(const std::string& s, size_t width);

 public:
  Skip_List() {}

  Skip_List(size_t max_level, float p)
      : kMaxLevel_{max_level},
        p_{p},
        head_{new Skip_Node<T>(nullptr, max_level)} {}

  ~Skip_List() {
    if (!head_) return;
    Skip_Node<T>* node = head_;
    while (node) {
      Skip_Node<T>* next_node = node->forward_[0].node;
      delete node->ptr_;
      delete node;
      node = next_node;
    }
  };

  Skip_Node<T>* at(size_t index);
  Skip_Node<T>* operator[](size_t index) { return at(index); };

  void DisplayList();

  // Returns `true` if the skip list is empty.
  inline bool empty() { return !head_->forward_[0].node; }

  Skip_Node<T>* find(T* const& ptr);

  Skip_Node<T>* insert(T* const& ptr);

  // Returns the length of the skip list.
  inline size_t length() { return width_; }

  static size_t MaxLevel(size_t N /*maximum number of elements*/, float p);

  T* remove(T* const& ptr);

  // TODO add + operator support
  // add an arry or an other skip list ?
};
}  // namespace jhr
#endif  // JHR_SKIP_LIST_H
#ifdef JHR_SKIP_LIST_IMPLEMENTATION

// Returns the `index`th element of the skip list.
// If `index` is greater than the width of the skip list returns a null
// pointer.
template <typename T>
jhr::Skip_Node<T>* jhr::Skip_List<T>::at(size_t index) {
  if (index >= width_) return nullptr;

  size_t w{index + 1};

  jhr::Skip_Node<T>* x{head_};

  for (size_t i = level_; i > 0; i--) {
    while (x->forward_[i - 1].node != nullptr &&
           x->forward_[i - 1].width <= w) {
      w -= x->forward_[i - 1].width;
      x = x->forward_[i - 1].node;
      if (w == 0) return x;
    }
  }

  return nullptr;
};

// Centers a string by padding it left and right with spaces.
template <typename T>
inline std::string jhr::Skip_List<T>::CenterString(const std::string& s,
                                                   size_t width) {
  std::string r = std::string((width - 1) / 2 - s.length() / 2, ' ');
  r = r + s;
  r.resize(width, ' ');
  return r;
};
// Draws a visual representation of the skip list.
// A link to a node is represented by an arrow (`o-->`) and final elements of
// a level, that point to a null pointer, are represented by an `x`.
template <typename T>
void jhr::Skip_List<T>::DisplayList() {
  // Example result, heavily inspired by wikipedia's illustrations on skip
  // lists
  //            4
  // o---------------------> x Level 2
  //   1     1        2
  // o---> o---> o---------> x Level 1
  //   1     1     1     1     1     1
  // o---> o---> o---> o---> o---> o---> Level 0
  //       3     6     7     9     12

  for (size_t i = level_; i > 0; i--) {
    Skip_Node<T>* node = head_;

    // Draws the width labels
    while (node != nullptr) {
      if (node->forward_[i - 1].width > 0)
        std::cout << CenterString(std::to_string(node->forward_[i - 1].width),
                                  node->forward_[i - 1].width * 6);

      node = node->forward_[i - 1].node;
    }

    std::cout << std::endl;

    // Draws the arrows
    node = head_;
    while (node != nullptr) {
      if (node->forward_[i - 1].width > 0)
        std::cout << "o"
                  << std::string(node->forward_[i - 1].width * 6 - 3, '-')
                  << "> ";
      else
        std::cout << "x ";
      node = node->forward_[i - 1].node;
    }

    std::cout << "Level " << i - 1 << std::endl;
  }

  // Draws the node labels
  Skip_Node<T>* node = head_;
  while (node != nullptr) {
    if (node->ptr_) {
      std::cout << std::to_string(*(node->ptr_))
                << std::string(6 - std::to_string(*(node->ptr_)).length(), ' ');
    } else {
      std::cout << std::string(6, ' ');
    }
    node = node->forward_[0].node;
  }
}

// Returns the node associated with `ptr` if it exist.
// Ff `ptr` is not in the skip list it returns a null pointer.
template <typename T>
jhr::Skip_Node<T>* jhr::Skip_List<T>::find(T* const& ptr) {
  jhr::Skip_Node<T>* x{head_};

  for (size_t i = level_; i > 0; i--) {
    while (x->forward_[i - 1].node != nullptr &&
           *(x->forward_[i - 1].node->ptr_) < *ptr) {
      x = x->forward_[i - 1].node;
    }
  }
  x = x->forward_[0].node;
  if (*(x->ptr_) == *ptr) return x;

  return nullptr;
};

// Inserts a new element in the skip list returns a pointer to the newly
// created node. If the element was already in the skip list, returns a
// pointer to the node with that elenent.
template <typename T>
jhr::Skip_Node<T>* jhr::Skip_List<T>::insert(T* const& ptr) {
  // Array of pointers to elements that will need updating
  Skip_Node<T>** update{new Skip_Node<T>* [kMaxLevel_] {}};

  size_t* update_width{new size_t[kMaxLevel_]{}};

  Skip_Node<T>* x{head_};

  for (size_t i = level_; i > 0; i--) {
    // The sum of traveled widths
    size_t width_sum{0};

    while (x->forward_[i - 1].node != nullptr &&
           *(x->forward_[i - 1].node->ptr_) < *ptr) {
      width_sum += x->forward_[i - 1].width;
      x = x->forward_[i - 1].node;
    }

    update[i - 1] = x;
    update_width[i - 1] = width_sum;
  }

  size_t level{RandomLevel()};

  // Complete the update list with the head of the list
  // if the new node is the first node on a new levels
  if (level > level_) {
    for (size_t i = level_; i < level; i++) {
      update[i] = head_;

      // For simplicity, the width to nullptr is always 0
      head_->forward_[i].width = 0;
    }
    level_ = level;
  }

  // If the node is already in the list retuns the already existing node
  if (x->forward_[0].node != nullptr)
    if (*(x->forward_[0].node->ptr_) == *ptr) return x->forward_[0].node;

  Skip_Node<T>* new_node = CreateNode(ptr, level);

  for (size_t i = 0; i < level; i++) {
    // Update the linked nodes
    new_node->forward_[i].node = update[i]->forward_[i].node;
    update[i]->forward_[i].node = new_node;

    // updates the widths of the links
    if (i > 0) {
      size_t width_before{update_width[i - 1] +
                          update[i - 1]->forward_[i - 1].width};

      if (update[i]->forward_[i].width > 0)
        // The width is the width of the previous connection,
        // + 1 ( because we are inserting a node )
        // - whatever is before the new node
        new_node->forward_[i].width =
            update[i]->forward_[i].width + 1 - width_before;
      else
        // For simplicity, the width to nullptr is always 0
        new_node->forward_[i].width = 0;

      update[i]->forward_[i].width = width_before;
    }
  }

  delete[] update;
  delete[] update_width;

  width_++;

  return new_node;
}

// Returns the optimal max level based on the probability `p` to add a new
// level and the estimated maximum number of elements `N`
// If `p` is invalid (p > 1 || p < 0) returns 0
template <typename T>
inline size_t jhr::Skip_List<T>::MaxLevel(
    size_t N /*maximum number of elements*/, float p) {
  if (!(0.0F <= p <= 1.0F)) return 0;
  return static_cast<size_t>(log(N) / log(1 / p));
}

template <typename T>
size_t jhr::Skip_List<T>::RandomLevel() {
  float rnd{std::rand() / static_cast<float>(RAND_MAX)};
  size_t level{1};
  while (rnd < p_ && level < kMaxLevel_ - 1) {
    level++;
    rnd = std::rand() / static_cast<float>(RAND_MAX);
  }
  return level;
}

// Removes an element from the skip list and returns a boolean if the
// operation was successful
template <typename T>
T* jhr::Skip_List<T>::remove(T* const& ptr) {
  Skip_Node<T>** update{new jhr::Skip_Node<T>* [level_] {}};

  Skip_Node<T>* x{head_};

  for (size_t i = level_; i > 0; i--) {
    while (x->forward_[i - 1].node != nullptr &&
           *(x->forward_[i - 1].node->ptr_) < *ptr) {
      x = x->forward_[i - 1].node;
    }
    update[i - 1] = x;
  }

  x = x->forward_[0].node;

  // Could not find `*ptr` in the skip list
  if (!(*(x->ptr_) == *ptr)) return nullptr;

  for (size_t i = 0; i < level_; i++) {
    if (update[i]->forward_[i].node != x) {
      update[i]->forward_[i].width--;
    } else {
      update[i]->forward_[i].node = x->forward_[i].node;

      // Removes the link to other nodes to not erase the entire list when we
      // delete this node
      x->forward_[i].node = nullptr;

      if (x->forward_[i].width > 0)
        update[i]->forward_[i].width += x->forward_[i].width - 1;
      else
        update[i]->forward_[i].width = 0;
    }
  }

  delete[] update;

  T* old_data = x->ptr_;
  delete x;

  // Updates the list's max level
  while (level_ > 1 && head_->forward_[level_ - 1].node == nullptr) level_--;

  return old_data;
};

#endif

// ============================================================================
// This software is available under 2 licenses-- choose whichever you prefer
// ============================================================================
// ALTERNATIVE A - MIT License
//
// Copyright(c) 2021 Jack Royer
// Permission is hereby granted, free of charge,
// to any person obtaining a copy of this software and associated documentation
// files(the "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and / or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the following
// conditions: The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// ============================================================================
// ALTERNATIVE B - Public Domain(www.unlicense.org)
//
// This is free and unencumbered software released into the public domain.
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// ============================================================================
