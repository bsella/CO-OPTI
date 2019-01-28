#include "BaseTest.h"

BaseTest::BaseTest(const std::string& t):title(t){}
BaseTest::~BaseTest(){}

bool BaseTest::doAllTests(std::ostream& out){
	bool successAll= true;
	uint testsSucceeded= 0, testIndex= 0;
	out << "\033[4mTesting the \033[1m" << title << "\033[0;4m module:\033[0m" << std::endl;
	for(const auto& test : tests){
		std::string message;
		out << "│\tTest " << testIndex << ": ";
		bool success= test.execute(message);
		if(success){
			testsSucceeded++;
			out << "\033[1;32m[OK";
		}
		else
			out << "\033[1;31m[KO";
		out << "] \033[0m"<< message << std::endl;
		testIndex++;
		successAll &= success;
	}
	out << "│\t";
	if(successAll)
		out << "\033[1;32m[OK";
	else
		out << "\033[1;31m[KO";
	out << "]\033[0m " << testsSucceeded << " tests out of " << tests.size() << " have been successfuly executed" << std::endl;
	return successAll;
}

void BaseTest::addTest(std::istringstream(*f)(std::istream&), const std::string& data, const std::string& gt){
	tests.push_back(testSet(f, data, gt));
}

BaseTest::testSet::testSet(
	std::istringstream(*f)(std::istream&),
	const std::string& data,
	const std::string& gt):procedure(f), dataPath(data), gtPath(gt){}

#include <fstream>
// template<typename Scalar>
bool BaseTest::testSet::execute(std::string& message)const{
	std::ifstream dataFile, gtFile;
	dataFile.open(dataPath);
	gtFile.open(gtPath);
	if(!dataFile){
		message = "Could not open file "+ dataPath;
		return false;
	}
	if(!gtFile){
		message = "Could not open file "+ gtPath;
		return false;
	}
	//Scalar tmp, gt;
	double tmp, gt;
	std::istringstream result= (*procedure)(dataFile); 
	while(result >> tmp){
		gtFile >> gt;
		if(gt!=tmp){
			message = "Test failed";
			return false;
		}
	}
	return true;
}