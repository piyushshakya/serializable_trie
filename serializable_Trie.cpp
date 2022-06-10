#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class trieNode{
    public:
    char data;
    map<char, trieNode*> children; // use unordered map to optimise but not in sorted order
    
    // vector<trieNode*> children;
    bool isTerminal;
    int wordWithPrefix;


    trieNode(){
        isTerminal = false;
        wordWithPrefix = 0;
    }
    trieNode(char data){
        this->data = data;
        this->isTerminal = false;
        wordWithPrefix = 0;
        // for(int i = 0; i < 26; i++)
        //     children.push_back(NULL);
    }
};

class trie{
    trieNode *root;
    int totalWords;

    void insertHelper(trieNode *root, string word){
        if(word.size() == 0){
            root->isTerminal = true;
            root->wordWithPrefix++;
            return;
        }

        // int idx = word[0] - 'A';
        trieNode *child;
        map<char, trieNode*>& childMap = root->children;
        if(childMap.find(word[0]) != childMap.end()){ //present
            // child = root->children[word[0]];
            child = childMap[word[0]];
        }
        else{
            child = new trieNode(word[0]);
            // root->children[word[0]] = child;
            childMap[word[0]] = child;
        }
        child->wordWithPrefix++;
        insertHelper(child, word.substr(1));
    }


    void removeHelper(trieNode *root, string word){
        if(word.size() == 0){
            root->isTerminal = false;
            return;
        }

        int idx = word[0] - 'A';
        map<char, trieNode*>& childMap = root->children;
        if(childMap.find(word[0]) != childMap.end()){ //absent
            removeHelper(childMap[word[0]], word.substr(1));
        }
    }

    // onlyPrefix : only need to check for prefix not the word that has terminal
    int wordsWithThePrefix_Helper(trieNode *root, string prefix, bool onlyPrefix = true){
        if(prefix.size() == 0){
            if(onlyPrefix || root->isTerminal)
                return root->wordWithPrefix;
        }

        map<char, trieNode*> &childMap = root->children;

        if(childMap.find(prefix[0]) != childMap.end())
            return wordsWithThePrefix_Helper(childMap[prefix[0]], prefix.substr(1));
        
        else
            return 0;
    }

    void buildSerialTrie(trieNode *root, string &serialTrie){
        if(root->isTerminal){
            serialTrie.push_back(']');
        }

        map<char, trieNode*> &childMap = root->children;

        for(auto child: childMap){
            serialTrie.push_back(child.first); //inserting character of words
            buildSerialTrie(child.second, serialTrie);
        }

        serialTrie.push_back('>');
    }


    void treeDepthTraversal(trieNode* root, vector<string> &trieToStr, string prefix){
        if(root->isTerminal){
            trieToStr.push_back(prefix);
        }

        map<char, trieNode*> &childMap = root->children;

        for(auto i : childMap){
            prefix.push_back(i.first);
            treeDepthTraversal(i.second, trieToStr, prefix);
            prefix.pop_back();
        }
    }


    int stringToTrie(trieNode* node, string &serialTrie, int &read){
        int wordSeen = 0;
        if(serialTrie[read] == ']'){
            node->isTerminal = true;
            wordSeen++;
            read++;
        }
        else
            node->isTerminal = false;

        map<char, trieNode*> &childMap = node->children;
        while(serialTrie[read] != '>'){
            char ch = serialTrie[read];
            childMap[ch] = new trieNode();
            wordSeen += stringToTrie(childMap[ch], serialTrie, ++read);
        }

        read++;
        node->wordWithPrefix = wordSeen;
        return wordSeen;
    }

    public:

    trie(){
        root = new trieNode();
        totalWords = 0;
    }
    

    trie(vector<string> words){
        totalWords = 0;
        root = new trieNode();

        for(auto word : words)
            insert(word);
    }

// Deserialise
    trie(string serialTrie){
        root = new trieNode();
        int read = 0;

        totalWords = stringToTrie(root, serialTrie, read);
    }

    void insert(string word){
        insertHelper(root, word);
        totalWords++;
    }

    bool search(string word){
        if(wordsWithThePrefix_Helper(root, word, false))
            return true;
        
        return false;
    }

    void remove(string word){
        removeHelper(root, word);
        totalWords--;
    }

    int numberOfWordsInTrie(){
        return totalWords;
    }

    int wordsWithThePrefix(string prefix){
        return wordsWithThePrefix_Helper(root, prefix);
    }

//  serialise
    string trieToString(){
        string serializedTrie;
        buildSerialTrie(root, serializedTrie);

        return serializedTrie;
    }


    vector<string> trieToList(){
        vector<string> trieToStr;
        treeDepthTraversal(root, trieToStr, "");

        return trieToStr;
    }
};



int main(){


    
    vector<string> input;
    string word;

    ifstream in("op.txt");
    ofstream out("ip.txt");

    while(in.eof() == 0){
        in>>word;
        // input.push_back(word);
    }

    trie *t = new trie(input);
    string serialise = t->trieToString();
    
    trie* deseralised_String = new trie(word);
    vector<string> s = deseralised_String->trieToList();
    for(auto i : s){
        out<<i<<endl;
    }

    // trie *t = new trie();
    // t->insert("123");
    // t->insert("12987b");
    // t->insert("ABDE");
    // t->insert("CAR");
    // t->insert("CAB");
    // t->insert("CARE");
    // t->insert("DO");
    // t->insert("DOES");
    // t->insert("DON");
    // t->insert("TIME");
    // t->insert("TICK");
    // t->insert("TILL");

    // trie *tt = new trie(t->trieToString());
    // cout<<t->trieToString()<<endl<<endl;

    // vector<string> list = tt->trieToList();
    // for(auto i: list)
    //     cout<<i<<endl;

    cout<<"\nnumber of words with : "<<t->wordsWithThePrefix("lo");
    cout<<"\nwords : "<<t->numberOfWordsInTrie();
    cout<<"\nSearch lorem : "<<t->search("lorem");
    vector<string> list = t->trieToList();
    cout<<"\n\nlist: \n\n";
    for(auto i: list){
        cout<<i<<endl;
    }
    cout<<endl;
    return 0;
}