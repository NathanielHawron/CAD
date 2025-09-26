#include "CAD/geometry/tree.hpp"

using namespace CAD;
using namespace geometry;

Tree::Tree():op{OPERATION::EMPTY}{

};
Tree::Tree(Tree *t):op{t->op},left{t->left},right{t->right}{
    t->op = OPERATION::NULL_OP;
    t->left = this;
    t->right = nullptr;
};
Tree::~Tree(){
    if(this->left != nullptr){
        delete this->left;
    }
    if(this->right != nullptr){
        delete this->right;
    }
}
void Tree::reset(){
    if(this->left != nullptr){
        delete this->left;
        this->left = nullptr;
    }
    if(this->right != nullptr){
        delete this->right;
        this->right = nullptr;
    }
    this->op = OPERATION::EMPTY;
}

void Tree::operator+=(Volume *t){
    if(this->left == nullptr){
        this->left = t;
    }else if(this->right == nullptr){
        this->right = t;
    }else{
        new Tree(this);
        this->right = t;
        this->op = OPERATION::UNION;
    }
}
void Tree::operator-=(Volume *t){
    if(this->left == nullptr){
        return;
    }else if(this->right != nullptr){
        new Tree(this);
    }
    this->right = t;
    this->op = OPERATION::DIFFERENCE;
}
void Tree::operator*=(Volume *t){
    if(this->left == nullptr){
        return;
    }else if(this->right != nullptr){
        new Tree(this);
    }
    this->right = t;
    this->op = OPERATION::INTERSECTION;
}

Graph Tree::generateGraph(){
    if(this->right == nullptr){
        if(this->left == nullptr){
            return Graph{};
        }else{
            return this->right->generateGraph();
        }
    }else{
        switch(this->op){
        case OPERATION::UNION:{
            return this->left->unionGraph(this->right);
        }break;
        case OPERATION::DIFFERENCE:{
            return this->left->differenceGraph(this->right);
        };
        case OPERATION::INTERSECTION:{
            return this->left->intersectionGraph(this->right);
        };
        }
    }
}

bool Tree::inside(glm::vec3 point){

}
glm::vec3 Tree::project(glm::vec3 origin, glm::vec3 ray){

}

Graph Tree::unionGraph(Volume *other) {
    
}
Graph Tree::differenceGraph(Volume *other) {
    
}
Graph Tree::intersectionGraph(Volume *other) {
    
}