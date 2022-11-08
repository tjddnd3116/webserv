#include <vector>
#include <map>
#include <iostream>


using namespace std;

#define TEST_IDX 4

void printUri(vector<string>& dir, string& fileName)
{
	for (size_t i = 0; i < dir.size(); i++)
		cout << dir[i] << endl;
	cout << "file name : " << fileName << endl;
}

int main(void)
{
	vector<string> uriVec;

	uriVec.push_back("/");
	uriVec.push_back("/aaa/");
	uriVec.push_back("/aaa/bbb");
	uriVec.push_back("/aaa/bbb/");
	uriVec.push_back("/aaa/bbb/ccc");
	uriVec.push_back("/aaa/bb/");

	{
		string			uri = uriVec[TEST_IDX];
		vector<string>	dirVec;
		string			fileName;
		size_t 			slashPos = 0;

		slashPos = uri.find("/", slashPos);
		while (slashPos != string::npos)
		{
			dirVec.push_back(uri.substr(0, ++slashPos));
			fileName = uri.substr(slashPos);
			slashPos = uri.find("/", slashPos);
		}

		printUri(dirVec, fileName);
	}

	return (0);
}

