#include <iostream>
#include <string>
#include <vector>
#include <algorithm>  // Для lower_bound и upper_bound
using namespace std;

struct Node {
    char key;
    Node* left;
    Node* right;
    Node* parent;
    int height;
    Node(char k) : key(k), left(nullptr), right(nullptr), parent(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;

    int height(Node* n) {
        return n ? n->height : 0;
    }
    int balanceFactor(Node* n) {
        return height(n->right) - height(n->left);
    }
    void updateHeight(Node* n) {
        n->height = max(height(n->left), height(n->right)) + 1;
    }
    Node* rotateLeft(Node* n) {
        Node* r = n->right;
        n->right = r->left;
        if (r->left) r->left->parent = n;
        r->left = n;
        r->parent = n->parent;
        n->parent = r;
        updateHeight(n);
        updateHeight(r);
        return r;
    }
    Node* rotateRight(Node* n) {
        Node* l = n->left;
        n->left = l->right;
        if (l->right) l->right->parent = n;
        l->right = n;
        l->parent = n->parent;
        n->parent = l;
        updateHeight(n);
        updateHeight(l);
        return l;
    }
    Node* balance(Node* n) {
        updateHeight(n);
        if (balanceFactor(n) == 2) {
            if (balanceFactor(n->right) < 0)
                n->right = rotateRight(n->right);
            return rotateLeft(n);
        }
        if (balanceFactor(n) == -2) {
            if (balanceFactor(n->left) > 0)
                n->left = rotateLeft(n->left);
            return rotateRight(n);
        }
        return n;
    }
    Node* insert(Node* n, char key) {
        if (!n) return new Node(key);
        if (key < n->key) {
            n->left = insert(n->left, key);
            n->left->parent = n;
        }
        else if (key > n->key) {
            n->right = insert(n->right, key);
            n->right->parent = n;
        }
        return balance(n);
    }

    void inOrderTraversal(Node* n, vector<char>& sortedKeys) {
        if (!n) return;
        inOrderTraversal(n->left, sortedKeys);
        sortedKeys.push_back(n->key);
        inOrderTraversal(n->right, sortedKeys);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(char key) {
        root = insert(root, key);
        root->parent = nullptr;
    }

    Node* getRoot() { return root; }

    // Поиск диапазона с бинарным поиском
    vector<char> findRange(char low, char high) {
        vector<char> sortedKeys;
        inOrderTraversal(root, sortedKeys);  // Преобразуем дерево в отсортированный массив

        // Используем бинарный поиск для нахождения границ диапазона
        auto startIt = lower_bound(sortedKeys.begin(), sortedKeys.end(), low);
        auto endIt = upper_bound(sortedKeys.begin(), sortedKeys.end(), high);

        // Возвращаем элементы в диапазоне [low, high]
        return vector<char>(startIt, endIt);
    }

    void print(Node* n, int depth = 0) {
        if (n) {
            print(n->right, depth + 1);
            for (int i = 0; i < depth; i++) cout << "   ";
            cout << n->key << "\n";
            print(n->left, depth + 1);
        }
    }
};

int main8() {
    setlocale(LC_ALL, "Russian");
    AVLTree tree;
    int n;

    // Ввод количества элементов и значений
    cout << "Введите количество элементов, которые вы хотите вставить в дерево: ";
    cin >> n;
    cout << "Введите элементы (символы A-Z): ";
    for (int i = 0; i < n; i++) {
        char key;
        cin >> key;
        tree.insert(key);
    }

    cout << "Дерево после вставки:\n";
    tree.print(tree.getRoot());

    // Ввод диапазона для поиска
    char low, high;
    cout << "\nВведите нижнюю границу диапазона (символ): ";
    cin >> low;
    cout << "Введите верхнюю границу диапазона (символ): ";
    cin >> high;

    // Поиск диапазона
    cout << "\nЭлементы в диапазоне от " << low << " до " << high << ":\n";
    vector<char> rangeResult = tree.findRange(low, high);
    for (char key : rangeResult) {
        cout << key << " ";
    }
    cout << "\n";

    return 0;
}
