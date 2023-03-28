enum Species  { oak, maple, beech };
struct Vec2   { float x, y; };
struct Leaf   { Vec2 size; };
struct Branch {
    Species  species;
    Leaf    *leaves;
    int      leaves_n;
};

float area_of_leaf(Leaf leaf) { // Rectangular approximation is close enough
    float area = leaf.size.x * leaf.size.y;
    return area;
}

// Takes an array of branches of different species, each with sorted arrays of leaves
// Finds and trims all leaves of the given species larger than the specified area
void trim_leaves_of_species(Branch const *branches, int n, Species species, float trim_area) {
    // Go through each branch to find those with the right species
    for (int branch_i = n; branch_i-- > 0;) {
        Branch const *branch = &branches[branch_i];

        if (branch->species == species) {
            Leaf *leaves           = branch->leaves;
            Leaf *leaves_to_trim   = nullptr;
            int   leaves_to_trim_n = 0;
            for (int i = 0; i < leaves_to_trim_n; ++i) { // Trim leaves only in the y axis
                leaves[i].size.y = trim_area * leaves[i].size.x;
            }
        }
    }
}






            /* // go through each leaf on the branch */
            /* for (Leaf *leaf = branches[i].leaves; leaf; leaf = leaf->next) { */
            /*     float leaf_area    = area_of_leaf(leaf); */
            /*     float largest_area = area_of_leaf(largest_leaf); */

            /*     if (leaf_area > largest_area) */
            /*         largest_leaf = leaf; */
/*     // trim the leaf to size */
/*     if (largest_leaf) { */
/*         if (largest_leaf->size.x > max_leaf_size.x) */
/*             largest_leaf->size.x = max_leaf_size.x; */
/*         if (largest_leaf->size.y > max_leaf_size.y) */
/*             largest_leaf->size.y = max_leaf_size.y; */
/*     } */






// TODO: binary search


/* /1* void example() *1/ */
/* { */
/* Leaf oaks[]    = { {2,3},     {5.8,7}              }; */
/* Leaf maples[]  = { {3.2,1.9}, {1.6,5.9}, {1.8,5.9} }; */
/* Leaf beeches[] = { {2.4,3},   {1.8,5.9}, {5.8,7}   }; */
/* Leaf maples2[] = { {5.2,1.9}                       }; */

/* Branch branches[] = { {oak,oaks,2}, {maple,maples,3}, {beech,beeches,3}, {maple,maples2,1} }; */
/* trim_leaves_of_species(branches, 4, oak, 10); */
/* } */



/* float largest_leaf_of_species(Branch *root, int n, Species species, Leaf *leaf_out) { */

/* void */
/* example_code() { */
/*     Leaf leaves[] = { {oak, {3.f,2.1f}, }, {oak, {3.f,2.1f}, } }; */

/* #define countof(a) (sizeof(a)/sizeof(*(a))) */
/*     for (int i = 0; i < 12) */
/* } */

