/*
 Student Name:
 Date:

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
//static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {

//}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {
    int h;
    HashFunction hash=hashTable-> hash; //point to the hash function
    h=hash(key); //call the hash function
    HashTableEntry* node=hashTable->buckets[h]; //head of the bucket
    
    while(node){ //keep walking the list until key matches
        if(node->key==key)
            return node;
        node=node->next;
    }
    return NULL;
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets are empty, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable) {
    HashTableEntry *node;
    HashTableEntry *last; //temporary node created to access value before deletion
    int i;
    
    for(i=0;i< hashTable->num_buckets; i++){ //walk through entire bucket
        node=hashTable->buckets[i]; //head of bucket
        while (node!=NULL){ //until you reach end of bucket
            last=node;
            node=node->next;
            free(last->value); //value is a pointer
            free(last); //free the node
        }
        //free(hashTable->buckets[i]);
    }
    free(hashTable->buckets); //free buckets pointer
    free(hashTable);
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {
    HashTableEntry* node;
    void* olddata;
    HashFunction hash=hashTable-> hash; //pointing to the hash function
    int h=hash(key); //calling the hash function and storing hash value in h
    if(node=(findItem(hashTable,key))){ //key already exists, so overwrite
        olddata=node->value; //save the old data value before overwriting
        node->value=value; //overwrite the value
        return olddata; //return pointer to old value
        
    }
    node=(HashTableEntry*) malloc(sizeof(HashTableEntry)); //node is not already present, so allocate space for it
    if(!node) return NULL; //problem in memory allocation
    node->value=value; //assign value given by user to the new node
    node->key=key; //assign given key
    HashTableEntry* nullhead=NULL; //make a null node
    node->next=nullhead; //the new node's 'next' will point to the null node
    HashTableEntry* prev=hashTable->buckets[h]; //prev is the head of buckets[h]
    // /*
    if (!prev){
        hashTable->buckets[h]=node;
        return NULL;
    }
    //*/
    /*
    while (prev->next){ //walk from the head of the bucket,i.e, prev, until 'next' is null, that is until last node is reached
        prev=prev->next;
    }
    prev->next=node; //link the newly created node to the last node of the bucket
    */
    node->next=hashTable->buckets[h]; //the node that was the head earlier
    hashTable->buckets[h]=node;//make new node the new head of the bucket
    return NULL;

}

void* getItem(HashTable* hashTable, unsigned int key) {
HashTableEntry* node=findItem(hashTable,key); //cal to helper function created above
if (node!=NULL) { //if node exists in the hash table
    return node->value;
}
else{
    return NULL; //if node doesn't exist
}
}

void* removeItem(HashTable* hashTable, unsigned int key) {
    int h;
    void* data;
    HashFunction hash=hashTable-> hash; //point to hash function
    h=hash(key); //call to hash function
    HashTableEntry* node=hashTable->buckets[h];
    HashTableEntry* tmp;
    
    if(!node) return 0;
    if(node->key==key){ //if required node is at the head of the bucket
        data=node->value;
        hashTable->buckets[h]=node->next; //update the next pointer
        free(node);
        return data;
    }
    while(node->next){ //else keep going to the next node of the bucket
        if(node->next->key==key){ 
            tmp=node->next; //store pointer before removing
            data=tmp->value; //access value
            node->next=node->next->next; //link to the node after the removed node
            free(tmp);
            return data;
        }
        node=node->next;
    }
    return NULL;

}

void deleteItem(HashTable* hashTable, unsigned int key) { //similar to removeItem but frees the node
    int h;
    HashFunction hash=hashTable-> hash;
    h=hash(key);
    HashTableEntry* node=hashTable->buckets[h];
    HashTableEntry* tmp;
    
    //if(!node) return NULL;
    if(node->key==key){ //if required node is at the head of the bucket
        hashTable->buckets[h]=node->next;
        //node = node->next;
        free(node->value);
        free(node);
        return;
    }
    while(node->next){ //else keep going to the next node of the bucket
        if(node->next->key==key){ 
            tmp=node->next;
            node->next=node->next->next;
            free(tmp->value);
            free(tmp);
            return;
        }
        node=node->next;
    }

}