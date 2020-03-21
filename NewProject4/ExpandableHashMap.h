
//  ExpandableHashMap.h
//  Project4
//
//  Created by Jiamin Xu on 3/4/20.
//  Copyright © 2020 Jiamin Xu. All rights reserved.
//

// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

#include<iostream>
using namespace std;

template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }

    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;
    

private:
    struct Node
    {
        KeyType key;
        ValueType value;
        Node* next;
    };
    
    int m_buckets;
    double m_maximumLoadFactor;
    Node** buckets; //pointer to an array of node pointers (aka the buckets)
    int associations;
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
{
    if (maximumLoadFactor <= 0)
    {
        m_maximumLoadFactor = 0.5;
    }
    
    else
    {
        m_maximumLoadFactor = maximumLoadFactor;
    }
    
    m_buckets = 8;
    associations = 0;
       buckets = new Node*[m_buckets]; //set all buckets to nullptr
       for (int i = 0; i < m_buckets; i++)
       {
           buckets[i] = nullptr;
       }
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
    for (int i = 0; i < m_buckets; i++)
    {
         Node* p = buckets[i]; //delete all buckets
         while (p != nullptr)
         {
             Node* kill = p;
             p = p->next;
             delete kill;
         }
     }
     delete[] buckets;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    for (int i = 0; i < m_buckets; i++) //delete all buckets
      {
           Node* p = buckets[i];
           while (p != nullptr)
           {
               Node* kill = p;
               p = p->next;
               delete kill;
           }
       }
       delete[] buckets;
     
     m_buckets = 8; //reset the size of hashmap
     associations = 0;
     buckets = new Node*[m_buckets];
     for (int i = 0; i < m_buckets; i++) //set every bucket to null
     {
         buckets[i] = nullptr;
     }
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return associations;
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
     unsigned int hasher(const KeyType& k); // prototype
    
    ValueType* v = find(key);  //replace the previous associated value with this key, with the new value
    if (v!= nullptr)
    {
        *v = value;
        return;
    }
    
    else
    {
        if ((1.0 * (associations + 1) / m_buckets) > m_maximumLoadFactor) //if adding a new node does exceed the max load factor
            {
            Node **previousHash = buckets;
        
            m_buckets = m_buckets * 2; //update size of hash table to double
            buckets = new Node*[m_buckets];
            for (int i = 0; i < m_buckets; i++)
            {
                buckets[i] = nullptr;
            }
            
            // Copy over data into new hash table
            for (int i = 0; i < m_buckets / 2; i++)
            {
                if (previousHash[i] == nullptr)
                {
                    continue;
                }
                else
                {
                    Node* prevNode = previousHash[i];
                    while (prevNode != nullptr)
                    {
                        unsigned int h = hasher(prevNode->key);
                        int mod = h % m_buckets;
                        
                        // insert new node
                        Node* n = new Node;
                        n->value = prevNode->value;
                        n->key = prevNode->key;
                        n->next = buckets[mod];
                        buckets[mod] = n;
                        
                        // delete previous hashtable's node
                        Node* kill = prevNode;
                        prevNode = prevNode->next;
                        delete kill;
                    }
                }
            }
            delete[] previousHash;
         }
        
          unsigned int h = hasher(key); //hash this key
            int mod = h % m_buckets;
            associations++; //increase assoications
         
        /*
         Node* p = buckets[mod];
         while(p->next != nullptr)
         {
             p = p->next;
         }
         Node* n = new Node;
         n->value = value;
         n->key = key;
         n->next = nullptr;
         p->next = n;
         */
            
            Node* n = new Node; //insert it
            n->value = value;
            n->key = key;
            n->next = buckets[mod];
            buckets[mod] = n;
    }
    
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
     unsigned int hasher(const KeyType& k); // prototype
      unsigned int h = hasher(key);
      int mod = h % m_buckets;
    
    if (buckets[mod] == nullptr)
    {
        return nullptr;
    }
    
    //iterate through this chosen linked list of nodes to find a certain value given a key
      Node* p = buckets[mod];
      while (p != nullptr)
      {
          if (p->key == key)
          {
              return &(p->value);
          }
          p = p->next;
      }
    
    //if not found
      return nullptr;
  
}

