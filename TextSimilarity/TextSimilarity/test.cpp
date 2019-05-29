#include "TextSimilarity.h"
#include <fstream>
#include <cassert>
#include <string>
#include <Windows.h>

using namespace std;

//const char* const DICT_PATH = "jieba.dict.utf8";
//const char* const HMM_PATH = "hmm_model.utf8";
//const char* const USER_DICT_PATH = "user.dict.utf8";
//const char* const IDF_PATH = "idf.utf8";
//const char* const STOP_WORD_PATH = "stop_words.utf8";


int main() {

	TextSimilarity test("dict");

	TextSimilarity::wordFreq exWF1;
	TextSimilarity::wordFreq exWF2;

	exWF1 = test.getWordFreq("test1.txt");
	exWF2 = test.getWordFreq("test2.txt");

	vector<pair<string, int>> wfVec1 = test.sortByValueReverse(exWF1);
	vector<pair<string, int>> wfVec2 = test.sortByValueReverse(exWF2);

	cout << "wfVec1:" << endl;
	for (int i = 0; i < 10; i++){

		cout << test.UTF8ToGBK(wfVec1[i].first) << ":" << wfVec1[i].second << " , ";
	}
	cout << endl;

	cout << "wfVec2:" << endl;
	for (int i = 0; i < 10; i++){

		cout << test.UTF8ToGBK(wfVec2[i].first) << ":" << wfVec2[i].second << " , ";
	}
	cout << endl;

	/*
	TestSimilarity::wordFreq::const_iterator map_it;
	for (map_it = wf.begin(); map_it != wf.end(); map_it++)
	{
	cout << "(\"" << test.UTF8ToGBK(map_it->first) << "\"," << map_it->second << ")" << endl;
	}
	*/

	/*
	TestSimilarity::wordSet wSet1;
	TestSimilarity::wordSet wSet2;

	test.selectAimWords(wfVec1, wSet1);
	cout << "wSet1:" << endl;
	for (const auto& e : wSet1){
	cout << test.UTF8ToGBK(e) << ", ";
	}
	cout << endl;

	test.selectAimWords(wfVec2, wSet2);
	cout << "wSet2:" << endl;
	for (const auto& e : wSet2){
	cout << test.UTF8ToGBK(e) << ", ";
	}
	cout << endl;
	*/


	TextSimilarity::wordSet wSet;

	test.selectAimWords(wfVec1, wSet);
	cout << "wSet:" << endl;
	test.selectAimWords(wfVec2, wSet);
	for (const auto& e : wSet){
		cout << test.UTF8ToGBK(e) << ", ";
	}
	cout << endl;

	vector<double> exVec1;
	vector<double> exVec2;
	exVec1 = test.getOneHot(wSet, exWF1);
	exVec2 = test.getOneHot(wSet, exWF2);

	cout << "exVec1:" << endl;
	for (const auto& v : exVec1){
		cout << v << ", ";
	}
	cout << endl;

	cout << "exVec2:" << endl;
	for (const auto& v : exVec2){
		cout << v << ", ";
	}
	cout << endl;

	double db = 0;
	db = test.cosine(exVec1, exVec2);
	cout << "文本相似度为：" << db * 100 <<"%"<< endl;

	system("pause");
	return 0;
}

