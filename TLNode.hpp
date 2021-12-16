template <typename T>
class TLNode {
  public:
      T data;
      TLNode<T>*up, * left,* right;

      TLNode(T data, TLNode<T>* up, TLNode<T>* left = nullptr, TLNode<T>* right = nullptr) {
        this->data = data;
        this->left = up;
        this->left = left;
        this->right = right;
      }
};
