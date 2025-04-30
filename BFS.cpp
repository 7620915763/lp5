#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

class node {
public:
    int data;
    node* left;
    node* right;
};

class Breadthfs {
public:
    node* insert(node* root, int data);
    void bfs(node* root);
};

// Regular BFS node insertion (same as before)
node* Breadthfs::insert(node* root, int data) {
    if (!root) {
        root = new node{ data, nullptr, nullptr };
        return root;
    }

    vector<node*> q;
    q.push_back(root);

    while (!q.empty()) {
        node* curr = q.front();
        q.erase(q.begin());

        if (!curr->left) {
            curr->left = new node{ data, nullptr, nullptr };
            return root;
        }
        else q.push_back(curr->left);

        if (!curr->right) {
            curr->right = new node{ data, nullptr, nullptr };
            return root;
        }
        else q.push_back(curr->right);
    }

    return root;
}

// Parallel BFS using OpenMP
void Breadthfs::bfs(node* root) {
    if (!root) return;

    vector<node*> currentLevel;
    currentLevel.push_back(root);

    while (!currentLevel.empty()) {
        vector<node*> nextLevel;

        // Parallel region over current level
        #pragma omp parallel for
        for (int i = 0; i < currentLevel.size(); ++i) {
            node* curr = currentLevel[i];

            // Print in critical section to avoid interleaved output
            #pragma omp critical
            {
                cout << "\t" << curr->data;
            }

            // Collect children in a thread-safe way
            #pragma omp critical
            {
                if (curr->left) nextLevel.push_back(curr->left);
                if (curr->right) nextLevel.push_back(curr->right);
            }
        }

        currentLevel = nextLevel;
    }
}

int main() {
    node* root = nullptr;
    int data;
    char ans;
    Breadthfs tree;

    do {
        cout << "\nEnter data => ";
        cin >> data;

        root = tree.insert(root, data);

        cout << "Do you want to insert one more node? (y/n): ";
        cin >> ans;

    } while (ans == 'y' || ans == 'Y');

    cout << "\nBreadth First Traversal:\n";
    tree.bfs(root);

    return 0;
}

