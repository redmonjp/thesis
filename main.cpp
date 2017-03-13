#include "headers/mce_setup.hpp"
#include "headers/ac.hpp"
#include "headers/rc.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>

using namespace std;

int main(int argc, const char * argv[]) {
	vector< vector <vector <int> > > refinements;
	//vector that will hold the edges in the hypergraph
	vector<vector <int> >edge_vector;
	//vector to hold the pairs of the primal graph
	vector<pair<int, int> >primal_edge_vector;
	//the 2d vector that will be used to hold the membership array of the graph
	vector<vector<bool> > membership_array;
	//vector to hold vertices
	vector<int> vertices;
	vector < int > R;
	vector < int > X;
	vector< vector <int> > maximal_cliques;
	const char *path="output/mce.txt";
	ofstream mce_output(path); //open in constructor
	
	//call read_file - read in the description file and update membership array
	read_file(membership_array, edge_vector, primal_edge_vector, vertices, argc, argv);
	
	//call print_matrix - print the membership array in matrix form
	mce_output<<"Adjacency matrix for the graph input: ";
	print_matrix(membership_array, mce_output);
	//call print_primal_edge_vector - print the vector containing the edges in the primal graph
	mce_output<<endl<<"\nSet of primal edges for the graph data input: \n";
	print_primal_edge_vector(primal_edge_vector, mce_output);
	//call print_edge_vector - print the vector containing each hyperedge line by line
	mce_output<<endl<<"Set of cliques as they appear in the input file: \n";
	print_edge_vector(edge_vector, mce_output);
	mce_output<<endl<<"Clique comparison (bk-algorithm): \n";
	//get all the maximal cliques for the given graph
	GetAllMaxCliques(primal_edge_vector, maximal_cliques, R,vertices, X );
	//print maximal cliques
	print_maximal_cliques(maximal_cliques, mce_output);
	//check to see if they are the same
	bool same_cstructure = CheckCStructure(edge_vector, maximal_cliques);
	if (same_cstructure) {
		mce_output<<"\nThe bk_algorithm has returned that the set of maximal cliques provided ";
		mce_output<<"is correct for this graph."<<endl;
	}
	else if (!same_cstructure){
		mce_output<<"\nThe bk_algorithm has returned that the set of maximal cliques provided ";
		mce_output<<"is NOT correct for this graph."<<endl;
		edge_vector = maximal_cliques;
	}
	
	//ac-procedure
	ac_procedure(edge_vector, membership_array);
	//rc-procedure
	GetRefinements(membership_array, edge_vector, refinements);
}
