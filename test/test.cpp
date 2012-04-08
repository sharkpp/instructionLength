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
		printf(
			"usage test.exe option\n"
			"  1. test.exe x86opecode.txt\n"
			"     --> generate pattern.dat\n"
			"  2. test.exe\n"
			"     --> run test\n"
			);
		return -1;
	}

	std::vector<uint8_t> buff;
	while( !feof(fp) && !ferror(fp) )
	{
		size_t offset = buff.size();
		buff.resize(offset + 1024*1024);
		buff.resize(offset + fread(&buff[0] + offset, 1, 1024*1024, fp));
	}

	size_t error_disp_num = 10;
	size_t error_num = 0;
	size_t test_num = 0;

	size_t offset = 0;

	while( offset < buff.size() )
	{
		size_t size_ = buff[offset];
		size_t size = instructionLength(&buff[0] + offset + 1);
		// テストに失敗したので詳細をダンプ
		if( '&' != buff[offset + size + 1] )
		{
			if( 0 < error_disp_num ) {
				char tmp[256] = {};
				for(size_t i = 0; i < size_; i++) {
					sprintf(tmp + strlen(tmp), "%02X ", buff[offset + 1 + i]);
				}
				printf("offset:%08Xh %2d!=%2d code:[%s]\n", offset, size_, size, tmp);
				error_disp_num--;
			}
			error_num++;
		}
		offset += size_ + 1 + 1;
		test_num++;
	}

	fclose(fp);

	printf("fail/total = %u/%u : Test %s.\n"
		, error_num, test_num, !error_num ? "successful" : "failed");

	return 0;
}

