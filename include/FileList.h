#ifndef H_FILELIST
#define H_FILELIST

#include <string>
#include <vector>

class FileList
{
	private:
		static FileList* instance;
		std::vector<std::string> files;
		
		FileList();
		FileList(const FileList&);
    	void operator=(const FileList&);

	public:
		static FileList* getFileListInstance();
		
		void addFile(std::string& name);
		int nbFiles();
		std::string getFileName(int index);
};



#endif
