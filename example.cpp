#include <iostream>

#define JHR_SKIP_LIST_IMPLEMENTATION
#include "jhr_skip_list.hpp"

// Example usage of a skip list
int main() {
  jhr::Skip_List<int> skip = jhr::Skip_List<int>();

  int l[]{3, 6, 7, 9, 12, 19, 17, 26, 21, 25, 1, 2, 4, 5, 50, 100, 18};

  for (size_t i = 0; i < 5; i++) {
    skip.insert(new int{l[i]});
  }

  std::cout << "Display Example" << std::endl;
  skip.DisplayList();

  std::cout << "\n\nFind Example" << std::endl
            << "is 1 in skip ? " << (skip.find(&l[10]) == nullptr) << std::endl
            << "is 3 in skip ? " << (skip.find(&l[0]) == nullptr) << "\n\n";

  std::cout << "Random Access Example" << std::endl
            << "skip[0]: " << *(skip[0]->ptr_) << std::endl
            << "skip[1]: " << *(skip[1]->ptr_) << std::endl
            << "skip[3]: " << *(skip[3]->ptr_) << "\n\n";

  skip.remove(&l[2]);
  std::cout << "Remove Example" << std::endl
            << "Removed element 7 from the skip list."
            << "\n\n";

  std::cout << "Display Example" << std::endl;
  skip.DisplayList();

  std::cout << "\n\nRandom Access Example" << std::endl
            << "skip[0]: " << *(skip[0]->ptr_) << std::endl
            << "skip[1]: " << *(skip[1]->ptr_) << std::endl
            << "skip[3]: " << *(skip[3]->ptr_) << '\n';

  return 0;
}
