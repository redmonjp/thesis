#ifndef mce_setup_h
#define mce_setup_h
#define MIN_LINE_SIZE   5   //{x,y}

#include "bk.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

void read_file(vector< vector<bool> > &membership_array, vector< vector<int> > &edge_vector, vector< pair<int, int> > &primal_edge_vector, vector<int>&vertices, int argc, const char *argv[]){
    int num_of_vertices=0;
    int vertex;
    bool pair_exists = false;
    vector<int>edge;
    vector<string>lines;
    pair<int, int>primal_edge_pair;
    ifstream read_file(argv[1]);
    stringstream convert;
    istringstream ss;
    string file_str;
    string line;
    
    //check the number of arguments - if the number is not correct then quit
    if (argc != 2) {
        cout<<"Incorrect number of arguments given to program"<<endl;
        exit(1);
    }
    //read the graph description file and push each line onto a vector
    if (read_file){
        while (!read_file.eof()) {
            getline (read_file,file_str);
            lines.push_back(file_str);
        }
    }
    else{
        cout << "Couldn't open file\n";
        read_file.close();
        exit(1);
    }
    
    for (vector<string>::iterator it = lines.begin(); it !=lines.end(); ++it) {
        line = *it;
        //check for validity in the file
        if (line.empty()) {
            cout<<"Empty edge - check graph file\n";
            exit(1);
        }
        //number of vertices
        if (line.at(0) != '{') {
            convert<<line;
            if (!(convert>>num_of_vertices).fail()) {
                convert.clear();
            }
            else{
                cout<<"Unknown number of vertices: "<<num_of_vertices<<endl;
                exit(1);
            }
            
            //Call CreateSetofVertices from bk.hpp
            CreateSetofVertices(num_of_vertices, vertices);
            
            //resize the array to match the number of vertices and set it all to false
            membership_array.resize(num_of_vertices+1, vector<bool>(num_of_vertices+1, false));
        }
        
        if (line.at(0) == '{' && line.size() >= MIN_LINE_SIZE) {
            //remove first and last character - i.e. '{' && '}'
            line = line.substr(1, line.size() - 2);
            //split the string up into tokens - split on ','
            //this leaves each element in the edge
            //pack it into a vector
            istringstream ss(line);
            while (getline(ss, line, ',')){
                convert << line;
                if (!(convert>>vertex).fail()) {
                    edge.push_back(vertex);
                }
                convert.clear();
            }
            for (int x = 0; x < edge.size(); x++){
                for (int y = 0; y < edge.size(); y++){
                    if(edge.at(x) != edge.at(y)){
                        //check to make sure that it is a valid edge in the graph
                        if (edge.at(x) <= num_of_vertices && edge.at(y) <= num_of_vertices
                            && edge.at(x) != 0 && edge.at(y)!= 0) {
                            membership_array[edge.at(x)][edge.at(y)] = true;
                            //check to make sure the (x,y) and (y,x) never show up
                            if (membership_array[edge.at(y)][edge.at(x)] == true) {
                                pair_exists = true;
                            }
                            //if (y,x) is not in the membership array then put it in as an edge
                            if (!pair_exists) {
                                primal_edge_pair = make_pair(edge.at(x), edge.at(y));
                                primal_edge_vector.push_back(primal_edge_pair);
                            }
                            pair_exists = false;
                        }
                        else{
                            cout<<"Attempted to add invalid edge:"<<line<<endl;
                            exit(1);
                        }
                    }
                }
            }
            edge_vector.push_back(edge);
            edge.clear();
        }
        else if(line.at(0) == '{' && line.size() < MIN_LINE_SIZE){
            cout<<"Invalid edge: "<<line<<endl;
            exit(1);
        }
    }
}

//nested for loops to print the membership array
void print_matrix(vector< vector<bool> > &membership_array, ofstream& outfile){
    
    for (int i = 1; i < membership_array.size(); i++){
        outfile<<endl;
        for (int j = 1; j < membership_array.size(); j++){
            outfile << membership_array[i][j]<<" ";
        }
    }
}

//nested for loops to print the hyperedges
void print_edge_vector(vector< vector<int> > &edge_vector, ofstream& outfile){
    vector<int>::iterator it;
    vector<int>edge;
    
    //print the hyperedges we found!
    for (int i=0; i<edge_vector.size(); i++) {
        edge = edge_vector[i];
        outfile<<"{";
        for (it=edge.begin(); it!=edge.end(); ++it){
            if (it != edge.begin()){
                outfile << ",";
            }
            outfile<<*it;
        }
        outfile<<"}"<<endl;
    }
}

//nested for loops to print the primal edges of the membership array
void print_primal_edge_vector(vector< pair<int, int> > &primal_edge_vector, ofstream& outfile){
    const char *path="graphs/graph.dot";
    ofstream myfile(path); //open in constructor
    pair<int, int>pair;
    //write edges to the file for graphing
    myfile<<"graph { "<<"\n";
    
    //print the primal edges!
    for (int i=0; i<primal_edge_vector.size(); i++) {
        pair = primal_edge_vector.at(i);
        outfile<<"{";
        outfile<<pair.first <<","<<pair.second;
        myfile<<"\t"<<pair.first<<" -- "<<pair.second<<";\n";
        outfile<<"}"<<endl;
    }
    
    myfile<<"}";
    myfile.close();
}

//nested for loops to print the hyperedges
void print_maximal_cliques(vector< vector<int> > &maximal_cliques, ofstream& outfile){
    vector<int>::iterator it;
    vector<int>edge;
    //sort the edges!
    for(int i=0; i<maximal_cliques.size(); i++){
        sort(maximal_cliques[i].begin(), maximal_cliques[i].end());
    }
    sort(maximal_cliques.begin(), maximal_cliques.end());
    //print the hyperedges we found!
    for (int i=0; i<maximal_cliques.size(); i++) {
        edge = maximal_cliques[i];
        outfile<<"{";
        for (it=edge.begin(); it!=edge.end(); ++it){
            if (it != edge.begin()){
                outfile << ",";
            }
            outfile<<*it;
        }
        outfile<<"}"<<endl;
    }
}

//nested for loops to print the hyperedges
void print_clique(vector<int> clique){
    vector<int>::iterator it;
    
    cout<<"{";
    for (it=clique.begin(); it!=clique.end(); ++it){
        if (it != clique.begin()){
            cout << ",";
        }
        cout<<*it;
    }
    cout<<"}";
}
#endif /* mce_setup_h */
