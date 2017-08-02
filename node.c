// 0000 file created by Yarib Nevarez @ Gdl IBM
#ifndef _NODE_C_
#define _NODE_C_

#include <memory.h>

#include    "node.h"
#include    "utilities.h"


Node * new_node(const NodeVTable * vtable)
{
    Node * node;
    
    node = new_buffer(sizeof(Node));

    ASSERT_TRACE(node != NULL, "node != NULL");

    if (node != NULL)
    {
        if (vtable != NULL)
        {
            node->vtable = vtable;

            if (node->vtable->duplicate_data != NULL)
            {
                node->vtable->duplicate_data(node, NULL);
            }
        }
    }

    return node;
}

void delete_node(Node * node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");

    if (node != NULL)
    {
        ASSERT_TRACE(node->parent == NULL, "node->parent == NULL");
        ASSERT_TRACE(node->first_child == NULL, "node->first_child == NULL");
        ASSERT_TRACE(node->prev == NULL, "node->prev == NULL");
        ASSERT_TRACE(node->next == NULL, "node->next == NULL");

        if ((node->vtable!= NULL) && (node->vtable->destroy_data != NULL))
        {
            node->vtable->destroy_data(node);
        }

        delete_buffer((void **) &node);
    }
}

Node * duplicate_tree(Node * tree)
{
    Node * copy_tree = NULL;

    if (tree != NULL)
    {
        copy_tree = new_node(NULL);

        ASSERT_TRACE(copy_tree != NULL, "copy_tree != NULL");

        if (copy_tree != NULL)
        {
            copy_tree->vtable = tree->vtable;

            if ((copy_tree->vtable != NULL) && (copy_tree->vtable->duplicate_data != NULL))
            {
                copy_tree->vtable->duplicate_data(copy_tree, tree);
            }

            ASSERT_TRACE(((tree->data != NULL) && (copy_tree->data != NULL)) || ((tree->data == NULL) && (copy_tree->data == NULL)),
                         "((tree->data != NULL) && (copy_tree->data != NULL)) || ((tree->data == NULL) && (copy_tree->data == NULL))");

            if (tree->first_child != NULL)
            {
                Node * subtree;
                Node * copy_subtree;

                for (subtree = tree->first_child; subtree != NULL; subtree = subtree->next)
                {
                    copy_subtree = duplicate_tree(subtree);

                    ASSERT_TRACE(copy_subtree != NULL, "copy_subtree != NULL");

                    append_child(copy_tree, copy_subtree);
                }
            }
        }
    }

    return copy_tree;
}

NavigationReturn navigate_tree(Node * node,
                               void * context,
                               NodeFunctionP through_node_down,
                               NodeFunctionP through_node_up,
                               NodeFunctionP through_leaf,
                               NodeFunctionP between_children)
{
    NavigationReturn nav_ret = NAV_ABORT;

    ASSERT_TRACE(node != NULL, "node != NULL");

    if (node != NULL)
    {
        nav_ret = NAV_CONTINUE;

        if (node->first_child != NULL)
        {
            Node * child;

            if (through_node_down != NULL)
            {
                nav_ret = through_node_down(node, context);
            }

            for (child = node->first_child;
                (nav_ret == NAV_CONTINUE) && (child != NULL);
                child = child->next)
            {
                if ((child != node->first_child) && (between_children != NULL))
                {
                    nav_ret = between_children(node, context);
                }

                if (nav_ret == NAV_CONTINUE)
                {
                    nav_ret = navigate_tree(child,
                                            context,
                                            through_node_down,
                                            through_node_up,
                                            through_leaf,
                                            between_children);
                }
            }

            if ((nav_ret != NAV_ABORT) && (through_node_up != NULL))
            {
                nav_ret = through_node_up(node, context);
            }
        }
        else if (through_leaf != NULL)
        {
            nav_ret = through_leaf(node, context);
        }
    }

    return nav_ret;
}

Node * navigate_tree_get_prev(Node * node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");

    // navigation algorithm
    if (node != NULL)
    {
        if (node->prev != NULL)
        {   // navigate through left
            node = node->prev;

            while ((node != NULL) && (node->first_child != NULL))
            {   // navigate through down
                node = node->first_child;

                while ((node != NULL) && (node->next != NULL))
                {   // navigate through right
                    node = node->next;
                }
            }
        }
        else
        {   // navigate through up
            node = node->parent;
        }
    }

    return node;
}

Node * navigate_tree_get_prev_vtable(Node * node, const NodeVTable * vtable)
{
    for (node = navigate_tree_get_prev(node);
        (node != NULL) && (node->vtable != vtable);
         node = navigate_tree_get_prev(node));

    return node;
}

Node * navigate_tree_get_next(Node * node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");

    // navigation algorithm
    if ((node != NULL) && (node->first_child != NULL))
    {   // navigate through down
        node = node->first_child;
    }
    else
    {
        while ((node != NULL)
            && (node->next == NULL))
        {   // navigate through up
            node = node->parent;
        }

        if (node != NULL)
        {   // navigate through right
            node = node->next;
        }
    }

    return node;
}

Node * navigate_tree_get_next_vtable(Node * node, const NodeVTable * vtable)
{
    for (node = navigate_tree_get_next(node);
        (node != NULL) && (node->vtable != vtable);
         node = navigate_tree_get_next(node));

    return node;
}

NavigationReturn destroy_subtree(Node * node, void * context)
{
    NavigationReturn nav_ret = NAV_ABORT;

    ASSERT_TRACE(node != NULL, "node != NULL");

    if (node != NULL)
    {
        Node * child;

        for (child = node->first_child; child != NULL; child = node->first_child)
        {
            ASSERT_TRACE(child->first_child == NULL, "child->first_child == NULL");

            detach_tree(child);
            delete_node(child);
        }

        ASSERT_TRACE(node->first_child == NULL, "node->first_child == NULL");

        nav_ret = NAV_CONTINUE;
    }

    return nav_ret;
}

void destroy_tree(Node * node)
{
    NavigationReturn nav_ret;

    if (node != NULL)
    {
        detach_tree(node);

        nav_ret = navigate_tree(node, NULL, NULL, destroy_subtree, NULL, NULL);

        ASSERT_TRACE(nav_ret != NAV_ABORT, "destroy_tree navigation fail");

        ASSERT_TRACE(node != NULL, "node != NULL");

        ASSERT_TRACE(node->first_child == NULL, "node->first_child == NULL");

        delete_node(node);
    }
}

Node * detach_tree(Node * node)
{
    ASSERT_TRACE(node != NULL, "node != NULL");

    if (node != NULL)
    {
        if (node->prev != NULL)
        {   // Update prev
            ASSERT_TRACE(node->prev->next == node, "node->prev->next == node");
            node->prev->next = node->next;
        }
        else if (node->parent != NULL)
        {   // Update parent
            ASSERT_TRACE(node->parent->first_child == node, "node->parent->first_child == node");
            node->parent->first_child = node->next;
        }

        if (node->next != NULL)
        {   // Update next
            ASSERT_TRACE(node->next->prev == node, "node->next->prev == node");
            node->next->prev = node->prev;
        }

        ASSERT_TRACE((node->prev == NULL) || (node->next == NULL) || (node->prev->next == node->next->prev),
                     "node->prev->next == node->next->prev");

        node->parent = NULL;
        node->prev = NULL;
        node->next = NULL;
    }

    return node;
}

Node * get_last_child(Node * parent)
{
    Node * node = NULL;

    ASSERT_TRACE(parent != NULL, "parent != NULL");

    if ((parent != NULL) && (parent->first_child != NULL))
    {
        node = parent->first_child;

        while (node->next != NULL)
        {
            node = node->next;
        }
    }

    return node;
}

void set_first_child(Node * parent, Node * first_child)
{
    ASSERT_TRACE(parent != NULL, "parent != NULL");

    if (parent != NULL)
    {
        while (parent->first_child != NULL)
        {
            destroy_tree(parent->first_child);
        }

        if (first_child != NULL)
        {
            detach_tree(first_child);
            first_child->parent = parent;
        }

        parent->first_child = first_child;
    }
}

void append_child(Node * parent, Node * child)
{
    ASSERT_TRACE(parent != NULL, "parent != NULL");
    ASSERT_TRACE(child != NULL, "child != NULL");

    if ((parent != NULL) && (child != NULL))
    {
        detach_tree(child);

        if (parent->first_child != NULL)
        {
            Node * node;

            node = get_last_child(parent);

            ASSERT_TRACE(node->next == NULL, "node->next == NULL");
            ASSERT_TRACE(node->parent == parent, "node->parent == parent");

            set_next(node, child);

            ASSERT_TRACE(node->next == child, "node->next == child");
            ASSERT_TRACE(node == child->prev, "node == child->prev");
        }
        else
        {
            set_first_child(parent, child);
            
            ASSERT_TRACE(parent->first_child == child, "parent->first_child == child");
            ASSERT_TRACE(child->next == NULL, "child->next == NULL");
        }

        ASSERT_TRACE(parent == child->parent, "parent == child->parent");
    }
}

void set_next(Node * node, Node * next)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE(next != NULL, "next != NULL");
    ASSERT_TRACE(node->parent != NULL, "node->parent != NULL");

    if ((node != NULL) && (next != NULL) && (node->parent != NULL))
    {
        detach_tree(next);

        next->parent = node->parent;
        next->prev = node;
        next->next = node->next;

        if (node->next != NULL)
        {
            node->next->prev = next;
        }

        node->next = next;
    }
}

void set_prev(Node * node, Node * prev)
{
    ASSERT_TRACE(node != NULL, "node != NULL");
    ASSERT_TRACE(prev != NULL, "prev != NULL");
    ASSERT_TRACE(node->parent != NULL, "node->parent != NULL");

    if ((node != NULL) && (prev != NULL) && (node->parent != NULL))
    {
        detach_tree(prev);

        prev->parent = node->parent;
        prev->prev = node->prev;
        prev->next = node;

        if (node->prev != NULL)
        {
            node->prev->next = prev;
        }
        else
        {
            node->parent->first_child = prev;
        }

        node->prev = prev;
    }
}

unsigned int get_node_index(Node * node)
{
    unsigned int node_index = (unsigned int)(-1);

    ASSERT_TRACE(node != NULL, "node != NULL");

    if (node != NULL)
    {
        node_index = 0;

        if (node->parent != NULL)
        {
            Node * child;

            for (child = node->parent->first_child;
                (child != NULL) && (child != node);
                child = child->next, node_index ++);
        }
    }

    return node_index;
}

Node * get_child_from_index(Node * parent, unsigned int node_index)
{
    Node * node = NULL;

    ASSERT_TRACE(parent != NULL, "parent != NULL");
    ASSERT_TRACE((parent != NULL) && (parent->first_child != NULL), "parent != NULL");
    ASSERT_TRACE(node_index != (unsigned int)(-1), "node_index != (unsigned int)(-1)");

    if ((parent != NULL)
        && (parent->first_child != NULL)
        && (node_index != (unsigned int)(-1)))
    {
        node = parent->first_child;

        while ((node_index > 0) && (node != NULL))
        {
            node = node->next;
            node_index --;
        }

        ASSERT_TRACE(node_index == 0, "Child index not found");
        ASSERT_TRACE(node != NULL, "Incorrect index of node");
    }

    return node;
}

NodePath * new_node_path(unsigned int depth)
{
    NodePath * node_path;

    node_path = (NodePath *) new_buffer(sizeof(NodePath));

    ASSERT_TRACE(node_path != NULL, "node_path != NULL");

    if ((node_path != NULL) && (depth > 0))
    {
        node_path->depth = depth;
        node_path->index = (unsigned int *) new_buffer(sizeof(unsigned int) * depth);

        ASSERT_TRACE(node_path->index != NULL, "node_path->index != NULL");
    }

    return node_path;
}

void delete_node_path(NodePath * node_path)
{
    if (node_path != NULL)
    {
        delete_buffer((void **) &(node_path->index));
        delete_buffer((void **) &(node_path));
    }
}

void node_path_insert_index(NodePath * node_path, unsigned int index)
{
    ASSERT_TRACE(node_path != NULL, "node_path != NULL");

    if (node_path != NULL)
    {
        node_path->index = realloc_buffer(node_path->index, sizeof(unsigned int) * (node_path->depth + 1));

        ASSERT_TRACE(node_path->index != NULL, "FATAL ERROR: node_path->index != NULL");

        if (node_path->index != NULL)
        {
            mem_move(&node_path->index[1], node_path->index, sizeof(unsigned int) * node_path->depth);

            *node_path->index = index;
            node_path->depth ++;
        }
    }
}

NodePath * obtain_node_path(Node * node)
{
    NodePath * node_path = NULL;

    ASSERT_TRACE(node != NULL, "node != NULL");

    if ((node != NULL) && (node->parent != NULL))
    {
        node_path = new_node_path(0);

        for (;node->parent != NULL; node = node->parent)
        {
            unsigned int index;

            index = get_node_index(node);

            ASSERT_TRACE(index != (unsigned int)(-1), "index != (unsigned int)(-1)");

            node_path_insert_index(node_path, index);
        }
    }

    return node_path;
}

NodePath * duplicate_node_path(NodePath * node_path)
{
    NodePath * copy_node_path = NULL;

    if (node_path != NULL)
    {
        copy_node_path = new_node_path(node_path->depth);
        
        ASSERT_TRACE(copy_node_path != NULL, "copy_node_path != NULL");

        if (copy_node_path != NULL)
        {
            mem_cpy((void *) copy_node_path->index,
                    (void *) node_path->index,
                    sizeof(int) * node_path->depth);

            ASSERT_TRACE(memcmp((void *) (copy_node_path->index), (void *) (node_path->index), sizeof(int) * node_path->depth) == 0,
                        "memcmp(copy_node_path->index, node_path->index, sizeof(int) * node_path->index) == 0");
        }
    }

    return copy_node_path;
}

Node * obtain_node_from_path(Node * root, NodePath * node_path)
{
    Node * node = NULL;

    ASSERT_TRACE(root != NULL, "root != NULL");
    ASSERT_TRACE((root == NULL) || (root->parent == NULL), "root->parent == NULL");
    ASSERT_TRACE((root == NULL) || (root->first_child != NULL), "root->first_child != NULL");
    
    ASSERT_TRACE(node_path != NULL, "node_path != NULL");
    ASSERT_TRACE((node_path == NULL) || (node_path->depth > 0), "node_path->depth > 0");
    ASSERT_TRACE((node_path == NULL) || (node_path->index != NULL), "node_path->index != NULL");

    if ((root != NULL)
        && (root->first_child != NULL)
        && (node_path != NULL)
        && (node_path->depth > 0)
        && (node_path->index != NULL))
    {
        unsigned int depth;

        node = root;

        for (depth = 0; (depth < node_path->depth) && (node != NULL); depth ++)
        {
            node = get_child_from_index(node, node_path->index[depth]);
        }

        ASSERT_TRACE(node != NULL, "Wrong node_path or root");
    }

    return node;
}

#endif // _NODE_C_
