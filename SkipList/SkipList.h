#include <vector>
#include <limits.h>
#include <random>
#include <iostream>

//Inspiration from https://stackoverflow.com/questions/31580869/skip-lists-are-they-really-performing-as-good-as-pugh-paper-claim
//At first I wanted to try and deviate more with working with less low-level data structures, but then there's a lot of issues with
//vectors deleting things when I don't want them to and such, so in the end I got kinda funneled into doing just what the post did ;_:
//At least I get their idea that they essentially want the nodes to just not be there in terms of points, but also really good pointer localities

//mem block for the nodes, taken from the stackoverflow post
class FixedAlloc
{
public:
    FixedAlloc(): root_block(0), free_element(0), type_size(0), block_size(0), block_num(0)
    {
    }

    FixedAlloc(int itype_size, int iblock_size): root_block(0), free_element(0), type_size(0), block_size(0), block_num(0)
    {
        init(itype_size, iblock_size);
    }

    ~FixedAlloc()
    {
        purge();
    }

    void init(int new_type_size, int new_block_size)
    {
        purge();
        block_size = std::max(new_block_size, type_size);
        type_size = std::max(new_type_size, static_cast<int>(sizeof(FreeElement)));
        block_num = block_size / type_size;
    }

    void purge()
    {
        while (root_block)
        {
            Block* block = root_block;
            root_block = root_block->next;
            free(block);
        }
        free_element = 0;
    }

    void* allocate()
    {
        if (free_element)
        {
            void* mem = free_element;
            free_element = free_element->next_element;
            return mem;
        }

        // Create new block.
        void* new_block_mem = malloc(sizeof(Block) - 1 + type_size * block_num);
        Block* new_block = static_cast<Block*>(new_block_mem);
        new_block->next = root_block;
        root_block = new_block;

        // Push all but one of the new block's elements to the free pool.
        char* mem = new_block->mem;
        for (int j=1; j < block_num; ++j)
        {
            FreeElement* element = reinterpret_cast<FreeElement*>(mem + j * type_size);
            element->next_element = free_element;
            free_element = element;
        }
        return mem;
    }

    void deallocate(void* mem)
    {
        FreeElement* element = static_cast<FreeElement*>(mem);
        element->next_element = free_element;
        free_element = element;
    }

private:
    struct Block
    {
        Block* next;
        char mem[1];
    };
    struct FreeElement
    {
        struct FreeElement* next_element;
    };

    // Disable copying.
    FixedAlloc(const FixedAlloc&);
    FixedAlloc& operator=(const FixedAlloc&);

    struct Block* root_block;
    struct FreeElement* free_element;
    int type_size;
    int block_size;
    int block_num;
};

//A vector that contains all the nodes in a specific level. Also needs to maintain funny stuffs with deletion..Is what I would like if vectors don't delete its own stuffs....
template<int cap>
class SkipList{
public:
    //contains what that item is, and what the item to the right at a specific level is
    struct Node{
        //The item
        int item;

        //The level
        int level;
        
        //Pointer to the node to the right
        //Their way of essentially just making every level a pointer in each index is just pure genius
        struct Node* right[1];
    };

    //A node that is freed. Will contain ref to other freed nodes if it exists
    Node* freed;

    //The leftmost Node. Will be useful for the skiplist to reference
    Node* left;
    
    //Making a node using their very cool memory block (so this code is essentially ctrl c ctrl v'd)
    Node* make_node(int level, int item){ 
        void* node_mem = allocs[level].allocate();
        Node* new_node = static_cast<Node*>(node_mem);
        new_node->item = item;
        new_node->level = level;
        for (int j=0; j < level+1; ++j)
            new_node->right[j] = 0;
        return new_node;
    }

    void destroy_node(Node* node)
    {
        allocs[node->level].deallocate(node);
    }
    
    //insert an item at this level in relation to what the node to the left of this node should be
    void insert(int item){
        Node* current = this->left;

        Node* updates[cap] = {0};

        //traverse the list at each level. If the right is higher/at the end, add node to be updated.
        for (int i=level; i >= 0; i--) 
        {
            while (current->right[i] != nullptr && current->right[i]->item < item){
                current = current->right[i];
            }
            updates[i] = current;
        }

        //insert if that item at the bottom is either a null node or node that's not this value already
        current = current->right[0];
        if (!current || current->item != item){
            //get the level
            int toplv = 0;
            while (toplv < cap-1 && std::rand() < RAND_MAX * 0.5){
                toplv++;
            }

            //if the randomized level is higher than the current lv, update current lv
            for (int i=level+1; i<=toplv; i++){
                updates[i] = left;
                level = i;
            }

            //make the node
            current = make_node(toplv, item);

            //insert at each stored update spots
            for (int i=toplv; i >= 0; i--) {
                current->right[i] = updates[i]->right[i];
                updates[i]->right[i] = current;
            }
        }
    }

    //The traversal code is just in the insert already... copy paste!
    bool search(int item){
        Node* current = this->left;
        //traverse the list at each level.
        for (int i=level; i >= 0; i--) 
        {
            while (current->right[i] != nullptr && current->right[i]->item < item){
                current = current->right[i];
            }
            if(current->right[i] && current->right[i]->item == item) return true;
        }

        //return true if that item at the bottom is either a null node or node that's not this value already
        current = current->right[0];
        return current && current->item == item;
    }

    //Q7.2 here. Implement bi-directional search
    bool bi_search(int item){
        Node* current = this->left;
        int i = 0;
        //traverse the list at each level. Starting from the bottom this time.
        //if we can go up, we go up.
        while (current->right[i] != nullptr && current->right[i]->item < item){
            if (current->level >= i+1 && current->right[i+1] && current->right[i+1]->item < item){
                current = current->right[i+1];
                i++;
            }
            else{
                current = current->right[i];
            }
        }

        //normal search downwards
        for (int downer=i; downer >= 0; downer--) 
        {
            while (current->right[downer] != nullptr && current->right[downer]->item < item){
                current = current->right[downer];
            }
            if(current->right[downer] && current->right[downer]->item == item) return true;
        }

        //return true if that item at the bottom is either a null node or node that's not this value already
        current = current->right[0];
        return current && current->item == item;
    }

    //erasing a node at this level that is to the right of this node
    void erase(int item){
        Node* current = this->left;

        Node* updates[cap] = {0};

        //traverse the list at each level. If the right is higher/at the end, add node to be updated.
        for (int i=level; i >= 0; i--) 
        {
            while (current->right[i] != nullptr && current->right[i]->item < item){
                current = current->right[i];
            }
            updates[i] = current;
        }

        //delete if that item at the bottom is either a null node or node that's not this value already
        current = current->right[0];
        if (current && current->item == item){
            //get the level
            int toplv = current->level;

            for (int i=0; i <= toplv; ++i) {
                updates[i]->right[i] = current->right[i];
            }

            //destroy
            destroy_node(current);

            //update max level as needed
            while (level > 0 && !left->right[level]){
                level--;
            }
        }
    }

    //construct list
    SkipList(){
        for (int j=0; j < cap; ++j){
            //Whoever posted that stackoverflow post must be really concious about space, who is this magical guy
            allocs[j].init(sizeof(Node) + (j+1)*sizeof(Node*), 4096);
        }
        left = make_node(cap-1, INT_MIN);
        level = 0;
    }

private:
    //node block
    FixedAlloc allocs[cap];

    //The highest level reached
    int level;
};