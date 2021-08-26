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

  skip.DisplayList();

  std::cout << "\n\n\n"
            << "is 1 in skip ? " << (skip.find(&l[10]) == nullptr) << '\n'
            << "is 3 in skip ? " << (skip.find(&l[0]) == nullptr) << '\n';

  std::cout << "\n\n\n"
            << "skip[0]: " << *(skip[0]->ptr_) << '\n'
            << "skip[1]: " << *(skip[1]->ptr_) << '\n'
            << "skip[3]: " << *(skip[3]->ptr_) << '\n';

  skip.remove(&l[2]);

  skip.DisplayList();

  std::cout << "\n\n\n"
            << "skip[0]: " << *(skip[0]->ptr_) << '\n'
            << "skip[1]: " << *(skip[1]->ptr_) << '\n'
            << "skip[3]: " << *(skip[3]->ptr_) << '\n';

  return 0;
}
