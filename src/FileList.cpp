#include "FileList.h"

FileList* FileList::instance;

FileList::FileList()
{
	
}

FileList* FileList::getFileListInstance()
{
	return (instance ? instance : instance = new FileList());
}

void FileList::addFile(std::string& name)
{
	files.push_back(name);
}

int FileList::nbFiles()
{
	return files.size();
}

std::string FileList::getFileName(int index)
{
	return files[index];
}


