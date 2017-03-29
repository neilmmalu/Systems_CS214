#include<stdio.h>
#include<stdlib.h>
#include"sorted-list.h"

SortedListPtr SLCreate(CompareFuncT cf, DestructFuncT df){
		SortedListPtr newList=(SortedListPtr)malloc(sizeof(SortedList));
		newList->compare=cf;
		newList->destroy=df;
		newList->head=NULL;
    newList->size=0;
		return newList;
}
void SLDestroy(SortedListPtr list){
  Node* nodeDestroyer = list->head;
  while(nodeDestroyer != NULL){
    Node* temp = nodeDestroyer;
    nodeDestroyer = nodeDestroyer->next;
    list->destroy(temp->data);
    free(temp);
  }
  list->head = NULL;
  free(list);
}
int SLInsert(SortedListPtr list, void *newObj){
  if(list == NULL || newObj == NULL){
    return 0;
  }
  Node* curr = list->head;
  if(curr == NULL){
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = newObj;
    newNode->next = NULL;
    newNode->refCounter = 1;
    newNode->inTheList = 1;
    list->head = newNode;
    list->size=list->size+1;
    // printf("Adding: %f\n",*(float *)list->head->data);
    return 1;
  }
  Node* prev = NULL;
  while(curr !=NULL){
    // printf("check 1\n");
    int c = list->compare(newObj,curr->data);
    if(c == 0){
      return 0;
    }
    else if(c == 1){
      Node* newNode = (Node*)malloc(sizeof(Node));
      newNode->data = newObj;
      newNode->next = curr;
      newNode->refCounter = 1;
      newNode->inTheList = 1;
      if(prev == NULL){
        list->head = newNode;
        // printf("Adding: %f\n",*(float *)list->head->data);
      }
      else{
        prev->next = newNode;
        // printf("Adding: %f\n",*(float *)prev->data);
      }
      list->size=list->size+1;
      return 1;
    }
    else if( c == -1 && !(curr->next)){
      Node* newNode = (Node*)malloc(sizeof(Node));
      newNode->data = newObj;
      newNode->next = curr;
      newNode->next = NULL;
      newNode->refCounter = 1;
      newNode->inTheList = 1;
      curr->next = newNode;
      list->size=list->size+1;
    }
    prev = curr;
    curr= curr->next;
  }
  // printf("check 2\n");
  return 0;
}
void DestroyNode(SortedListPtr list ,Node* nodePtr){
  if(nodePtr==NULL||list==NULL){
    return;
  }
  if(nodePtr->data==NULL||list->destroy==NULL){
    return;
  }
  list->destroy(nodePtr->data);
  free(nodePtr);

}
int SLRemove(SortedListPtr list, void *newObj){
  if(list == NULL || newObj == NULL){
    return 0;
  }
  Node* curr = list->head;
  if(curr==NULL||curr->data==NULL){
    return 0;
  }
  int c = list->compare(newObj,curr->data);
  if(c == 0){
    curr->refCounter--;
    curr->inTheList = 0;
    list->head = curr->next;
    if(list->head){
      list->head->refCounter++;
    }
    if(curr->refCounter == 0){
      DestroyNode(list,curr);
    }
    // free(newObj);
    list->size=list->size-1;
    return 1;
  }
  Node* prev = curr;
  curr= curr->next;
  while(curr !=NULL){
    c = list->compare(newObj,curr->data);
    if(c == 0){
      curr->refCounter--;
      curr->inTheList = 0;
      prev->next = curr->next;
      if(prev->next){
        prev->next->refCounter++;
      }
      if(curr->refCounter == 0){
        DestroyNode(list,curr);
      }
      // free(newObj);
      list->size=list->size-1;
      return 1;
    }
    if(c == 1){
      free(newObj);
      return 0;
    }
    prev = curr;
    curr= curr->next;
  }
  // free(newObj);
  return 0;
}
SortedListIteratorPtr SLCreateIterator(SortedListPtr list){
  if(list == NULL){
    return NULL;
  }
  SortedListIteratorPtr iterator = (SortedListIteratorPtr)malloc(sizeof(struct SortedListIterator));
  iterator->list = list;
  iterator->currentNode = list->head;
  if(iterator->currentNode){
    iterator->currentNode->refCounter++;
  }
  return iterator;
}
void SLDestroyIterator(SortedListIteratorPtr iter){
  if(iter == NULL){
    return;
  }
  if(iter->list){
    if(iter->currentNode){
      iter->currentNode->refCounter--;
      if(iter->currentNode->refCounter==0){
        iter->list->destroy(iter->currentNode->data);
        free(iter->currentNode);
      }
    }
  }
  free(iter);
}

//===1.2: SortedList Iterator Get/Next Operations

/*
 * SLNextItem returns a pointer to the data associated with the
 *  next item in the SortedListIterator's list
 *
 * SLNextItem should return a NULL if all of iter's elements have
 *  been iterated through.
 *
 * NB: Be sure to check the length of the list that SortedListIterator holds
 *         before attempting to access and return an item from it.
 *         If an item is removed from the list, making it shorter, be careful not
 *         to try to read and return an item off the end of the list.
 */

void * SLNextItem(SortedListIteratorPtr iter){
  if(iter == NULL || iter->list == NULL || iter->currentNode == NULL || iter->currentNode->data == NULL){
    return NULL;
  }
  iter->currentNode->refCounter--;
  Node* temp=iter->currentNode;
  iter->currentNode = iter->currentNode->next;

  if(temp->refCounter==0){
    iter->list->destroy(temp->data);
    free(temp);
  }

  while(iter->currentNode && iter->currentNode->inTheList == 0){
    iter->currentNode = iter->currentNode->next;
  }
  
  if(iter->currentNode){
    iter->currentNode->refCounter++;
    return iter->currentNode->data;
  }
  return NULL;

}


/*
 * SLGetItem should return a pointer to the current data item
 *   The current data item is the item that was most recently returned by SLNextItem
 *   SLGetItem should not alter the data item that SortedListIterator currently refers to
 *
 * SLGetItem should return a NULL pointer if the SortedListIterator has been advanced
 *  all the way through its list.
 *
 * NB: Be sure to check the length of the list that SortedListIterator holds
 *         before attempting to access and return an item from it.
 *         If an items are removed from the list, making it shorter, be careful not
 *         to try to read and return an item off the end of the list.
 */

void * SLGetItem( SortedListIteratorPtr iter ){
  if(iter == NULL || iter->list == NULL || iter->currentNode == NULL || iter->currentNode->data == NULL){
    return NULL;
  }
  return iter->currentNode->data;
}

