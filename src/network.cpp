
// autotools
#include <config.h>

// c++ includes
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// local includes
#include <common.hpp>
#include <network.hpp>

// namespaces
using namespace std;

Network read_network(char *filename, char separator)
{
  ifstream inFile;
  string line, pred, prey;
  int pred_i, prey_i;
  
  Network N;
  N.name = string(filename);

  inFile.open(filename);
  while (getline(inFile, line)){
    istringstream linestream(line);

    getline(linestream, pred, separator);
    getline(linestream, prey, separator);

    if(N.node_i.count(pred) == 0){
      pred_i = N.nodes.size();
      N.node_i[pred] = pred_i;
      Node * n = new Node;
      n->name = pred;
      N.nodes.push_back(n);
    }else
      pred_i = N.node_i[pred];

    if(N.node_i.count(prey) == 0){
      prey_i = N.nodes.size();
      N.node_i[prey] = prey_i;
      Node * n = new Node;
      n->name = prey;
      N.nodes.push_back(n);
    }else
      prey_i = N.node_i[prey];

    N.nodes[pred_i]->prey.push_back(N.nodes[prey_i]);
    N.nodes[prey_i]->predators.push_back(N.nodes[pred_i]);
  }
  inFile.close();

  return N;
}

Network read_network(char separator)
{
  string line, pred, prey;
  int pred_i, prey_i;
  
  Network N;
  N.name = string("stdin");

  while (getline(cin,line)){
    istringstream linestream(line);

    getline(linestream, pred, separator);
    getline(linestream, prey, separator);

    if(N.node_i.count(pred) == 0){
      pred_i = N.nodes.size();
      N.node_i[pred] = pred_i;
      Node * n = new Node;
      n->name = pred;
      N.nodes.push_back(n);
    }else
      pred_i = N.node_i[pred];

    if(N.node_i.count(prey) == 0){
      prey_i = N.nodes.size();
      N.node_i[prey] = prey_i;
      Node * n = new Node;
      n->name = prey;
      N.nodes.push_back(n);
    }else
      prey_i = N.node_i[prey];

    N.nodes[pred_i]->prey.push_back(N.nodes[prey_i]);
    N.nodes[prey_i]->predators.push_back(N.nodes[pred_i]);
  }

  return N;
}

void print_network(Network N)
{
  for(unsigned int i=0;i<N.nodes.size();++i)
    for(unsigned int j=0;j<N.nodes[i]->prey.size();++j)
      cout << N.nodes[i]->name << " " << N.nodes[i]->prey[j]->name << endl;
}
