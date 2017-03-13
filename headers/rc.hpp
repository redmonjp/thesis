/*********************************************
 rc.hpp
 version 1.0
 Language: C++
 Written by Paul Ziarko
 Teammate: John Redmond
 Capstone 2016
 This header file contains an implementation of
 the rc-procedure as introduced in Cavallo and Klir's
 "Reconstructability Analysis" paper.
 
 Description of Functions:
 CheckNewClique:
	Input:
 clique_to_check-
 type: vector <int>
 descripton: set of possible cliques
 for c-structure of refinement.
 cstructure-
 type: vector<vector <int> >
 description: Maximal cliques already in
 new graph.
	Output:
 is_subset-
 type: bool
 description: describes whether the clique
 being checked is in the c-structure of
 the refinement.
 
 GetRefinements:
	Input:
 primal_graph-
 type: vector <vector <bool> >
 descripton: adjacency matrix for primal graph
 corresponding to hypergraph input by user.
 maximal_clique-
 type: vector<vector <int> >
 description: Set of maximal cliques of
 primal_graph.
	Output:
 refinements-
 type: vector<vector <int> >
 description: refinements of primal_graph
 that are c-structures.
 *********************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include "mce_setup.hpp"
using namespace std;

/************************************************************
 Function: CheckNewClique
 Description: This function takes as input a possible maximal
 clique in a graph that is a refinement and the maximal
 cliques already in the c-structure refinement. Then it
 checks the possible maximal clique against every clique
 in the c-structure. If the possible maximal clique
 is a subset of any clique in the c-structure then a value
 of false is returned and the possible maximal clique is not
 added to the c-structure. If the possible maximal clique is
 not a subset of any maximal clique in the c-structure, then
 the maximal clique is added to the c-structure.
 
 Dictionary of Variables:
 clique_to_check-
 type: vector <int>
 description: input to function, possible maximal clique in refinement
 of graph.
 cstructure-
 type: vector <vector<int> >
 description: input to function, contains maximal cliques already in
 refinement.
 is_subset-
 type: bool
 description: The value of this variable is false while clique_to_check
 is not a subset of any member of cstructure. If clique_to_check turns
 out to be a subset of some member of cstructure, then the value of
 is_subset is changed to true. This function returns the opposite
 value of is_subset so that false is returned if clique_to_check is a
 subset and true if it is not a subset.
 num_in_set-
	type: int
	description: Keeps track of how many members of the possible
	maximal clique are also in a clique already in the new c-structure.
 *************************************************************/


bool CheckNewClique(vector <int> clique_to_check, vector <vector <int> > cstructure){
    bool is_subset = false;
    int i=0;
    int num_in_set;
    while(!is_subset && i<cstructure.size()){//begin while
        num_in_set = 0;
        for(int j=0;j<cstructure[i].size();j++){//begin for
            for(int k=0;k<clique_to_check.size();k++){//begin for
                if(cstructure[i][j] == clique_to_check[k] && num_in_set!=clique_to_check.size()){//begin if
                    num_in_set++;
                }//end if
                if(num_in_set == clique_to_check.size()){//begin else if
                    is_subset=true;//clique_to_check is a subset
                }//end else if
            }//end for
        }//end for
        i++;
    }//end while
    return (!is_subset);//return whether or not clique_to_check is a maximal clique
    
}


/************************************************************
 Function: CreateSetofVertices
 Description: This function takes as input a cstructure and a corresponding
 adjacency matrix representing a primal graph. It then takes
 an edge in the adjacency matrix and removes it. Then, using
 the removed edge, each maximal clique in the set of maximal
 cliques that the removed edge is a subset of is broken up
 using the set difference function. The clique that was broken
 up is removed from the set of maximal cliques. Then the new
 cliques are sent to the CheckNewClique function to see if they
 will be members of the new c-structure that is a refinement
 of the original. After it does this with all new cliques, the
 refinement will be output to the screen.
 Dictionary of Variables:
 primal_graph-
 type: vector <vector <bool> >
 description: input to function, adjacency matrix for primal graph
 vertices-
 type: vector< vector <int> >
 description: input to function, C-structure to find refinements of
 removed_edges-
 type: vector < vector <int> >
 description: used to keep track of edges that have been removed
 removed_edge-
 type: vector <int>
 description: edge that is currently removed from primal_graph
 new_maximal_cliques-
 type: vector <vector <int> >
 description: copy of maximal_cliques
 new_cstructure-
 type: vector <vector <int> >
 description: c-structure that is refinement of original c-structure
 one_in_clique-
 type: bool
 description: true if one vertex in the removed edge is in some clique
 in new_maximal_cliques, false otherwise
 is_max_clique-
 type:bool
 description: true if a new clique is in the new c-structure, otherwise
 false
 two_in_clique-
 type:bool
 description: true if removed vertex is a subset of some member of
 c-structure, false otherwise
 primal_graph_copy-
 type: vector <vector <bool> >
 description: copy of primal_graph, used to keep track of which edges
 have been removed
 iterate-
 type: int
 description: keeps track of number of edges removed
 new_clique_one-
	type: vector <int>
	description: first new clique created when breaking up an existing
	clique
 new_clique_two-
 type: vector <int>
 description: second new clique created when breaking up an existing
 clique
 *************************************************************/

void GetRefinements(vector <vector <bool> > primal_graph, vector <vector <int> > maximal_cliques,vector< vector <vector <int> > > &refinements)
{
    int iterate = 0;
    vector < vector<int> > removed_edges;
    vector <int>  removed_edge;
    vector <vector <int> > new_maximal_cliques;
    vector <vector <int>  > new_cstructure;
    bool one_in_clique;
    bool is_max_clique;
    bool two_in_clique;
    vector<vector <bool> > primal_graph_copy;
    primal_graph_copy.resize(primal_graph.size(), vector<bool>(primal_graph.size(), true));//initialize primal_graph_copy
    vector<int>::iterator it;
    const char *path="output/rc.txt";
    ofstream rc_output(path);
    
    rc_output<<"Starting RC-procedure:";
    for(int x=0; x<primal_graph.size();x++){//begin for
        for(int y=0; y<primal_graph[x].size();y++){//begin for
            if(primal_graph[x][y] == true && x!=y && primal_graph_copy[y][x]!=false){//begin if
                removed_edge.clear();//reset removed edge
                //find edge to remove
                removed_edge.push_back(x);
                removed_edge.push_back(y);
                iterate++;
                rc_output<<endl<<"Removing edge #"<<iterate<<" which is - '("<<x<<","<<y<<")'";
                //mark edges removed in primal_graph_copy
                primal_graph_copy[x][y]  = false;
                primal_graph_copy[y][x] = false;
                //remove edge
                primal_graph[x][y] = false;
                primal_graph[y][x] = false;
                removed_edges.push_back(removed_edge);
                sort(removed_edge.begin(), removed_edge.end());
                new_maximal_cliques.clear();
                new_cstructure.clear();
                print_matrix(primal_graph, rc_output);
                for(int f=0;f<maximal_cliques.size();f++)
                {//begin for
                    //copy maximal_cliques to new_maximal_cliques and new_cstructure
                    new_maximal_cliques.push_back(maximal_cliques[f]);
                    new_cstructure.push_back(maximal_cliques[f]);
                }//end for
                for(int i=0;i<new_maximal_cliques.size();i++){//begin for
                    two_in_clique = false;
                    one_in_clique = false;
                    for(int j=0;j<new_maximal_cliques[i].size();j++)
                    {//begin for
                        if ((removed_edge[0] == new_maximal_cliques[i][j]||removed_edge[1] == new_maximal_cliques[i][j])&& one_in_clique ==false && new_maximal_cliques[i].size()>1){//begin if
                            one_in_clique = true;
                        }//end if
                        else if (((removed_edge[0] == new_maximal_cliques[i][j]||removed_edge[1] == new_maximal_cliques[i][j]) && new_maximal_cliques[i].size()==1)||((removed_edge[0] == new_maximal_cliques[i][j]||removed_edge[1] == new_maximal_cliques[i][j])&& one_in_clique ==true)){//begin else if
                            vector <int> new_clique_one(100);
                            vector <int> new_clique_two(100);
                            sort(new_maximal_cliques[i].begin(), new_maximal_cliques[i].end());
                            two_in_clique = true;
                            sort(new_cstructure[i].begin(), new_cstructure[i].end());
                            //find first new clique by breaking apart existing clique
                            it=set_difference (new_maximal_cliques[i].begin(), new_maximal_cliques[i].end(), removed_edge.begin(), removed_edge.begin()+1, new_clique_one.begin());
                            new_clique_one.resize(it-new_clique_one.begin());
                            //find second new clique by breaking apart existing clique
                            it=set_difference (new_maximal_cliques[i].begin(), new_maximal_cliques[i].end(), removed_edge.begin()+1, removed_edge.end(), new_clique_two.begin());
                            new_clique_two.resize(it-new_clique_two.begin());
                            new_cstructure.erase(new_cstructure.begin()+i);
                            sort(new_clique_one.begin(),new_clique_one.end());
                            sort(new_clique_two.begin(),new_clique_two.end());
                            is_max_clique = CheckNewClique(new_clique_one,new_cstructure);
                            if(is_max_clique){//begin if
                                new_cstructure.push_back(new_clique_one);//add to new_cstructure
                            }//end if
                            is_max_clique = CheckNewClique(new_clique_two,new_cstructure);
                            if(is_max_clique){//begin if
                                new_cstructure.push_back(new_clique_two);//add to new_cstructure
                            }//end if
                        }//end else if
                        if(two_in_clique == true)
                            break;
                    }//end for
                    
                }//end for
                rc_output<<endl<<"The new refined set of maximal cliques when adding in the edge {";
                rc_output<<x<<","<<y<<"} results in - \n";
                print_maximal_cliques(new_cstructure, rc_output);
                //reset graph for next iteration
                primal_graph[x][y] = true;
                primal_graph[y][x] = true;
            }//end for
            
        }//end for
    }//end for
    
    
    
    
    //FindImmediateRefinements(primal_graph, removed_edges, maximal_cliques, refinements);
}

