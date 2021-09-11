#include "trie.hpp"

#include <algorithm>
#include <cctype>
#include <stack>
#include <utility>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>


using namespace std;

Trie::Trie():end_of_word(false){
    for(int i = 0; i < ALPHABET_SIZE; i++){
        roots[i] = NULL;
    }
}

Trie::Trie(Trie const& other):end_of_word(other.end_of_word){
    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(other.roots[i] != NULL){
            this->roots[i] = new Trie(*(other.roots[i]));
        }else{
            this->roots[i] = NULL;
        }
    }
}

Trie::~Trie(){
    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(roots[i] != NULL){
            delete roots[i];
        }
    }
}

Trie& Trie::operator=(Trie const& other){
    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(roots[i] != NULL){
            delete roots[i];
        }
    }

    if(this == &other){
        return *this;
    }


    for(int i = 0; i < ALPHABET_SIZE; i++){
        if(other.roots[i] != NULL){
            Trie* trie = new Trie(*(other.roots[i]));
            this->roots[i] = trie;
        }else{
            this->roots[i] = NULL;
        }
    }

    return *this;
}


void Trie::insert(char const* const str){
    if(str != NULL){
        if(strlen(str) != 0){
            if(isalpha(str[0])){
                if(this->roots[toupper(str[0]) - 65] == NULL){
                    Trie* trie = new Trie();
                    (*this).roots[toupper(str[0]) - 65] = trie;
                    this->roots[toupper(str[0]) - 65]->insert(str + 1);
                }else{
                    this->roots[toupper(str[0]) - 65]->insert(str + 1);
                }
            }else{
                this->insert(str + 1);
            }
        }else{
            this->end_of_word = true;
            return;
        }
    }

}

bool Trie::check(char const* const str) const{
    if(str != NULL){
        if(strlen(str) != 0){
            if(isalpha(str[0])){
                if(this->roots[toupper(str[0]) - 65] != NULL){
                    return this->roots[toupper(str[0]) - 65]->check(str +1);
                }else{
                    return false;
                }
            }else{
                return this->check(str + 1);
            }
        }else{
            return this->end_of_word;
        }
    }
    return false;
}

char* Trie::firstWithPrefix(char const* const str,unsigned depth) const{
    if(str != NULL){
        if(strlen(str) != 0){
         //   cout << str << "aaa" << endl << endl;
          // cout << str << endl;
            if(isalpha(str[0])){
               // cout << "Case A " << str << " " << depth << endl;
                if(this->roots[toupper(str[0]) - 65] != NULL){
                   // cout << "Case A " << str << " " << depth << endl;
                    Trie* t = this->roots[toupper(str[0]) - 65]; 
                    char* a = t->firstWithPrefix(str + 1, depth + 1);
                    if(a == NULL){
                        return NULL;
                    }
                    //cout << a << endl;
                   // cout << (a == NULL) << endl;
                    a[depth] = str[0];
                    return a;
                }else{
                   // cout << "here" << endl;
                    return NULL;
                }
            }else{
                
                return NULL;
            }
        }else{
            if(this->end_of_word == true){ 
                char* a = new char[depth+1];
                a[depth] = '\0';
                return a;
            }

            for(int i = 0; i < ALPHABET_SIZE; i++){
                if(this->roots[i] != NULL){
                    Trie* t = this->roots[i]; 
                    char* a = t->firstWithPrefix(str, depth + 1);
                    if(a == NULL){
                        return NULL;
                    }
                    char x = i + 97;
                    a[depth] = x;
                    return a;
                }
            }
            return NULL;
        }
    }
    return NULL;
}


Trie load_trie(std::istream& is){
    Trie trie;
    string word = "";
    while(getline(is,word)){
        if(word.length() == 0){
            return trie;
        }
        trie.insert(word.c_str());
    }
    return trie;
}

Trie load_trie(std::string filename){
    ifstream ifs;
    ifs.open(filename);
    return load_trie(ifs);
}

int main(){
  //  cout << "default" << endl;
    Trie trie;
    // trie.insert("car");
    // //trie.insert("ball");
    // //trie.insert("carrom");
    // Trie b = Trie(trie);
    // Trie c;
    // c = b;
    // cout << c.check("ball") << endl;
    // cout << NULL << endl;
    // c.firstWithPrefix("carr",0);
    trie = load_trie("given_dict.txt");
    ifstream is;
    is.open("given_dict.txt");
    string word = "";
    int x = 0;
    while(getline(is,word)){
        cout << trie.check(word.c_str());
        x++;
        if (!(trie.check(word.c_str()))){
            //cout << word << endl;
        }
    }
    cout << x << endl;
    return 0;
}
