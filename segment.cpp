#include <iostream>
#include <vector>
#include <fstream>

using std::vector;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

class SegmentTree {
public:
	// Constructor takes an input array and builds a segment tree for it.
    SegmentTree(const vector<int>& array);
    // Search for the sum of the values in the range [start, end] of the original array.
    int search(int start, int end);
    // Insert a value at a specific index in the original array and update the segment tree.
    void insert(int index, int value);
    // Write the structure of the segment tree to a Graphviz file.
    void printDotFile(const char* filename);

private:
	// Structure of a node in the segment tree.
    struct Node {
        int start, end, sum;
        Node* left, * right;

		// Constructor for a node with a given start, end, and sum.
        Node(int start, int end, int sum = 0) {
            this->start = start;
            this->end = end;
            this->sum = sum;
            left = nullptr;
            right = nullptr;
        }

		// Destructor for a node, deletes the left and right subtrees.
        ~Node() {
            delete left;
            delete right;
        }

		// Returns the middle index of the range [start, end].
        int mid() const {
            return (start + end) / 2;
        }
    };

    Node* root;

	// Recursive helper function for inserting a value at a specific index.
    void insert(int index, int value, Node* node);
    // Recursive helper function for searching for the sum of values in a range.
    int search(int start, int end, Node* node);
    // Recursive helper function for writing the structure of the segment tree to a Graphviz file.
    void printDotFile(Node* node, ofstream& outfile);
};

// Constructor for the segment tree.
SegmentTree::SegmentTree(const vector<int>& array) {
	// get the size of the input array
    int n = array.size();
    // create a new node as the root of the segment tree
    root = new Node(0, n - 1);
    // loop through each element in the input array
    for (int i = 0; i < n; i++) {
    	// insert the i-th element of the array into the segment tree using the insert method
        insert(i, array[i], root);
    }
}

void SegmentTree::insert(int index, int value, Node* node) {
	// Check if we've reached a leaf node
    if (node->start == node->end) {
    	// Set the sum of this leaf node to the given value
        node->sum = value;
        return;
    }
    // Calculate the midpoint of this node's range
    int mid = node->mid();
    // If the index falls within the left child's range
    if (index <= mid) {
    	// Create a new left child if it doesn't exist yet
        if (node->left == nullptr) {
            node->left = new Node(node->start, mid);
        }
        // Recursively insert the value into the left child
        insert(index, value, node->left);
    }
    // If the index falls within the right child's range
    else {
    	// Create a new right child if it doesn't exist yet
        if (node->right == nullptr) {
            node->right = new Node(mid + 1, node->end);
        }
        // Recursively insert the value into the right child
        insert(index, value, node->right);
    }
    // Update this node's sum based on its children's sums
    node->sum = (node->left != nullptr ? node->left->sum : 0)
        + (node->right != nullptr ? node->right->sum : 0);
}

void SegmentTree::insert(int index, int value) {
	// Call the insert function with index, value, and root node of the segment tree
    insert(index, value, root);
}
// This function searches the segment tree to find the sum of values in the range
// [start, end].
int SegmentTree::search(int start, int end) {
	// Call the private search function with the segment tree root node.
    return search(start, end, root);
}

int SegmentTree::search(int start, int end, Node* node) {
	// If the current node is null or the range represented by the current node does not intersect with the given range, return 0.
    if (node == nullptr || start > node->end || end < node->start) {
        return 0;
    }
    // If the range represented by the current node is completely inside the given range, return the sum stored in this node.
    if (start <= node->start && node->end <= end) {
        return node->sum;
    }
    // Otherwise, recursively search the left and right subtrees of the current node.
    return search(start, end, node->left) + search(start, end, node->right);
}

void SegmentTree::printDotFile(const char* filename) {
	// open a file for writing and create a stream
    ofstream outfile(filename);
    // write the beginning of the Graphviz file
    outfile << "digraph G {" << endl;
    // recursively print the tree in the Graphviz format
    printDotFile(root, outfile);
    // write the end of the Graphviz file
    outfile << "}" << endl;
    // close the output file
    outfile.close();
    cout << "DOT file generated for visualization" << endl;
}

void SegmentTree::printDotFile(Node* node, ofstream& outfile) {
	// Output the label for the current node with its sum.
    outfile << "n" << node << " [label=\"" << node->sum << "\"]" << endl;
    // If the left child exists, output an arrow from the current node to the left child.
    if (node->left != nullptr) {
        outfile << "n" << node << " -> n" << node->left << " [label=\"L\"]" << endl;
        // Recursively output the dot file for the left subtree.
        printDotFile(node->left, outfile);
    }
    // If the right child exists, output an arrow from the current node to the right child.
    if (node->right != nullptr) {
        outfile << "n" << node << " -> n" << node->right << " [label=\"R\"]" << endl;
        // Recursively output the dot file for the right subtree.
        printDotFile(node->right, outfile);
    }
}
int main() {
	// Open input file
    ifstream infile("input.txt");
    int n;
    // Read in size of input array
    infile >> n;
	// Read in array elements and store them in a vector
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        infile >> arr[i];
    }
	// Construct a SegmentTree object with the array
    SegmentTree tree(arr);
	// Output the SegmentTree in DOT format to a file
    tree.printDotFile("output.dot");
	// Close input file and exit
    return 0;
}


