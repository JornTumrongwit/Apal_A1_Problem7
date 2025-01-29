#include <limits.h>
class Node{
public:
    //The item
    int item;

    //Pointer to the node to the right
    Node* right;

    //Pointer to the node below
    Node* below;

    //the level of this node
    int level;
    
    Node(){
        item = INT_MIN;
        right = nullptr;
        below = nullptr;
        level = 1;
    }

    Node(int newItem){
        item = newItem;
        right = nullptr;
        below = nullptr;
        level = 1;
    }
};

class SkipList{
public:
    //inserts an item into a skip list
    void insert(int item);

    //search an item in that list, returns item
    int search(int key);

    //deletes an item in that index if it exists
    void erase(int index);

    //sets a new probability
    void p_set(float p);

    //displaying the skip list in a very janky way just for debugging
    void debug_print();

    //cap level
    int cap;

    //constructs the first node
    SkipList(){
        firstNode = new Node();
        cap = 7;
        p_set(0.5);
    };

private:
    Node* firstNode;
    //probability of growing
    float p_grow;

    //helper
    int p_help;
};