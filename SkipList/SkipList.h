#include <limits.h>
class Node{
public:
    //The item
    int item;

    //Pointer to the node to the right
    Node* right;

    //Pointer to the node below
    Node* below;

    Node(){
        item = INT_MIN;
        right = nullptr;
        below = nullptr;
    }

    Node(int newItem){
        item = newItem;
        right = nullptr;
        below = nullptr;
    }
};

class SkipList{
public:
    //inserts an item into a skip list
    void insert(int item);

    //search an item in that list, returns item
    void search(int key);

    //deletes an item in that index if it exists
    void erase(int index);

    //cap level
    int cap;

    //probability of growing
    float p_grow;

    //constructs the first node
    SkipList(){
        firstNode = Node();
        cap = 7;
        p_grow = 0.5;
    };

private:
    Node firstNode;
};