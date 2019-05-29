#define _CRT_SECURE_NO_WARNINGS 1

#ifndef _TESTSIMILARITY_H
#define _TESTSIMILARITY_H

#include "cppjieba/Jieba.hpp"
#include <string>

class TextSimilarity{

public:


	typedef std::unordered_map<std::string, int> wordFreq;
	typedef std::unordered_set<std::string> wordSet;

	TextSimilarity(std::string dict);
	std::vector<double> getOneHot(wordSet& wset, wordFreq& wf);

	void getStopWordTable(const char* stopWordFile);
	wordFreq getWordFreq(const char* file);
	std::string UTF8ToGBK(std::string str);
	std::string GBKToUTF8(std::string str);
	std::vector<std::pair<std::string, int>> sortByValueReverse(wordFreq& wf);
	void selectAimWords(std::vector<std::pair<std::string, int>>& wfvec, wordSet& wset);

	double cosine(std::vector<double> oneHot1, std::vector<double> oneHot2);

	double getTextSimilarity(const char* file1, const char* file2);

private:

	std::string DICT;
	std::string DICT_PATH;
	std::string HMM_PATH;
	std::string USER_DICT_PATH;
	std::string IDF_PATH;
	std::string STOP_WORD_PATH;
	cppjieba::Jieba _jieba;

	wordSet _stopWordSet;
	int _maxWordNumber;
};




#endif//_TESTSIMILARITY_H