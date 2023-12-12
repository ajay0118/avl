#include <stdio.h>

#define MAX_SIZE 100

struct AVLNode {
    int key;
    int height;
    int left;
    int right;
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(struct AVLNode* tree, int index) {
    if (index == -1) {
        return 0;
    }
    return tree[index].height;
}

int getBalance(struct AVLNode* tree, int index) {
    if (index == -1) {
        return 0;
    }
    return height(tree, tree[index].left) - height(tree, tree[index].right);
}

int newNode(struct AVLNode* tree, int key) {
    int newNodeIndex = -1;
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (tree[i].key == -1) {
            tree[i].key = key;
            tree[i].height = 1;
            tree[i].left = -1;
            tree[i].right = -1;
            newNodeIndex = i;
            break;
        }
    }
    return newNodeIndex;
}

int rightRotate(struct AVLNode* tree, int y) {
    int x = tree[y].left;
    int T2 = tree[x].right;

    tree[x].right = y;
    tree[y].left = T2;

    tree[y].height = 1 + max(height(tree, tree[y].left), height(tree, tree[y].right));
    tree[x].height = 1 + max(height(tree, tree[x].left), height(tree, tree[x].right));

    return x;
}

int leftRotate(struct AVLNode* tree, int x) {
    int y = tree[x].right;
    int T2 = tree[y].left;

    tree[y].left = x;
    tree[x].right = T2;

    tree[x].height = 1 + max(height(tree, tree[x].left), height(tree, tree[x].right));
    tree[y].height = 1 + max(height(tree, tree[y].left), height(tree, tree[y].right));

    return y;
}

int minValueNode(struct AVLNode* tree, int nodeIndex) {
    while (tree[nodeIndex].left != -1) {
        nodeIndex = tree[nodeIndex].left;
    }
    return nodeIndex;
}

int deleteNode(struct AVLNode* tree, int rootIndex, int key) {
    if (rootIndex == -1) {
        return rootIndex;
    }

    if (key < tree[rootIndex].key) {
        tree[rootIndex].left = deleteNode(tree, tree[rootIndex].left, key);
    } else if (key > tree[rootIndex].key) {
        tree[rootIndex].right = deleteNode(tree, tree[rootIndex].right, key);
    } else {
        if ((tree[rootIndex].left == -1) || (tree[rootIndex].right == -1)) {
            int temp = (tree[rootIndex].left != -1) ? tree[rootIndex].left : tree[rootIndex].right;

            if (temp == -1) {
                temp = rootIndex;
                rootIndex = -1;
            } else {
                tree[rootIndex] = tree[temp];
                tree[temp].key = -1;
                tree[temp].height = 0;
            }
        } else {
            int temp = minValueNode(tree, tree[rootIndex].right);
            tree[rootIndex].key = tree[temp].key;
            tree[rootIndex].right = deleteNode(tree, tree[rootIndex].right, tree[temp].key);
        }
    }

    if (rootIndex == -1) {
        return rootIndex;
    }

    tree[rootIndex].height = 1 + max(height(tree, tree[rootIndex].left), height(tree, tree[rootIndex].right));

    int balance = getBalance(tree, rootIndex);

    // Left Left Case
    if (balance > 1 && getBalance(tree, tree[rootIndex].left) >= 0)
        return rightRotate(tree, rootIndex);

    // Left Right Case
    if (balance > 1 && getBalance(tree, tree[rootIndex].left) < 0) {
        tree[rootIndex].left = leftRotate(tree, tree[rootIndex].left);
        return rightRotate(tree, rootIndex);
    }

    // Right Right Case
    if (balance < -1 && getBalance(tree, tree[rootIndex].right) <= 0)
        return leftRotate(tree, rootIndex);

    // Right Left Case
    if (balance < -1 && getBalance(tree, tree[rootIndex].right) > 0) {
        tree[rootIndex].right = rightRotate(tree, tree[rootIndex].right);
        return leftRotate(tree, rootIndex);
    }

    return rootIndex;
}

void display(struct AVLNode* tree, int rootIndex, int level) {
    if (rootIndex != -1) {
        display(tree, tree[rootIndex].right, level + 1);
        printf("\n");
        for (int i = 0; i < level; i++)
            printf(" ");
        printf("%d", tree[rootIndex].key);
        display(tree, tree[rootIndex].left, level + 1);
    }
}

int main() {
    struct AVLNode avlTree[MAX_SIZE];
    for (int i = 0; i < MAX_SIZE; ++i) {
        avlTree[i].key = -1;
        avlTree[i].height = 0;
        avlTree[i].left = -1;
        avlTree[i].right = -1;
    }

    int rootIndex = -1;
    int choice, key;

    do {
        printf("1. Insert\n");
        printf("2. Delete\n");
        printf("3. Display\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the key to insert: ");
                scanf("%d", &key);
                rootIndex = newNode(avlTree, key);
                break;
            case 2:
                printf("Enter the key to delete: ");
                scanf("%d", &key);
                rootIndex = deleteNode(avlTree, rootIndex, key);
                break;
            case 3:
                printf("AVL Tree (in-order display):\n");
                display(avlTree, rootIndex, 0);
                printf("\n");
                break;
            case 4:
                printf("Exiting the program\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 4);

    return 0;
}

