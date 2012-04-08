// test.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <tchar.h>
#include <stdio.h>
#include <string>
#include <vector>

static void generateSIB(uint8_t modrm, const std::vector<uint8_t>& src, std::vector<std::vector<uint8_t> >& result)
{
	for(int sib = 0; sib < 256; sib++)
	{
		if( 0x05 != sib ) {
		//	continue;
		}
		result.push_back(src);
		result.back().push_back(uint8_t(sib));
		switch(modrm >> 6)
		{
		case 0: // 00 [scaled index] + disp32
			if( 0x05 == (sib & 0x07) ) {
				result.back().push_back(0x00);
				result.back().push_back(0x00);
				result.back().push_back(0x00);
				result.back().push_back(0x00);
			}
			break;
		case 1: // 01 [scaled index] + disp8 + [EBP]
			result.back().push_back(0x00);
			break;
		case 2: // 10 [scaled index] + disp32 + [EBP]
			result.back().push_back(0x00);
			result.back().push_back(0x00);
			result.back().push_back(0x00);
			result.back().push_back(0x00);
			break;
		}
	}
}

static void generateModRM(uint8_t modrm, const std::vector<uint8_t>& src, std::vector<std::vector<uint8_t> >& result)
{
	uint8_t prefix = !src.empty() ? src[0] : 0x00;
	result.push_back(src);
	result.back().push_back(modrm);
	if( 0x67 == prefix ) {
		if( 0x06 == (modrm & 0xC7) ) { // disp16
			result.back().push_back(0x00);
			result.back().push_back(0x00);
		} else { // disp16 or disp8
			switch(modrm >> 6)
			{
			case 1: // 01
				result.back().push_back(0x00);
				break;
			case 2: // 10
				result.back().push_back(0x00);
				result.back().push_back(0x00);
				break;
			}
		}
	} else {
		switch( modrm >> 6 )
		{
		case 0: // 00
		case 1: // 01
		case 2: // 10
			if( 0x04 == (modrm & 0x07) ) { // SIB
				std::vector<uint8_t> srcSIB;
				srcSIB.swap(result.back());
				result.pop_back();
				generateSIB(modrm, srcSIB, result);
			} else {
				switch( modrm >> 6 )
				{
				case 0: // 00
					if( 0x05 == (modrm & 0xC7) ) {
						result.back().push_back(0x00);
						result.back().push_back(0x00);
						result.back().push_back(0x00);
						result.back().push_back(0x00);
					}
					break;
				case 1: // 01
					// disp8
					result.back().push_back(0x00);
					break;
				case 2: // 10
					// disp32
					result.back().push_back(0x00);
					result.back().push_back(0x00);
					result.back().push_back(0x00);
					result.back().push_back(0x00);
					break;
				}
			}
			break;
		case 3: // 11
			break;
		}
	}
}

static bool perse_line(const TCHAR* line, std::vector<uint8_t>& result)
{
	const TCHAR* p = line;
	TCHAR tmp[128];
	std::vector<std::vector<uint8_t> > buff(1);

	if( !*line ) {
		return false;
	}

	while( *p && ' ' != *p && NULL != (p = _tcsspnp(p, " ")) )
	{
		const TCHAR* pa = _tcschr(p, ' ');
		size_t len = !pa ? strlen(p) : size_t(pa - p);
		int byte_data = 0;

		if(	!_tcsncicmp(p, "/0", len) || !_tcsncicmp(p, "/1", len) ||
			!_tcsncicmp(p, "/2", len) || !_tcsncicmp(p, "/3", len) ||
			!_tcsncicmp(p, "/4", len) || !_tcsncicmp(p, "/5", len) ||
			!_tcsncicmp(p, "/6", len) || !_tcsncicmp(p, "/7", len) )
		{
			int digit = _tcstol(p + 1, 0, 10);
			std::vector<std::vector<uint8_t> > tmp2;
			for(std::vector<std::vector<uint8_t> >::iterator
					ite = buff.begin(), last= buff.end();
				ite != last; ++ite)
			{
				for(int i = 0; i < 4; i++) { // Mod
					for(int j = 0; j < 8; j++) { // R/M
						uint8_t modrm = uint8_t((i << 6)|(digit<<3)|j);
						generateModRM(modrm, *ite, tmp2);
					}
				}
			}
			buff.swap(tmp2);
		}
		else if( !_tcsncicmp(p, "/r", len) )
		{
			std::vector<std::vector<uint8_t> > tmp2;
			for(std::vector<std::vector<uint8_t> >::iterator
					ite = buff.begin(), last= buff.end();
				ite != last; ++ite)
			{
				for(int i = 0; i < 256; i++) {
					generateModRM(uint8_t(i), *ite, tmp2);
				}
			}
			buff.swap(tmp2);
		}
		else if(	!_tcsnccmp(p, "ib", len)    || !_tcsnccmp(p, "iw", len) ||
					!_tcsnccmp(p, "id", len)    ||
					!_tcsnccmp(p, "cb", len)    || !_tcsnccmp(p, "cw", len) ||
					!_tcsnccmp(p, "cd", len)    || !_tcsnccmp(p, "cp", len) ||
					!_tcsnccmp(p, "imm8",  len) || !_tcsnccmp(p, "moffs8",  len) ||
					!_tcsnccmp(p, "imm16", len) || !_tcsnccmp(p, "moffs16", len) ||
					!_tcsnccmp(p, "imm32", len) || !_tcsnccmp(p, "moffs32", len) )
		{
			size_t length = 0;
			length = !_tcsnccmp(p, "ib",   len) || !_tcsnccmp(p, "cb",     len) ||
			         !_tcsnccmp(p, "imm8", len) || !_tcsnccmp(p, "moffs8", len)    ? 1 : length;
			length = !_tcsnccmp(p, "iw",   len) || !_tcsnccmp(p, "cw",     len) ||
			         !_tcsnccmp(p, "imm16",len) || !_tcsnccmp(p, "moffs16",len)    ? 2 : length;
			length = !_tcsnccmp(p, "id",   len) || !_tcsnccmp(p, "cd",     len) ||
			         !_tcsnccmp(p, "imm32",len) || !_tcsnccmp(p, "moffs32",len)    ? 4 : length;
			length = !_tcsnccmp(p, "cp",   len)                                    ? 6 : length;
			for(std::vector<std::vector<uint8_t> >::iterator
					ite = buff.begin(), last= buff.end();
				ite != last; ++ite)
			{
				for(size_t i = 0; i < length; i++) {
					(*ite).push_back(0xFF);
				}
			}
		}
		else
		{
			uint8_t byte_data = (uint8_t)_tcstol(p, 0, 16);
			std::vector<std::vector<uint8_t> > tmp2;
			for(std::vector<std::vector<uint8_t> >::iterator
					ite = buff.begin(), last= buff.end();
				ite != last; ++ite)
			{
				(*ite).push_back(byte_data);
			}
		}
		p += len + 1;
	}

	if( buff[0].empty() ) {
		return false;
	}

	for(std::vector<std::vector<uint8_t> >::iterator
			ite = buff.begin(), last = buff.end();
		ite != last; ++ite)
	{
		size_t pos = 0;
		result.push_back(uint8_t((*ite).size()));
		result.insert(result.end(), (*ite).begin(), (*ite).end());
		result.push_back('&');
	}

	return true;
}

int make_test_pattern_main(int argc, _TCHAR* argv[])
{
	FILE* fp = fopen(argv[1], "r");
	FILE* fp2 = fopen("pattern.dat", "wb");

	if( !fp ) {
		if( fp2 ) {
			fclose(fp2);
		}
		return -1;
	}

	TCHAR buff[256];

	while( !feof(fp) && !ferror(fp) )
	{
		if( !fgets(buff, 255, fp) ) {
			break;
		}
		char* p = _tcspbrk(buff, "\r\n");
		if( p ) { *p = '\0'; }

		std::vector<uint8_t> result;
		if( perse_line(buff, result) &&
			!result.empty() )
		{
			fwrite(&result[0], 1, result.size(), fp2);
		}
	}

	fclose(fp2);
	fclose(fp);

	return 0;
}

