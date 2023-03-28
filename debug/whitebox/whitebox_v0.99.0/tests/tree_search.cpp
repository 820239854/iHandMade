enum Species  { oak, maple, beech };
struct Vec2   { float x, y; };
struct Leaf   {
    Vec2    size;
    Leaf *next;
};
struct Branch {
    Species species;
    Leaf *leaves;
    int leaves_n;
};

int area_of_leaf(Leaf *leaf) {
    float area = 0.f;
    if (leaf)
        area = leaf->size.x * leaf->size.y;
    return area;
}

Vec2 max_leaf_size = { 3, 6 };


// finds and trims the largest leaf of the given species
void trim_leaf_of_species(Branch *branches, int n, Species species) {
    Leaf *largest_leaf = 0;

    // go through each branch
    for (int i = n; i-- > 0;) {
        if (branches[i].species == species) {
            // go through each leaf on the branch
            for (Leaf *leaf = branches[i].leaf_list; leaf; leaf = leaf->next) {
                float leaf_area    = area_of_leaf(leaf);
                float largest_area = area_of_leaf(largest_leaf);

                if (leaf_area > largest_area)
                    largest_leaf = leaf;
            }
        }
    }

    // trim the leaf to size
    if (largest_leaf) {
        if (largest_leaf->size.x > max_leaf_size.x)
            largest_leaf->size.x = max_leaf_size.x;
        if (largest_leaf->size.y > max_leaf_size.y)
            largest_leaf->size.y = max_leaf_size.y;
    }
}






// TODO: binary search


void example()
{
Leaf oaks[]    = { {{2,3}},     {{5.8,7},&oaks[0]},};
Leaf maples[]  = { {{3.2,1.9}}, {{1.6,5.9},&maples[0]}, {{1.8,5.9},&maples[1]} };
Leaf beeches[] = { {{5.8,7}},   {{1.8,5.9},&beeches[0]}, {{2.4,3},&beeches[1]} };
Leaf maples2[] = { {{5.2,1.9}} };

Branch branches[] = { {oak,oaks+1}, {maple,maples+2}, {beech,beeches+2}, {maple,maples2} };
trim_largest_leaf_of_species(branches, 4, oak);
}



/* float largest_leaf_of_species(Branch *root, int n, Species species, Leaf *leaf_out) { */

/* void */
/* example_code() { */
/*     Leaf leaves[] = { {oak, {3.f,2.1f}, }, {oak, {3.f,2.1f}, } }; */

/* #define countof(a) (sizeof(a)/sizeof(*(a))) */
/*     for (int i = 0; i < 12) */
/* } */

