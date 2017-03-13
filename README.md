# thesis
This project is the basis for my Masters Thesis at SUNY Polytechnic
So far the work surrounds Maximal Clique Enumeration (MCE) in  undirected graphs:
  - An implementation of the Bron-Kerbosh (BK) Algorithm for finding the set of all maximal cliques in a given unperturbed graph
  - An implementation of the AC and RC Algorithms from Cavallo and Klir 
  
Further work includes 
  - An implementation of adding an edge from Hendrix 
  - An implementation of removing an edge from Hendrix 
  
My goals for this project are to display and understand the efficiency and complexity of each of these algorithms

As far as building the project:
  - From the root of the project directory running the included make file is sufficent to build the project 

Upon successful completion of the build:
  - Output from the program is piped to the output directory and split up into according flies (AC, RC, MCE)
    - MCE holds the output of reading the file in and create an adjacency matrix from the data as well as results of the Bron-Kerbosh     Algorithm
    - AC holds the results of the AC algorithm (Adding all possible edges)
    - RC holds the results of the RC algorithm (Removing all possible edges)
    
    
    
Refreneces:
    
Cavallo and Klir - https://www.researchgate.net/publication/233331580_Reconstructability_analysis_of_multi-dimensional_relations_A_theoretical_basis_for_computer-aided_determination_of_acceptable_systems_models
Hendrix et. al -http://www.sciencedirect.com/science/article/pii/S0304397510001386
