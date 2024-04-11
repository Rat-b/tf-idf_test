#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

// ͣ�ôʣ����ôʣ��б�
vector<string> stop_words = {
        "the", "be", "to", "of", "and", "a", "in", "that", "have", "I",
        "it", "for", "not", "on", "with", "he", "as", "you", "do", "at",
        "this", "but", "his", "by", "from", "they", "we", "say", "her", "she",
        "or", "an", "will", "my", "one", "all", "would", "there", "their", "what",
        "so", "up", "out", "if", "about", "who", "get", "which", "go", "me",
        "when", "make", "can", "like", "time", "no", "just", "him", "know", "take",
        "people", "into", "year", "your", "good", "some", "could", "them", "see", "other",
        "than", "then", "now", "look", "only", "come", "its", "over", "think", "also",
        "back", "after", "use", "two", "how", "our", "work", "first", "well", "way",
        "even", "new", "want", "because", "any", "these", "give", "day", "most", "us",
        "is", "are", "was", "were", "has", "had", "do", "does", "did", "should",
        "could", "would", "shall", "will", "must", "can", "may", "might", "am", "been",
        "being","Mr","My","years"
    };
vector<string> get_document(string filename){

    vector<string> document;
    // ���ı��ļ�
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return {{}};
    }
    
    string line;
    // ���ж�ȡ�ļ�����
    while (getline(file, line)) {
        // ���ڴ洢��ǰ�еĵ��ʵ�vector
        vector<string> words;
        // ʹ��stringstream����ǰ�����ݷָ�ɵ���
        stringstream ss(line);
        string word;
        while (ss >> word) {
            // �����ʴ���words����
            document.push_back(word);
        }
    }
    
    // �ر��ļ�
    file.close();
    return document;

}

map<string, double> calculate_tf(const vector<string>& words) {
    map<string, double> tf;
    int total_words = words.size();
    
    for (const auto& word : words) {
        tf[word] += 1.0 / total_words;
    }
    
    return tf;
}

// �������ĵ�Ƶ�ʣ�IDF��
map<string, double> calculate_idf(const vector<vector<string>>& documents) {
    map<string, double> idf;
    int total_documents = documents.size();
    
    for (const auto& doc : documents) {
        vector<string> unique_words;
        for (const auto& word : doc) {
            if (find(unique_words.begin(), unique_words.end(), word) == unique_words.end()) {
                unique_words.push_back(word);
            }
        }
        for (const auto& word : unique_words) {
            idf[word] += 1.0;
        }
    }
    
    for (auto& pair : idf) {
        pair.second = log(total_documents / pair.second);
    }
    
    return idf;
}

int main() {
    // ʾ���ı�
    vector<vector<string>> documents;

    for(int i = 1;i<=3;++i){
        documents.push_back(get_document("tf-idf_example"+to_string(i)+".txt"));
    }
    
    
    // �������ĵ�Ƶ�ʣ�IDF��
    map<string, double> idf = calculate_idf(documents);
    
    // ����TF-IDF
    map<string, double> term_tfidf_scores;
    map<string, double> tf = calculate_tf(documents[2]);
    for (const auto& pair : tf) {
            // ������ʲ��ǳ��ôʣ������TF-IDF
            if (find(stop_words.begin(), stop_words.end(), pair.first) == stop_words.end()) {
                term_tfidf_scores[pair.first] = pair.second * idf[pair.first];
            }
        }

    // ��TF-IDFֵ��������
    vector<pair<string, double>> sorted_terms(term_tfidf_scores.begin(), term_tfidf_scores.end());
    sort(sorted_terms.begin(), sorted_terms.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return a.second > b.second;
    });
    
    // ����������ı�����
    cout << "Top terms after removing stop words, sorted by TF-IDF score:" << endl;
    int count = 0;
    for (const auto& term : sorted_terms) {
        cout << term.first << ": " << term.second << endl;
        count++;
        if(count==10)break;
    }
    
    return 0;
}
