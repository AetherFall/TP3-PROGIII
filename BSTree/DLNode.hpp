template <typename T>
class DLNode {
  public:
      T data;
      DLNode<T>* left;
      DLNode<T>* right;

      DLNode(T data, DLNode<T>* left = nullptr, DLNode<T>* right = nullptr) {
        this->data = data;
        this->left = left;
        this->right = right;
      }

      DLNode<T>* leftLeaf() { return left; }
      DLNode<T>* rightLeaf() { return right; }
};
