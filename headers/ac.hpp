#ifndef ac_h
#define ac_h
#include "mce_setup.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>

using namespace std;

/**************************************************************
 Function: ac_procedure
 Description: This function is used to implement an instance of
 the ac_procedure (adding an edge) as described by cavallo and
 klir. First we look for a valid place to add the edge - this is
 done in the membership array. We are looking for a place that
 is not already an edge, loop (x=y), or an edge who's inverse is
 true ((y,x) = true). Then we need to see if the new pair interacts
 with any of the old edges. This is done by taking the intersection
 of the new edge with each of the current cliques. If the edge
 interacts with any of the cliques then we put it in a buffer that
 contains the intersection of every clique that the edge interacts
 with. Once we have checked every clique we union the new edge with
 the intersection of each clique to get the final edge we have been
 looking for. Once we have found the new clique we must check that
 there are no other cliques that are a subset of the new. If there
 are any cliques that are a subset of the new then they are excluded
 from the aggregate. This process is then repeated for each valid
 edge in the graph.
 
 Dictionary of Variables:
 membership_array -
 type: vector <vector <bool> >
 Description: input to function, contains the matrix representation
 of the graph description file - ie what edges are valid
 maximal_cliques -
 type: vector <vector <int> >
 Description: input to function, contains a set of vectors of type
 int that represent the set of maximal cliques for the graph
 iterations -
 type: int
 Description: local to function, keeps track of the number of edges
 that we have been adding to the graph
 new_edge -
 type: pair<int,int>
 Description: local to function, keeps track of each pair we have
 added into the graph
 clique -
 type: vector<int>
 Description: local to function - the clique from the full set of
 maximal cliques that we are working with
 edge -
 type: vector<int>
 Description: local to function - the edge from the full set of
 edges that we are working with
 final_edge -
 type: vector<int>
 Description: local to function - the vector that will hold the final
 edge that we have found
 buffer -
 type: vector<int>
 Description: local to function - this vector is used as swap space
 for the std functions like set_intersection, set_union, includes.
 it usually holds a copy of what is in final_edge or clique
 aggregates -
 type: vector<vector<int>>
 Description: local to function - this 2d vector is a copy of the
 maximal cliques that are given to this function - this is used to
 add in the aggregate edges when they have been found - we do this
 as to not change the original set of cliques so they can be used in
 the next iteration of the loop
 membership_array_copy -
 type: vector<vector<bool>>
 Description: local to function - this 2d vector is a copy of the
 membership_array that is given to this function. Everytime we add an
 edge to the original membership_array we also add it to the copy. At
 the end of the loop we reset the membership_array to what it was
 before we added the edge. However, one condition for a valid edge
 is that the inverse is not a valid edge so we cross-refernce this
 copy to make sure the condition is met. For example, if we add the
 point (1,2) to the memebership_array we then add it to the copy. At
 The end of the loop (1,2) is removed from the original membership
 array but the copy still contains the edge. When we get to edge
 (2,1) we check the copy and find the the inverse, (1,2), has already
 been tested and we determine that it is an invalid edge.
 
 *********************************************************************/

//function that will find all immediate aggregates of a given c-structure
void ac_procedure(vector< vector <int> > maximal_cliques, vector<vector<bool> > membership_array){
    //holds the number of points we have added so far
    int iterations = 0;
    //single pair for holding the point we are working with
    pair<int,int>new_edge;
    //holds the vector that is the local clique we are working with
    vector<int>clique;
    //holds the vector that is the local edge we are working with
    vector<int>edge;
    //holds the final edge that we are working on
    vector<int>final_edge;
    //holds any misc data - copies and swaps
    vector<int>buffer;
    //hold aggregate data of new cliques
    vector< vector <int> > aggregates;
    //create a copy of the membership array
    vector<vector<bool> > membership_array_copy;
    //resize it and set it all to false
    membership_array_copy.resize(membership_array.size(), vector<bool>(membership_array.size(), false));
    const char *path="output/ac.txt";
    ofstream ac_output(path);
    
    ac_output<<"Starting AC-procedure:";
    //loop for each possible entry in the array
    for (int i=1; i<membership_array.size(); i++) {
        for (int j=1; j<membership_array.size(); j++) {
            //clear the edge for the new loop
            edge.clear();
            //reset the aggregate vector to the original set of maximal cliques
            aggregates.clear();
            aggregates = maximal_cliques;
            //find the first spot to add an edge
            //conditions - the spot is not a valid edge
            //the spot is not on the main diagonal
            //the inverse of the spot has not already been tested; ex -  (1,2) or (2,1)
            if (membership_array[i][j] == false && i!=j && membership_array_copy[j][i]!=true) {
                //keep track of how many edges we have added
                iterations++;
                //what spot are we going to add
                ac_output<<endl<<"Adding edge #"<<iterations<<" which is - '("<<i<<","<<j<<")'";
                //keep the (x,y) coordinate that is being added so it can be taken out
                new_edge = make_pair(i, j);
                //push the two numbers onto the edge vector
                edge.push_back(i);
                edge.push_back(j);
                //sort it for use with the std functions like intersection/union/include
                sort(edge.begin(), edge.end());
                //set the new spot to true
                membership_array[new_edge.first][new_edge.second] = true;
                //add the edge into the copy of the array so we can see if it has been tested yet
                membership_array_copy[new_edge.first][new_edge.second] = true;
                //print out the modified array - with the new edge
                print_matrix(membership_array, ac_output);
                
                //for each clique
                for (int k=0; k<maximal_cliques.size(); k++) {
                    //clear the buffer that is holding results from the intersection
                    buffer.clear();
                    //set it to the local edge so we can see it
                    clique = maximal_cliques.at(k);
                    //sort the clique for use with the std functions like intersection/union/include
                    sort(clique.begin(), clique.end());
                    //search for an intersection between our new edge and our clique
                    set_intersection(edge.begin(), edge.end(), clique.begin(), clique.end(), back_inserter(buffer));
                    //if the intersection is not empty that means that the edge interacts with the clique in someway
                    if (!(buffer.empty())) {
                        /*comment the cout comments
                         //print the elments in the intersection
                         cout<<"There is something in the intersection - '";
                         print_clique(buffer);
                         cout<<"'"<<endl;
                         //print the clique that is involved in this intersection
                         cout<<"The solution will involve clique:'";
                         print_clique(clique);
                         cout<<"'"<<endl;
                         */
                        //we need to begin computing the final edge -
                        //if there is nothing in the final edge yet we can just set the final
                        //edge to the first clique that is involved in the aggregate
                        //this is because we can do the intersection of the null set
                        if (final_edge.empty()) {
                            final_edge = clique;
                            /*comment the cout comments
                             cout<<"Final edge is empty - lets put '";
                             print_clique(final_edge);
                             cout<<"' in there!"<<endl;
                             */
                        }
                        //if there is something is the edge we need to take the intersection
                        //of what is already there with whatever we have as a clique
                        else if (!(final_edge.empty())){
                            //find some space to keep what we have as our work-in-progress final edge
                            buffer.clear();
                            buffer = final_edge;
                            //clear what was in final edge so we can fill it with new stuff
                            final_edge.clear();
                            //sort what we have so it can be used in the intersection function
                            sort(buffer.begin(), buffer.end());
                            //find the intersection between the clique we are working with and the
                            //current data in the buffer that is our working final edge
                            set_intersection(clique.begin(), clique.end(), buffer.begin(), buffer.end(), back_inserter(final_edge));
                            /*comment the cout comments
                             //print what we have as an intersection
                             cout<<"So we took the intersection and now we are left with - '";
                             print_clique(final_edge);
                             cout<<"' as our intersection"<<endl;
                             */
                        }
                    }
                }
                
                //clear the buffer so we can do the same swap we did above
                buffer.clear();
                clique.clear();
                //store our working final edge in a buffer
                buffer = final_edge;
                //clear what was in the final edge so we can fill it with the real thing
                final_edge.clear();
                //sort what we have so we can use the std intersection/union/include functions
                sort(buffer.begin(), buffer.end());
                //find the union of our edge and the intersection of each edge involved - this gives the new edge
                set_union(edge.begin(), edge.end(), buffer.begin(), buffer.end(), back_inserter(final_edge));
                /*comment the cout comments
                 //print out what the new edge is
                 cout<<"We took the union, what is the new edge we have? - '";
                 print_clique(final_edge);
                 cout<<"'"<<endl;
                 */
                //sort the final edge for use with includes
                sort(final_edge.begin(), final_edge.end());
                //counter for how many elements we have popped off
                int z=0;
                //check to see if any of the old edges are a subset of the new edge
                for (int k=0; k<maximal_cliques.size(); k++) {
                    //local loop clique
                    clique = maximal_cliques.at(k);
                    //if there is an edge that is the subset of our new edge - exclude it
                    if (includes(final_edge.begin(), final_edge.end(), clique.begin(), clique.end())) {
                        //increment i each time we are going to erase something
                        //when we erase an element it needs to be accounted for
                        /*comment the cout comments
                         cout<<"The edge '";
                         print_clique(clique);
                         cout<<"' is a subset of '";
                         print_clique(final_edge);
                         cout<<"' so, exclude it"<<endl;
                         */
                        //aggregates is a copy of the original maximal cliques that we started with
                        //erase the corresponding element in aggregates from the original maximal
                        //cliques - this is just so that we can display the new aggregates and not
                        //disturb the flow for the next cycle of the loop
                        //subtract i from our index - this accounts for erasing elements
                        aggregates.erase(aggregates.begin()+(k-z));
                        z++;
                    }
                }
                //push the new edge onto the set of maximal cliques
                aggregates.push_back(final_edge);
                //print out the aggregates for adding in the edge
                ac_output<<endl<<"The new aggregate set of maximal cliques when adding in the edge {";
                ac_output<<new_edge.first<<","<<new_edge.second<<"} results in - \n";
                print_maximal_cliques(aggregates, ac_output);
        
                //remove the edge that we added so we can start fresh on the next loop
                membership_array[new_edge.first][new_edge.second] = false;
            }
        }
    }
}

#endif
