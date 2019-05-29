#include "TextSimilarity.h"
#include <iostream>
#include <Windows.h>


using namespace std;

TextSimilarity::TextSimilarity(string dict):DICT(dict)
, DICT_PATH(dict + "/jieba.dict.utf8")
, HMM_PATH(dict + "/hmm_model.utf8")
, USER_DICT_PATH(dict + "/user.dict.utf8")
, IDF_PATH(dict + "/idf.utf8")
, STOP_WORD_PATH(dict + "/stop_words.utf8")
, _jieba(DICT_PATH,
HMM_PATH,
USER_DICT_PATH,
IDF_PATH,
STOP_WORD_PATH)
, _maxWordNumber(10)
{
	// 获取停用词表
	getStopWordTable(STOP_WORD_PATH.c_str());
}

string TextSimilarity::GBKToUTF8(std::string str){

	// UTF8 -> UTF16
	// 返回所需要buffer大小
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wstr, len);

	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* utf8_str = new char[len + 1];
	memset(utf8_str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8_str, len, NULL, NULL);
	string strtmp(utf8_str);
	if (wstr){
		delete[] wstr;
		wstr = NULL;
	}

	if (utf8_str){
		delete[] utf8_str;
		utf8_str = NULL;
	}

	return strtmp;
}

string TextSimilarity::UTF8ToGBK(std::string str){

	// UTF8 -> UTF16
	// 返回所需要buffer大小
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr, len);

	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* gb_str = new char[len + 1];
	memset(gb_str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, gb_str, len, NULL, NULL);
	string strtmp(gb_str);
	if (wstr){
		delete[] wstr;
		wstr = NULL;
	}

	if (gb_str){
		delete[] gb_str;
		gb_str = NULL;
	}

	return strtmp;
}

TextSimilarity::wordFreq TextSimilarity::getWordFreq(const char* file){


	string s;
	vector<string> words;
	wordFreq wf;

	ifstream infile; // ifstream 读取文件（ofstream 写入文件）
	infile.open(file);  // 不加打开方式默认 in打开
	assert(infile.is_open());


	while (getline(infile, s)){

		s = GBKToUTF8(s);

		// jieba分词
		_jieba.Cut(s, words, false);

		/*
		去除停用词
		for (int i = 0; i < words.size(); i++){

		// count>0表示 该词在停用词表中出现过
		if (_stopWordSet.count(words[i]) > 0){
		continue;
		}
		// 不是停用词则对应词频++
		else{
		if (wf.count(words[i]) > 0){
		wf[words[i]]++;
		}
		else
		{
		wf[words[i]] = 1;
		}
		//wf[words[i]]++;
		}

		}
		*/

		for (const auto& e : words){
			if (_stopWordSet.count(e)){
				continue;
			}
			else
			{
				if (wf.count(e) > 0){
					wf[e]++;
				}
				else{
					wf[e] = 1;
				}
			}
		}

	}


	infile.close();
	return wf;
}

void TextSimilarity::getStopWordTable(const char* stopWordFile){

	ifstream infile;
	string line;

	infile.open(stopWordFile);
	assert(infile.is_open());

	while (!infile.eof()){
		getline(infile, line);

		_stopWordSet.insert(line);
	}

	infile.close();

}

bool cmpReverse(pair<string, int> lp, pair<string, int> rp){

	return lp.second > rp.second;
}

// 词频排序
vector<pair<string, int>> TextSimilarity::sortByValueReverse(wordFreq& wf){

	vector<pair<string, int>>wfvector(wf.begin(), wf.end());
	sort(wfvector.begin(), wfvector.end(), cmpReverse);
	return wfvector;
}

// 选择不重复的词
void TextSimilarity::selectAimWords(vector<pair<string, int>>& wfvec, wordSet& wset){

	int len = wfvec.size();
	int sz = len > _maxWordNumber ? _maxWordNumber : len;
	for (int i = 0; i < sz; i++){
		wset.insert(wfvec[i].first);
	}
}

// 获取向量
vector<double> TextSimilarity::getOneHot(wordSet& wset, wordFreq& wf){

	vector<double> exVec;
	for (const auto& e : wset){
		if (wf.count(e)){
			exVec.push_back(wf[e]);
		}
		else{
			exVec.push_back(0);
		}
	}
	return exVec;
}

// 计算相似度
double TextSimilarity::cosine(vector<double> oneHot1, vector<double> oneHot2){

	double modular1 = 0, modular2 = 0;
	double products = 0;
	double ret = 0;
	assert(oneHot1.size() == oneHot2.size());
	for (int i = 0; i < oneHot1.size(); i++)
	{
		products += oneHot1[i] * oneHot2[i];
	}
	for (int i = 0; i < oneHot1.size(); i++)
	{
		modular1 += pow(oneHot1[i], 2);
		modular2 += pow(oneHot2[i], 2);
	}
	ret = pow(modular1, 0.5) * pow(modular2, 0.5);
	return products / ret;

}