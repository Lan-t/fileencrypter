#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <vector>

#using "mscorlib.dll"
#using "System.IO.Compression.FileSystem.dll"


using namespace std;

int8_t key[8] = { 0 }; // ��(�o�C�g��8)
string fileNameO = "";


string stos(System::String^ a)
{
	string b = "";
	for (int i = 0; i < a->Length; i++)
	{
		b += a[i];
	}
	return b;
}

void get_key() // ���ϐ�USERNAME����8����������(8���������̍ۂ͌J��Ԃ�)
{
	System::String ^username_String = System::Environment::GetEnvironmentVariable("username");
	string username = stos(username_String);

	for (int i = 0, j = 0; j < 8; i++, j++)
	{
		if (i == username.size())
		{
			i = 0;
		}
		key[j] = (int8_t)username[i];
	}
}

void cipher(vector<int8_t>& bin)
{
	int j = 0;
	for (int i = 0; i < bin.size(); i++)
	{
		try
		{
			bin.at(i);
		}
		catch (out_of_range e)
		{
			break;
		}

		bin[i] = bin[i] ^ key[j];
		j = (j + 1) % 8;

	}
}

string zip(System::String ^path)
{
	//System::String ^a;
	//a = path->Substring(0, path->LastIndexOf("\\")+1);
	//a += gcnew System::String(fileNameO.c_str());
	//System::Console::WriteLine(a);
	
	try {
		System::IO::Compression::ZipFile::CreateFromDirectory(path, gcnew System::String(fileNameO.c_str()));
	}
	catch (System::IO::IOException^ e)
	{
		System::Console::WriteLine(
			"{0}�͊��ɑ��݂��܂��B\n�폜���Ă�����x��蒼���Ă�������",
			gcnew System::String(fileNameO.c_str()));
		getchar();
		return "ERROR";
	}

	return fileNameO;
}


int main(int args, char** argv)
{

	if (args != 2)
	{
		std::cout << "�t�@�C�����h���b�O�A���h�h���b�v���ċN�����Ă�������" << endl;
		getchar();
		return -1;
	}

	get_key();

	string fileNameI = argv[1];
	
	//std::cout << "�o�͂���t�@�C���� "; cin >> fileNameO;
	fileNameO = fileNameI;

	struct stat s;
	if (stat(fileNameI.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
		{
			fileNameO += ".zipE";
			fileNameI = zip(gcnew System::String(fileNameI.c_str()));

			if (fileNameI == "ERROR")
			{
				return -1;
			}
		}
		else if (fileNameI[fileNameI.size() - 1] == 'E')
		{
			/*string buf = fileNameI;
			buf.erase(buf.size() - 1);
			fileNameO += buf.substr(buf.find_last_of("."));*/

			fileNameO.erase(fileNameO.size() - 1);

		}
		else
		{
			//fileNameO += fileNameI.substr(fileNameI.find_last_of("."));
			fileNameO += "E";
		}
	}
	


	fstream fin(fileNameI, ios::binary | ios::in);
	if (!fin)
	{
		std::cout << "error_Can'tOpen" << endl;
		getchar();
		return -1;
	}
	std::cout << "Opened" << endl;


	int8_t a;
	vector<int8_t> bin;


	/* �ǂݎ�� */

	while (!fin.eof())
	{
		fin.read((char*)&a, 1);
		bin.push_back(a);
	}
	bin.pop_back();

	std::cout << "Read" << endl;


	fin.close();


	/* �Í��� */

	cipher(bin);


	/* �������� */

	fstream fo;

	fo.open(fileNameO, ios::out | ios::binary | ios::trunc);

	for (int i = 0; i < bin.size(); i++)
	{
		fo.write((char*)&bin[i], 1);
	}

	std::cout << "wrote" << endl;

	fo.close();


	std::cout << "finished" << endl;
	return 0;
}
