# tf-idf_test    一个对tf-idf算法的简单应用
## 实现过程及关键代码

​	TF-IDF(term frequency–inverse document frequency)是一种用于信息检索与数据挖掘的常用加权技术，常用于挖掘文章中的关键词。

​	tf-idf算法的具体使用环境需要一个语料库，在这个语料库中有多条文本信息，我们对每条文本信息使用使用tf-idf算法即可求出该文本信息在整个语料库中最有特征的几个关键词。在BBC中复制三篇新闻到对应文本文件中构成一个简单的语料库：

​	文本1为越南一个犯罪人被捕，文本二为美国总统选举的选票支持率，文本三为英国邮票被盗版印刷。

1. 首先将每条文本信息存储到一个vector中。

2. 计算他在该文本信息中的词频TF并存储到一个map中键为string类型的单词，值为其出现的频率。

   t为某个指定单词，d表示某个文本信息

   $$
   TF(t, d) = \frac{n_{t,d}}{\sum_{i} n_{i,d}}
   $$


 ```cpp
   
// 计算词频（TF）
map<string, double> calculate_tf(const vector<string>& words) {
    map<string, double> tf;
    int total_words = words.size();
    for (const auto& word : words) {
        tf[word] += 1.0 / total_words;
    }
    
    return tf;
}
   ```

3. 计算逆文档频率（IDF）并存储到一个map中键为string类型的单词，值为其出现的频率。IDF为含有改单词在整个语料库所有的文档中出现的频率

   N表示语料库中所有文本信息的总数
   $$
   IDF(t, D) = \log{\frac{N}{|\{d \in D : t \in d\}|}}
   $$
   N是总文档数，

   ```cpp
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
   ```

4. 剔除常用词计算tf-idf的得分，并按照tf-idf的得分的降序存储计算结果到sorted_terms中。


$$
   TF-IDF(t, d, D) = TF(t, d) \times IDF(t, D)
$$


```cpp
// 计算逆文档频率（IDF）
    map<string, double> idf = calculate_idf(documents);
    // 计算TF-IDF
    map<string, double> term_tfidf_scores;
    map<string, double> tf = calculate_tf(documents[0]);
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
```



