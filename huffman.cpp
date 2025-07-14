#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char ch, int freq) {
        this->ch = ch;
        this->freq = freq;
        this->left = nullptr;
        this->right = nullptr;
    }
};

class HuffmanCode {
private:
    string inputFile;
    string outputFile;
    unordered_map<char, string> code_dict;
    string encoded_text_bits;
    unordered_map<char, int> freqMap;
    priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> minHeap;

public:
    HuffmanCode(string inputPath) :
        minHeap([](Node* a, Node* b) { return a->freq > b->freq; }) {
        this->inputFile = inputPath;
        this->outputFile = inputPath.substr(0, inputPath.find_last_of(".")) + ".bin";
    }

private:
    unordered_map<char, int> calculateFrequencyOfEachChar(const string& text) {
        unordered_map<char, int> freqMap;
        for (char c : text) {
            freqMap[c]++;
        }
        return freqMap;
    }

    void Build_heap(const unordered_map<char, int>& map) {
        for (auto& pair : map) {
            Node* node = new Node(pair.first, pair.second);
            minHeap.push(node);
        }
    }

    Node* Build_Binary_tree() {
        while (minHeap.size() > 1) {
            Node* binary_tree_node_1 = minHeap.top();
            minHeap.pop();

            Node* binary_tree_node_2 = minHeap.top();
            minHeap.pop();

            int frequency = binary_tree_node_1->freq + binary_tree_node_2->freq;
            Node* node = new Node('\0', frequency);
            node->left = binary_tree_node_1;
            node->right = binary_tree_node_2;

            minHeap.push(node);
        }
        return minHeap.top();
    }

    void Build_tree_code_bits(Node* root, const string& curr_bits) {
        if (root == nullptr) return;

        if (root->left == nullptr && root->right == nullptr) {
            code_dict[root->ch] = curr_bits;
            return;
        }

        Build_tree_code_bits(root->left, curr_bits + "0");
        Build_tree_code_bits(root->right, curr_bits + "1");
    }

    void Build_Tree_code() {
        Node* root = Build_Binary_tree();
        Build_tree_code_bits(root, "");
    }

    string Encode_text(const string& text) {
        for (char c : text) {
            encoded_text_bits += code_dict[c];
        }
        return encoded_text_bits;
    }

    string Build_padded_text(const string& text) {
        string padded_text = text;
        int padding_value = 8 - text.length() % 8;

        string padding_info = bitset<8>(padding_value).to_string();
        padded_text = padding_info + padded_text;

        for (int i = 0; i < padding_value; i++) {
            padded_text += "0";
        }

        return padded_text;
    }

    vector<uint8_t> Convert_to_bytes(const string& padded_text) {
        vector<uint8_t> byte_array;

        for (size_t i = 0; i < padded_text.length(); i += 8) {
            string byte_str = padded_text.substr(i, 8);
            byte_array.push_back(static_cast<uint8_t>(bitset<8>(byte_str).to_ulong()));
        }

        return byte_array;
    }

    string readFileToText() {
        ifstream file(inputFile);
        if (!file) {
            cerr << "Error: Could not open file " << inputFile << endl;
            exit(1);
        }
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void writeCompressedDataToFile(const vector<uint8_t>& byteArray) {
        ofstream outFile(outputFile, ios::binary);
        if (!outFile) {
            cerr << "Error: Could not create output file " << outputFile << endl;
            exit(1);
        }

        outFile.write(reinterpret_cast<const char*>(byteArray.data()), byteArray.size());
        outFile.close();
        cout << "Compression successful! Compressed file saved as: " << outputFile << endl;
    }

public:
    void compressFile() {
        string text = readFileToText();  
        freqMap = calculateFrequencyOfEachChar(text);  

        Build_heap(freqMap);
        Build_Tree_code();    

        string encoded_text = Encode_text(text);  
        string padded_text = Build_padded_text(encoded_text);  
        vector<uint8_t> byte_array = Convert_to_bytes(padded_text);  

        writeCompressedDataToFile(byte_array);  

        ifstream orig(inputFile, ios::binary | ios::ate);
        ifstream comp(outputFile, ios::binary | ios::ate);

        if (orig && comp) {
            streamsize originalSize = orig.tellg();
            streamsize compressedSize = comp.tellg();

            cout << "Original: " << originalSize << " bytes" << endl;
            cout << "Compressed: " << compressedSize << " bytes" << endl;

            if (originalSize > 0) {
                double ratio = 100.0 * compressedSize / originalSize;
                cout << fixed << setprecision(2);
                cout << "Compression Ratio: " << ratio << "%" << endl;
            }

            orig.close();
            comp.close();
        } else {
            cerr << " Could not open one of the files to measure size." << endl;
        }
    }

    void decompressFile() {
        ifstream inFile(outputFile, ios::binary);
        if (!inFile) {
            cerr << "Error: Could not open compressed file " << outputFile << endl;
            exit(1);
        }

        vector<uint8_t> byteArray((istreambuf_iterator<char>(inFile)), {});
        inFile.close();

        string bitString;
        for (uint8_t byte : byteArray) {
            bitString += bitset<8>(byte).to_string();
        }

        int padding = bitset<8>(bitString.substr(0, 8)).to_ulong();
        bitString = bitString.substr(8);
        bitString = bitString.substr(0, bitString.size() - padding);

        Node* root = Build_Binary_tree();
        Node* current = root;
        string decodedText;

        for (char bit : bitString) {
            if (bit == '0') {
                current = current->left;
            } else {
                current = current->right;
            }

            if (!current->left && !current->right) {
                decodedText += current->ch;
                current = root;
            }
        }

        string decompressedFile = inputFile.substr(0, inputFile.find_last_of(".")) + "_decompressed.txt";
        ofstream outFile(decompressedFile);
        if (!outFile) {
            cerr << "Error: Could not create output file " << decompressedFile << endl;
            exit(1);
        }

        outFile << decodedText;
        outFile.close();

        cout << "Decompression successful! Decompressed file saved as: " << decompressedFile << endl;
    }
};

int main() {
    string inputFileName;
    cout << "Enter filename to compress: ";
    cin >> inputFileName;

    HuffmanCode huff(inputFileName);
    huff.compressFile();
    huff.decompressFile();

    return 0;
}
