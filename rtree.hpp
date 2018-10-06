#ifndef RTREE_HPP
#define RTREE_HPP
#include <iostream>
#include <vector>
#include "spatialdata.hpp"
using namespace std;
using namespace SpatialData;
namespace SpatialIndex{
/*
 * Forma geométrica no disco
 * Não faço ideia de como implementar essa classe
 */

enum{
    ORDEM = 4,
    MINNODES = ORDEM/2,
    MAXNODES = ORDEM,
};

struct Node;

struct Chave{
    Retangulo MBR;
    union{
        Node* ChildPointer{};
        const streampos Dado; // Guarda o indice da forma em disco
    };
    Chave(Retangulo&, streampos&);
    Chave(Retangulo&, Node*);
};

struct Node{
    int m_Nivel;
    int m_Count;
    vector<Chave> mchaves;
    Node(int nivel=0, int count=0);
};

class RTree{
private:
    Node* raiz;
    size_t count;

public:
   RTree();
   void Push(Retangulo&, const streampos&);
   bool isEmpty();
};

}
#endif // RTREE_HPP
