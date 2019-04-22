#include <iostream>
#include <fstream>
#include <conio.h>

int main()
{
	char targetFiles[][10] = { {"7zFM.exe"}, {"7zG.exe"}, {"7z.sfx"}, {""} };

	char orgFont[] = { 0x08, 0x00, 'M', 0x00, 'S', 0x00, ' ', 0x00, 'S', 0x00, 'h', 0x00, 'e', 0x00, 'l', 0x00, 'l', 0x00, ' ', 0x00, 'D', 0x00, 'l', 0x00, 'g' };
	char newFont[] = { 0x0a, 0x00, 'Y', 0x00, 'u', 0x00, ' ', 0x00, 'G', 0x00, 'o', 0x00, 't', 0x00, 'h', 0x00, 'i', 0x00, 'c', 0x00, ' ', 0x00, 'U', 0x00, 'I' };

	for (int t = 0; targetFiles[t][0] != NULL; t++)
	{
		std::ifstream ifs;
		ifs.open(targetFiles[t], std::ios::in | std::ios::binary);

		if (!ifs)
		{
			std::cout << targetFiles[t] << " not found" << std::endl;
			continue;
		}

		ifs.seekg(0, std::fstream::end);
		std::streamoff eofPos = ifs.tellg();
		ifs.clear();
		ifs.seekg(0, std::fstream::beg);
		std::streamoff begPos = ifs.tellg();
		int size = (int)(eofPos - begPos);

		char *buf = new char[size];
		memset(buf, 0, size);
		ifs.read(buf, size);
		ifs.close();

		int oSize = sizeof(orgFont);
		int nSize = sizeof(newFont);

		std::cout << "Search from " << targetFiles[t] << std::endl;
		bool changed = false;
		for (int i = 0; i < size; i++)
		{
			if (buf[i] == orgFont[0])
			{
				for (int j = 1; j < oSize; j++)
				{
					if (buf[i + j] != orgFont[j])
					{
						break;
					}
					else
					{
						if (j == oSize - 1)
						{
							std::cout << "Found at 0x" << std::hex << i << std::endl;
							changed = true;
							for (j = 0; j < nSize; j++)
								buf[i + j] = newFont[j];
							break;
						}
					}
				}

			}
		}

		if (changed)
		{
			std::string newName(targetFiles[t]);
			newName += ".Original";
			std::rename(targetFiles[t], newName.c_str());
			std::cout << "Change to " << newName.c_str() << std::endl;

			std::ofstream ofs;
			ofs.open(targetFiles[t], std::ios::out | std::ios::trunc | std::ios::binary);
			ofs.write(buf, size);
			ofs.close();
			std::cout << "Rewritten and saved" << std::endl;
		}

		delete[] buf;
	}

	std::cout << "Press any key to continue ..." << std::endl;
	_getch();
}
