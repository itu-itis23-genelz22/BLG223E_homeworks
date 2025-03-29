#include <stddef.h>
#include "../include/min_heap.h"




// Creates a new heap with initial capacity
// Element_size specifies the size of stored elements in bytes
// Compare function must return negative if first argument is smaller
MinHeap* heap_create(size_t capacity, size_t element_size, int (*compare)(const void*, const void*)){
    MinHeap* new_heap = (MinHeap*)malloc(sizeof(MinHeap));
    if(!new_heap) return NULL;

    new_heap->data = malloc(capacity * element_size);
    if (!new_heap->data) {
        free(new_heap);
        return NULL;
    }
    new_heap->element_size = element_size;
    new_heap->capacity = capacity;
    new_heap->size = 0; 
    new_heap->compare = compare; 

    return new_heap;
}

// Deallocates all memory used by the heap
void heap_destroy(MinHeap* heap){ 
    if(!heap) return;
    free(heap->data);
    free(heap);  
    
}

// Adds new element to heap
// Element is added at the end and bubbled up to maintain heap property
// If heap is full, capacity is doubled automatically
// Returns 1 if successful, 0 if memory allocation fails
int heap_insert(MinHeap* heap, const void* element){
    //there is no heap to be inserted
    if(!heap) return 0; //Returns 0 if memory allocation fails

    // If heap is full, capacity is doubled automatically
    if(heap->size >= heap->capacity){
        size_t new_capacity = heap->capacity*2; //doubled
        void* new_data = realloc(heap->data, new_capacity * heap->element_size);
        if (!new_data) return 0; //Returns 0 if memory allocation fails
        heap->data = new_data;
        heap->capacity = new_capacity;
    }
    // Adds new element to heap at the end 
    void* end_of_heap = (char*)heap->data + heap->size * heap->element_size;
    memcpy(end_of_heap, element, heap->element_size);
    size_t index = heap->size;
    heap->size++;

    //and new element is bubbled up to maintain heap property
    while(index>0){
        size_t parent_index = (index - 1) / 2;
        void* current_element = (char*)heap->data + index * heap->element_size;
        void* parent_element = (char*)heap->data + parent_index * heap->element_size;
    
        if (heap->compare(current_element, parent_element) < 0) {
            void* temp = malloc(heap->element_size);
            if (!temp) return 0;
            
            //swapping and bubble up
            memcpy(temp, current_element, heap->element_size);
            memcpy(current_element, parent_element, heap->element_size);
            memcpy(parent_element, temp, heap->element_size);
            free(temp);

            index = parent_index;
        } else { //heap is correct 
            break; 
        }
    }
    // Returns 1 if successful
    return 1; 
}

// Removes and returns the minimum element (root)
// Last element is moved to root and bubbled down
// Returns 1 if successful, 0 if heap is empty
int heap_extract_min(MinHeap* heap, void* result){
    if(!heap || heap->size==0) return 0; //empty heap

    memcpy(result, heap->data, heap->element_size); //the min element (root)

    void* last_element= (char*)heap->data + (heap->size-1) * heap->element_size;
    memcpy(heap->data, last_element , heap->element_size); // Last element is moved to root and bubbled down
    heap->size--;     
      
    size_t current_index = 0;
    //and bubbled down to maintain heap property
    while(current_index<heap->size){
        size_t left_child_index = current_index*2 +1;
        size_t right_child_index = current_index*2 +2;
        size_t smallest = current_index; //index of small one 

        
        if (left_child_index < heap->size) {
            void* left_element = (char*)heap->data + left_child_index * heap->element_size;
            void* smallest_element = (char*)heap->data + smallest * heap->element_size;
            if (heap->compare(left_element, smallest_element) < 0) { //left is smaller so left child must go up
                smallest = left_child_index;
            }
        }
        if (right_child_index < heap->size) {
            void* right_element = (char*)heap->data + right_child_index * heap->element_size;
            void* smallest_element = (char*)heap->data + smallest * heap->element_size;
            if (heap->compare(right_element, smallest_element) < 0) { //right child is smaller so right must go up
                smallest = right_child_index; 
            }
        }
        if (smallest != current_index) { //if new root is not the smallest it must go down (bubble down)
            void* temp = malloc(heap->element_size);
            if (!temp) return 0;

            void* current_element = (char*)heap->data + current_index * heap->element_size;
            void* smallest_element = (char*)heap->data + smallest * heap->element_size;
            //swapping in heap
            memcpy(temp, current_element, heap->element_size);
            memcpy(current_element, smallest_element, heap->element_size);
            memcpy(smallest_element, temp, heap->element_size);
            free(temp);

            current_index = smallest; //control again 
        } else { //heap ordering is correct now
            break;
        }
    }

    return 1; //successful
}

// Returns the minimum element without removing it
// Returns 1 if successful, 0 if heap is empty
int heap_peek(const MinHeap* heap, void* result){
    if (!heap || heap->size == 0) return 0; //empty heap

    memcpy(result, heap->data, heap->element_size); 
    return 1; //successful
}

// Returns current number of elements in heap
size_t heap_size(const MinHeap* heap){
    if (!heap) return 0;
    return heap->size;
}

// Merges heap2 into heap1
// Grows capacity of heap1 if needed
// Returns 1 if successful, 0 if memory allocation fails or heaps are incompatible
int heap_merge(MinHeap* heap1, const MinHeap* heap2){
    if (heap1->element_size != heap2->element_size || heap1->compare != heap2->compare) {
        return 0; //incompatible heaps -> return 0
    }

    //insert heap2s elements to end of heap 1 and bubble up (insert func already does that)
    //insert func grows the capacity  of heap1 if needed
    for(size_t i=0; i<heap2->size; i++){  
        void* element = (char*)heap2->data + i * heap2->element_size;
        if (!heap_insert(heap1, element)) {
            return 0; // 0 if memory allocation fails
        }
    }
    
    return 1; //successful 
}