#include "Automata.h"
#include "parser.h"
#include "FileList.h"

#include <stdio.h>
#include <iostream>
#include <string>

void scan_string(char const *arg);

int main(int argc, char** argv){
	if(argc < 2){
		std::cerr << "Usage : egrepMain [regexp] [?<filename> ...] " << std::endl;
		return 1;
	}
	scan_string(argv[1]);
	FileList* filelist = FileList::getFileListInstance();

	for ( int i = 2; i < argc; i++ )
	{
		std::string str(argv[i]);
		filelist->addFile(str);
	}

	yyparse();
	return 0;
}
