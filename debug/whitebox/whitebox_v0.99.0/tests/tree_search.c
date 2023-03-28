// leaves in a collector's museum
typedef enum   { oak, sycamore, maple, beech } Species;
typedef struct { float x, y; } V2;
typedef struct Leaf Leaf; struct Leaf {
    Species species;
    float   chlorophyll_level;
    V2      dimensions;
    Leaf *
} Leaf;

typedef struct Tree {

} ;


Leaf *
tree_depth_first_search_for_leaf()
{
    Life *it = *life_it;
    if (it == NULL) // first time
    {   it = root;   }

    else if (! skip_children && it->last_child)
    { // down
        it = it->last_child;
        ++*depth;
    }

    else for (Life *at = it; ; at = at->parent, --*depth)
    { // get next unhandled sibling or if there are parents try the level above
        if (! at->parent || at == root) // hit absolute/sub root
        {   it = NULL; break;   }

        assert(at->parent->last_child && "parent-child relationship not held");
        if (at->prev != at->parent->last_child)
        {   it = at->prev; break;   } // across
    }

    return (*life_it = it);
}


