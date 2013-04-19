/*
 * Contains the the apply function, the cofactors and quantification functions.
 *
 * For Project 1, implement
 * (1) apply, also handles probabilities
 * (2) negative_cofactor
 * (3) positive_cofactor, 
 * (4) boolean_difference
 * (5) sort by influence
 */

#include "project1.h"
#include "bdd_node.h"
#include <vector>
#include <set>
#include <algorithm>

using namespace std;



// wrapper function to allow calling with the operation, i.e apply(bdd1, bdd2, "or")
bdd_ptr apply(bdd_ptr bdd1, bdd_ptr bdd2, string o)
{
  operation dop;
  if (!dop.set_operation(o))
  {
    return 0;
  }
  
  return apply(bdd1, bdd2, dop);
}

// apply implements an arbitrary operation (specified in op) on two BDDs
// bdd_tables is used to handle the book keeping (see bdd_tables.h).
//
// apply works recursively one the idea that given an arbitrary operation $, 
// and functions f and g: f $ g = a'(fa' $ ga') + a(fa $ ga), 
// where a is a variable, fa' is the negative cofactor etc.
bdd_ptr apply(bdd_ptr bdd1, bdd_ptr bdd2, operation &op)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();


  // ... your code goes here

  // Base cases
  bdd_ptr ret;
  if ((ret = tables.find_in_computed_table(op.get_operation(), bdd1, bdd2))) {
      return ret;
  }

  if ((ret = op(bdd1, bdd2))) {
      return ret;
  }


  // end of base cases

  char var = find_next_var(bdd1, bdd2);

  bdd_ptr bdd1_nc = negative_cofactor(bdd1, var);
  bdd_ptr bdd1_pc = positive_cofactor(bdd1, var);
  bdd_ptr bdd2_nc = negative_cofactor(bdd2, var);
  bdd_ptr bdd2_pc = positive_cofactor(bdd2, var);

  bdd_ptr left = apply(bdd1_nc, bdd2_nc, op);
  bdd_ptr right = apply(bdd1_pc, bdd2_pc, op);

  
  if (left == right) {  // no need to create a new node
      ret = left;
  }
  else {
      ret = tables.find_in_unique_table(var, left, right);
      if (ret == 0) {
          ret = tables.create_and_add_to_unique_table(var, left, right);
          ret->probability = left->probability * 0.5 + right->probability * 0.5;
      }
  }

  tables.insert_computed_table(op.get_operation(), bdd1, bdd2, ret);


  // change the return value when you're finished
  return ret;
}

// negative_cofactor takes the BDD pointed to by np, 
// and returns the negative cofactor with respect to var.
bdd_ptr negative_cofactor(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  
  // Base case
  if (np == bdd_node::one) {
      return bdd_node::one;
  }

  if (np == bdd_node::zero) {
      return bdd_node::zero;
  }

  if (np->var == var) {
      return np->neg_cf;
  }


  // recursive case
  bdd_ptr left = negative_cofactor(np->neg_cf, var);
  bdd_ptr right = negative_cofactor(np->pos_cf, var);

  if (left == right)
      return left;

  bdd_ptr ret;
  ret = tables.find_in_unique_table(np->var, left, right);
  if (ret == 0) {
      ret = tables.create_and_add_to_unique_table(np->var, left, right);
      ret->probability = left->probability * 0.5 + right->probability * 0.5;
  }

  // once you add your code, remove this and return your own result
  return ret; 
}

// posative_cofactor takes the BDD pointed to by np, 
// and returns the posative cofactor with respect to var.
bdd_ptr positive_cofactor(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  
  // Base case
  if (np == bdd_node::one) {
      return bdd_node::one;
  }

  if (np == bdd_node::zero) {
      return bdd_node::zero;
  }

  if (np->var == var) {
      return np->pos_cf;
  }

  // recursive case
  bdd_ptr left = positive_cofactor(np->neg_cf, var);
  bdd_ptr right = positive_cofactor(np->pos_cf, var);

  if (left == right)
      return left;

  bdd_ptr ret;
  ret = tables.find_in_unique_table(np->var, left, right);
  if (ret == 0) {
      ret = tables.create_and_add_to_unique_table(np->var, left, right);
      ret->probability = left->probability * 0.5 + right->probability * 0.5;
  }

  // once you add your code, remove this and return your own result
  return ret; 
}

// boolean_difference takes the BDD pointed to by np, 
// and returns the boolean difference with respect to var.
bdd_ptr boolean_difference(bdd_ptr np, char var)
{
  // get reference to tables
  //bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  bdd_ptr ret = apply(negative_cofactor(np, var), positive_cofactor(np, var), "xor");
  
  // once you add your code, remove this and return your own result
  return ret; 
}


// This is sub functions used by sort_by_influence()
void extract_vars(bdd_ptr np, set<char> &vars) {
    if (np->is_terminal()) {
        return;
    }

    vars.insert(np->var);

    extract_vars(np->neg_cf, vars);
    extract_vars(np->pos_cf, vars);
}

struct influence_entry {
    char var;
    float influence;

    influence_entry(char var, float influence) : var(var), influence(influence) {}
};

bool compare_entry(influence_entry e1, influence_entry e2) {
    return (e1.influence > e2.influence);
}

// sort_by_influence calculates the influence of all the variables in np
// and displays them in descending order (most influent variable is
// shown first). For this task you can assume the number of variable
// is no greater than 20.
bdd_ptr sort_by_influence(bdd_ptr np)
{
    //... your code goes here

    set<char> vars;
    extract_vars(np, vars);

    vector<influence_entry> infs;

    for (set<char>::iterator it=vars.begin(); it!=vars.end(); ++it) {
        bdd_ptr bdiff = boolean_difference(np, *it);
        influence_entry entry(*it, bdiff->probability);
        infs.push_back(entry);
    }

    sort(infs.begin(), infs.end(), compare_entry);

    for (vector<influence_entry>::iterator it = infs.begin(); it != infs.end(); ++it)
        cout << it->var << ',' << it->influence << endl;

    // this function does not alter the current node, so np must be
    // returned at the end
    return np; 
}


