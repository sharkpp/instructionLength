// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "../include/instructionLength.hpp"
#include <vector>

int make_test_pattern_main(int argc, _TCHAR* argv[]);

int _tmain(int argc, _TCHAR* argv[])
{
	if( 1 < argc )
	{
		return make_test_pattern_main(argc, argv);
	}

	FILE* fp = fopen("pattern.dat", "rb");
	if( !fp ) {
		return -1;
	}

	std::vector<uint8_t> buff;
	while( !feof(fp) && !ferror(fp) )
	{
		size_t offset = buff.size();
		buff.resize(offset + 1024*1024);
		buff.resize(offset + fread(&buff[0] + offset, 1, 1024*1024, fp));
	}

	int err_ignore_num = 10;

	size_t offset = 0;

	while( offset < buff.size() && 0 < err_ignore_num )
	{
		size_t size_ = buff[offset];
		size_t size = instructionLength(&buff[0] + offset + 1);
		if( '&' != buff[offset + size + 1] ) {
			char tmp[256] = {};
			for(size_t i = 0; i < size_; i++) {
				sprintf(tmp + strlen(tmp), "%02X ", buff[offset + 1 + i]);
			}
			printf("offset:%08Xh [%s] %d!=%d\n", offset, tmp, size_, size);
			err_ignore_num--;
		}
		offset += size_ + 1 + 1;
	}

	fclose(fp);

	return 0;
}

