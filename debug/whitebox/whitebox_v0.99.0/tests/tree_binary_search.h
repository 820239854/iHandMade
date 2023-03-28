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

/* Vec2 max_leaf_size = { 3, 6 }; */


// takes an array of branches of different species, each with sorted arrays of leaves
// finds and trims all leaves of the given species larger than the specified area
void trim_leaves_of_species(Branch const *branches, int n, Species species, float trim_area) {

    // go through each branch
    for (int i = n; i-- > 0;) {
        if (branches[i].species == species) {
            Leaf *first_leaf_to_trim = 0;

            // binary search to find which leaf to start trimming at
            for (Leaf *leaf = branches[i].leaves; leaf; leaf = leaf->next) {
                float leaf_area    = area_of_leaf(leaf);
                float largest_area = area_of_leaf(largest_leaf);

                if (leaf_area > largest_area)
                    largest_leaf = leaf;
            }

            // invariant: (lo_i <= i < hi_i) if first[i] == val
            // bound time: (hi_i - lo_i)
            for (Leaf *lo = 0, *hi = branches[i].leaves + branches[i].leaves_n;
                 lo < hi;) {
                Size i        = (hi_i + lo_i) / 2; // TODO: if (hi_i - lo_i) is below a certain size is it faster to just linear search the remaining items?
                U8   test_val = *stride_to_i_U8(first, stride, i);

                if (test_val < val)
                {   lo_i = i + 1;   } // look at the top half next time

                else if (val < test_val)
                {   hi_i = i;   } // look at the bottom half next time

                else
                {   result = i; break;   }
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


/* void example() */
/* { */
/* Leaf oaks[]    = { {{2,3}},     {{5.8,7},&oaks[0]},}; */
/* Leaf maples[]  = { {{3.2,1.9}}, {{1.6,5.9},&maples[0]}, {{1.8,5.9},&maples[1]} }; */
/* Leaf beeches[] = { {{5.8,7}},   {{1.8,5.9},&beeches[0]}, {{2.4,3},&beeches[1]} }; */
/* Leaf maples2[] = { {{5.2,1.9}} }; */

/* Branch branches[] = { {oak,oaks+1}, {maple,maples+2}, {beech,beeches+2}, {maple,maples2} }; */
/* trim_largest_leaf_of_species(branches, 4, oak); */
/* } */



/* float largest_leaf_of_species(Branch *root, int n, Species species, Leaf *leaf_out) { */

/* void */
/* example_code() { */
/*     Leaf leaves[] = { {oak, {3.f,2.1f}, }, {oak, {3.f,2.1f}, } }; */

/* #define countof(a) (sizeof(a)/sizeof(*(a))) */
/*     for (int i = 0; i < 12) */
/* } */

/* internal Size */
/* bin_search_U8(U8 const *first, Size n, Size stride, U8 val) */
/* { */
/*     assert(n == 0 || first); */

/*     Size result = ~(Size)0, */
/*          lo_i   = 0, */
/*          hi_i   = n; */

/*     // invariant: (lo_i <= i < hi_i) if first[i] == val */
/*     // bound time: (hi_i - lo_i) */
/*     while (lo_i < hi_i) */
/*     { */
/*         Size i        = (hi_i + lo_i) / 2; // TODO: if (hi_i - lo_i) is below a certain size is it faster to just linear search the remaining items? */
/*         U8   test_val = *stride_to_i_U8(first, stride, i); */

/*         if (test_val < val) */
/*         {   lo_i = i + 1;   } // look at the top half next time */

/*         else if (val < test_val) */
/*         {   hi_i = i;   } // look at the bottom half next time */

/*         else */
/*         {   result = i; break;   } */
/*     } */

/*     assert(!~result || (lo_i <= result && result < hi_i), "result should be within half-open bounds"); */
/*     assert(!~result || *stride_to_i_U8(first, stride, result) == val); */
/*     return result; */
/* } */

