#include "CAD/geometry/tree.hpp"

using namespace CAD;
using namespace geometry;

Tree::Tree():op{OPERATION::EMPTY},volume{nullptr}{

};
Tree::Tree(std::unique_ptr<Volume> p):op{OPERATION::NONE},volume{std::move(p)}{
    
}
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