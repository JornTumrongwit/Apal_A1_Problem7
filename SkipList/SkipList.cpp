#include <iostream>
#include "SkipList.h"
#include <random>
#include <map>
#include <set>
#include <sstream>

void SkipList::p_set(float p){
    p_grow = p;
    p_help = p_grow * RAND_MAX;
}

void SkipList::insert(int item){
    //Everytime an item is inserted, that item gets a coin flip. If passes, we let that item get one higher level
    int levels = 1;
    int rng = rand();
    while (rng < p_help && levels <= cap){
        levels++;
        rng = rand();
    }

    //Set the leftmost column to the highest level
    while (firstNode->level < levels) {
        Node* newLeftmost = new Node();
        newLeftmost->level = firstNode->level + 1;
        newLeftmost->below = firstNode;
        firstNode = newLeftmost;
    }

    Node* currentLeftmost = firstNode;
    Node* prevNode = nullptr;
    //update each level
    while (currentLeftmost != nullptr){
        if(currentLeftmost->level <= levels){
            Node* current = currentLeftmost;
            while(current->right != nullptr && current->right->item < item) {
                current = current->right;
                if(current->item==item){
                    //if item is here, just break, everything lower should be here as well
                    return;
                }
            }

            //make the new node
            Node* newNode = new Node(item);
            //std::printf("%d test item\n", newNode.item);
            
            newNode->level = currentLeftmost->level;
            newNode->right = current->right;

            //pointing the current node to this new node
            current->right = newNode;

            if(prevNode != nullptr) prevNode->below = newNode;

            //set the previous node to this node
            prevNode = newNode;
        }
        //update working level
        currentLeftmost = currentLeftmost->below;
        //if(currentLeftmost->level != 1) std::printf("%d test level\n", currentLeftmost->level);
    }

    //Should be done by this point
    return;
}

void SkipList::erase(int item){
    //erase that item and adjust each level accordingly
    Node* currentLeftmost = firstNode;
    //go down each level
    while (currentLeftmost != nullptr){
        Node* current = currentLeftmost;
        //search until find item
        while(current->right != nullptr){
            //if item found, erase
            if(current->right->item == item){
                Node* thisItem = current->right;
                current->right = current->right->right;
                delete thisItem;
                break;
            }
            current = current->right;
        }
        currentLeftmost = currentLeftmost->below;
    }

    //handling the deletion of empty levels
    currentLeftmost = firstNode;
    while (currentLeftmost != nullptr && currentLeftmost->right == nullptr){
        firstNode = currentLeftmost->below;
        Node* oldLv = currentLeftmost;
        delete oldLv;
        currentLeftmost = firstNode;
    }
}

int SkipList::search(int item){
    //traverse the skip list to find the item
    int index = -1;
    Node* current = firstNode;
    while (current != nullptr){
        if(current->right == nullptr) {
            current = current->below;
            continue;
        }
        if(current->right->item == item) return true;
        else if (current->right->item > item){
            current = current->below;
        }
        else{
            current = current->right;
        }
    }
    return false;
}

//helper for printing
std::string ZeroPadNumber(int num, int pad)
{
	std::stringstream ss;
	
	// the number is converted to string with the help of stringstream
	ss << num; 
	std::string ret;
	ss >> ret;
	
	// Append zero chars
	int str_length = ret.length();
	for (int i = 0; i < pad - str_length; i++)
		ret = " " + ret;
	return ret;
}

void SkipList::debug_print(){
    //print all items in each level
    std::map<int, std::set<int>> item;
    std::set<int> items;
    //print all items in each level
    Node* currentleft = firstNode;
    while(currentleft != nullptr){
        Node* current = currentleft->right;
        while(current != nullptr){
            items.insert(current->item);
            item[current->level].insert(current->item);
            current = current->right;
        }
        currentleft = currentleft->below;
    }

    //print each level
    for(int level = firstNode->level; level > 0; level--){
        std::set<int>::iterator i;
        for (i = items.begin(); i != items.end(); ++i){
            if (item[level].count(*i)) std::cout << ("%s", ZeroPadNumber(*i, 3));
            else std::printf("   ");
            std::printf(" --- ");
        }
        std::printf("\n");
    }
}

// int main(){
//     SkipList list = SkipList();
//     for(int i = 0; i < 30; i++){
//         list.insert(i);
//     }
//     std::printf("debugging\n");
//     list.debug_print();
//     list.erase(21);
//     list.debug_print();
//     std::printf("%d\n", list.search(21));
// }