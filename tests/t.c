#include <stdio.h>
#include <stdlib.h>
#include <catl/linked-list/catl_linked_list.h>

#define catl_make_ptr(type, value) ( (void*) &(type) {value} )

int main() {
  catl_linked_list_t* linked_list;
  catl_linked_list_create(&linked_list, sizeof(float), free);
  catl_linked_list_push_back(linked_list, catl_make_ptr(float, 1.0f));

  float* data;
  catl_linked_list_get((void**)&data, linked_list, 0);
  printf("%f\n", *data);
}
