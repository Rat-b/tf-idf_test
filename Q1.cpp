#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

// 停用词（常用词）列表
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
    // 打开文本文件
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Unable to open the file." << endl;
        return {{}};
    }
    
    string line;
    // 逐行读取文件内容
    while (getline(file, line)) {
        // 用于存储当前行的单词的vector
        vector<string> words;
        // 使用stringstream将当前行内容分割成单词
        stringstream ss(line);
        string word;
        while (ss >> word) {
            // 将单词存入words向量
            document.push_back(word);
        }
    }
    
    // 关闭文件
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

// 计算逆文档频率（IDF）
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
    // 示例文本
    vector<vector<string>> documents;

    for(int i = 1;i<=3;++i){
        documents.push_back(get_document("tf-idf_example"+to_string(i)+".txt"));
    }
    
    
    // 计算逆文档频率（IDF）
    map<string, double> idf = calculate_idf(documents);
    
    // 计算TF-IDF
    map<string, double> term_tfidf_scores;
    map<string, double> tf = calculate_tf(documents[2]);
    for (const auto& pair : tf) {
            // 如果单词不是常用词，则计算TF-IDF
            if (find(stop_words.begin(), stop_words.end(), pair.first) == stop_words.end()) {
                term_tfidf_scores[pair.first] = pair.second * idf[pair.first];
            }
        }

    // 按TF-IDF值降序排序
    vector<pair<string, double>> sorted_terms(term_tfidf_scores.begin(), term_tfidf_scores.end());
    sort(sorted_terms.begin(), sorted_terms.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
        return a.second > b.second;
    });
    
    // 输出排序后的文本术语
    cout << "Top terms after removing stop words, sorted by TF-IDF score:" << endl;
    int count = 0;
    for (const auto& term : sorted_terms) {
        cout << term.first << ": " << term.second << endl;
        count++;
        if(count==10)break;
    }
    
    return 0;
}
