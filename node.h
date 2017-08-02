// 0000 file created by Yarib Nevarez @ Gdl IBM
#ifndef _NODE_H_
#define _NODE_H_


/******************************************************************************
 Node : Object used to create any shape of data structures.
******************************************************************************/
typedef struct _Node Node;

/******************************************************************************
 NodePath : Struct created to store the path of a particular node inside of
            a tree.
 index :    Array of numbers to indicate the child inside of the subtree
            through the levels of the complete tree.
 depth :    Indicates the number of levels to hit the node inside of the tree.
******************************************************************************/
typedef struct
{
    unsigned int *  index;
    unsigned int    depth;
} NodePath;

/******************************************************************************
 NodeData : Object pointer used to store any type of data inside of a node.
******************************************************************************/
typedef void * NodeData;

/******************************************************************************
 DuplicateData :    Pointer type signature for any function created to
                    duplicate the data of a particular object node.
******************************************************************************/
typedef void    (*DuplicateData)(Node *, Node *);

/******************************************************************************
 DestroyData :  Pointer type signature for any function created to
                destroy the data of a particular object node.
******************************************************************************/
typedef void    (*DestroyData)(Node *);

/******************************************************************************
 NODE_VTABLE_MEMBERS :  Macro definition for the function members of the
                        virtual function table for the abstract node object.
                        In this definition add any new required function for
                        abstract node object.
******************************************************************************/
#define NODE_VTABLE_MEMBERS         \
    DuplicateData   duplicate_data; \
    DestroyData     destroy_data;

/******************************************************************************
 NodeVTable :   Struct to store the virtual functions of the node object.
                The function virtual table defines the behavior and also the
                type of the node.
                This Virtual table needs to be defined since a node without a
                virtual table is an abstract object.
******************************************************************************/
typedef struct
{
    NODE_VTABLE_MEMBERS
} NodeVTable;

/******************************************************************************
 _Node :        Structure to create the abstract node object.
 vtable :       Member to access the virtual functions members of the node object.
 data :         Pointer to the partucular node data.
 parent :       Pointer to the parent node if it's different to NULL.
 first_child :  Pointer to the first child node if it's different to NULL.
 prev :         Pointer to the previous node if it's different to NULL.
 next :         Pointer to the nex node if it's different to NULL.
******************************************************************************/
struct _Node
{
    const NodeVTable *  vtable;
    NodeData            data;

    Node *              parent;
    Node *              first_child;
    Node *              prev;
    Node *              next;
};

/******************************************************************************
 new_node : Create and initialize a node and returns its pointer.
 vtable :   Pointer to the virtual table thats define the type of node.
******************************************************************************/
Node * new_node(const NodeVTable * vtable);

/******************************************************************************
 destroy_tree : Detach the given node from its tree and delete the node or
                subtree (Also delete the data of the given node or subtree)
 ndoe :         Node or subtree thats will be deleted.
******************************************************************************/
void destroy_tree(Node * node);

/******************************************************************************
 detach_tree :  Detach or separate the given node or subtree from its tree.
 node :        Node or subtree which will be desconnected form its tree.
******************************************************************************/
Node * detach_tree(Node * node);

/******************************************************************************
 set_first_child :  Function to connects the given first_child node as the
                    first node of the given parent node. The nodes will be
                    conected to each other acording to the hierarchy roles.
                    child of the given parent node.
 parent :           Node that will take the role as a parent node.
 first_child :      Node that will take the role as a child of the given parent
                    node.
******************************************************************************/
void set_first_child(Node * parent, Node * first_child);

/******************************************************************************
 append_child : Function to add a node to the given parent node. The given
                child node will be added as the last child.
 parent :       Node that will has the role as a parent node.
 child :        Node that will take the role as the last child of the given
                parent node.
******************************************************************************/
void append_child(Node * parent, Node * child);

/******************************************************************************
 set_next : Function to connect the given next node as the next node of the
            given node.
 node :     Node that will be the prev node of the given next node.
 next :     Node that will be the next node.
******************************************************************************/
void set_next(Node * node, Node * next);

/******************************************************************************
 set_prev : Function to connect the given prev node as the previous node of the
            given node.
 node :     Node that will be the next node of the given prev node.
 prev :     Node that will be the previous node.
******************************************************************************/
void set_prev(Node * node, Node * prev);

/******************************************************************************
 get_last_child :   Function to obtain the last child of a given parent node,
                    return NULL if the parent node does not have any child.
 parent :           Node that is needed to obtain its last child.
******************************************************************************/
Node * get_last_child(Node * parent);

/******************************************************************************
 get_node_index :   Function to obtain the number of child of the given node
                    from its parent. The given node needs to have parent.
 node :             Node wich is needed to obtain its index.
******************************************************************************/
unsigned int get_node_index(Node * node);

/******************************************************************************
 duplicate_tree :   Function to duplicate any given tree, the duplicated tree
                    will be have identical structure and identical data
                    duplicated.
 tree :             The tree, subtree or node that will be duplicated.
******************************************************************************/
Node * duplicate_tree(Node * tree);

/******************************************************************************
 obtain_node_path : Function to create the NodePath data that is used to obtain
                    any particular node from its root, the NodePath data
                    returned needs to be deleted with delete_node_path.
 node :             Node wich is needed to obtain its path from its tree root.
******************************************************************************/
NodePath * obtain_node_path(Node * node);

/******************************************************************************
 duplicate_node_path :  As its name says, duplicate the node path.
 node_path :            It's the path of the node that will be duplicated.
******************************************************************************/
NodePath * duplicate_node_path(NodePath * node_path);

/******************************************************************************
 obtain_node_from_path :    Function to obtain the node from the given root
                            the returned node will be obtained according to its
                            given path.
 root :                     The relative tree root where the node will be
                            searched according to its given node path.
 node_path :                Data that contains the information that is needed
                            to obtain a particular node inside of the tree or
                            subtree.
******************************************************************************/
Node * obtain_node_from_path(Node * root, NodePath * node_path);

/******************************************************************************
 delete_node_path :     As its name says, delete the complete data of the path,
                        this function avoids memory leakages.
 node_path :            Node path data that will be deleted.
******************************************************************************/
void delete_node_path(NodePath * node_path);

/******************************************************************************
 NavigationReturn :     Enumeration defined to be used as a return value type
                        to control the navigation through the given tree.
 NAV_CONTINUE :         Returning this value the navigation engine will be
                        working as normally.
 NAV_ABORT :            Returning this value the navigation engine will abort
                        the navigation through the given tree.
 NAV_SKIP_CHILDREN :    Returning this value the navigation engine will skip
                        the children of the current node on the navigation.
******************************************************************************/
typedef enum
{
    NAV_CONTINUE,
    NAV_ABORT,
    NAV_SKIP_CHILDREN
} NavigationReturn;

/******************************************************************************
 NodeFunctionP :    Function type signature used as a type of the functions
                    used to navigate through any given tree.
                    Any function created according to this signature will
                    receive the current node (current position of the
                    navigation engine) and the second parameter is a comon
                    pointer used to access any data nedded on the navigation.
******************************************************************************/
typedef NavigationReturn (*NodeFunctionP)(Node *, void *);

/******************************************************************************
 navigate_tree :        As its name says, this function is used to navigate
                        through any given node, this function contains the
                        navigation engine or algorithm.
 node :                 Node that is any relative or absolute tree root that
                        needs to be navigated through its nodes.
 conext :               Is the pointer of any type of data or structure, this
                        pointer will be received in all the navigation points,
                        to obtain the data that his pointer represents this
                        pointer oonly needs to ve casted in the functions
                        navigation point.
 through_node_down :    Pointer of a function that will be called when the
                        navigation algorithm is navigating throug a node that
                        contains children, and this function is called before
                        to achieve or navigate any child of this current node.
 through_node_up :      Pointer of a function that will be called when the
                        navigation algorithm is navigating through a completed
                        navigated subtree, so all the children nodes has been
                        already navigated and the navigation is completed in
                        that subtree.
 Through_leaf :         Pointer of a function that will be called when the
                        navigation algorithm is navigating when the algorithm
                        hits a node that does not have any child.
 between_children :     Pointer of a function that will be called when the
                        navigation algorithm hits any node that is not a first
                        child.
******************************************************************************/
NavigationReturn navigate_tree(Node * node,
                               void * context,
                               NodeFunctionP through_node_down,
                               NodeFunctionP through_node_up,
                               NodeFunctionP through_leaf,
                               NodeFunctionP between_children);

/******************************************************************************
 navigate_tree_get_prev :   Function that returns the graphically previous node
                            in the tree of a given node.
 node :                     Node that is needed to get its previous node.
******************************************************************************/
Node * navigate_tree_get_prev(Node * node);

/******************************************************************************
 navigate_tree_get_next :   Function that returns the graphically next node
                            in the tree of a given node.
 node :                     Node that is needed to get its next node.
******************************************************************************/
Node * navigate_tree_get_next(Node * node);

/******************************************************************************
 navigate_tree_get_prev_vtable :    Function that returns the graphically
                                    previous node that has the given vtabe in
                                    the tree of a given node.
 node :                             Node that is needed to get its previous
                                    node.
 vtable :                           Pointer of the virtual function table that
                                    will be have the returned node. This vtable
                                    define the type of the node and its
                                    behavior also.
******************************************************************************/
Node * navigate_tree_get_prev_vtable(Node * node, const NodeVTable * vtable);

/******************************************************************************
 navigate_tree_get_next_vtable :    Function that returns the graphically
                                    next node that has the given vtabe in the
                                    tree of a given node.
 node :                             Node that is needed to get its next
                                    node.
 vtable :                           Pointer of the virtual function table that
                                    will be have the returned node. This vtable
                                    define the type of the node and its
                                    behavior also.
******************************************************************************/
Node * navigate_tree_get_next_vtable(Node * node, const NodeVTable * vtable);

#endif // _NODE_H_
