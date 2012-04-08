//-------------------------------------------------------------------------------
// x86ÇÃñΩóﬂí∑ÇåvéZ
//-------------------------------------------------------------------------------
//
// Copyright(c) 2012 sharkpp All rights reserved.
//
// The MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//-------------------------------------------------------------------------------

// Intel(r) 64 and IA-32 Architectures Software DeveloperÅfs Manual

typedef unsigned char uint8_t;

inline
size_t instructionLength(const uint8_t* codes)
{
	const uint8_t* top = codes;
	uint8_t prefix, opecode;
	bool presentModRM = false;
	bool presentSIB = false;
	uint8_t bitModRM = 0;
	uint8_t bitSIB = 0;
	size_t  displacement = 0;
	size_t  immediate = 0;
	size_t  address_size = 4;

	// Prefix
	prefix = *codes++;

	for(;;)
	{
		// instruction prefixes check from Volume.2A 2.1.1
		switch( prefix )
		{
		case 0x67: // Group 4
			address_size = 2;
		case 0xF0: case 0xF2: case 0xF3: // Group 1
		case 0x2E: case 0x36: case 0x3E: // Group 2
		case 0x26: case 0x64: case 0x65:
		case 0x66: // Group 3
			prefix = *codes++;
			continue;
		// opecode
		default:
			break;
		}
		break;
	}
	opecode = prefix;

	// Opcode
	switch( opecode )
	{
	// XX
	                                                                  case 0x06: case 0x07:
	                                                                  case 0x0E:
	                                                                  case 0x16: case 0x17:
	                                                                  case 0x1E: case 0x1F:
	                                                                             case 0x27:
	                                                                             case 0x2F:
	                                                                             case 0x37:
	                                                                             case 0x3F:
	case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
	case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
	case 0x60: case 0x61:
	                                            case 0x6C: case 0x6D: case 0x6E: case 0x6F:
	case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
	case 0x98: case 0x99:            case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F:
	                                            case 0xA4: case 0xA5: case 0xA6: case 0xA7:
	                      case 0xAA: case 0xAB: case 0xAC: case 0xAD: case 0xAE: case 0xAF:
	                                 case 0xC3:
	           case 0xC9:            case 0xCB: case 0xCC:            case 0xCE: case 0xCF:
	                                                                             case 0xD7:
	                                            case 0xEC: case 0xED: case 0xEE: case 0xEF:
	                                            case 0xF4: case 0xF5:
	case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD:
		break;
	// XX ib/cb
	                                            case 0x04:
	                                            case 0x0C:
	                                            case 0x14:
	                                            case 0x1C:
	                                            case 0x24:
	                                            case 0x2C:
	                                            case 0x34:
	                                            case 0x3C:
	                      case 0x6A:
	case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
	case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7E: case 0x7F:
	                                                       case 0xCD:
	                                            case 0xD4: case 0xD5:
	case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5: case 0xE6: case 0xE7: 
	                                 case 0xEB:
	case 0xA0:            case 0xA2:
		immediate = 1;
		break;
	// XX iw
	case 0xC2: case 0xCA:
		immediate = 2;
		break;
	// XX id/iw
	                                                       case 0x05:
	                                                       case 0x0D:
	                                                       case 0x15:
	                                                       case 0x1D:
	                                                       case 0x25:
	                                                       case 0x2D:
	                                                       case 0x35:
	                                                       case 0x3D:
	case 0x68:
	           case 0xA1:            case 0xA3:
	           case 0xA9:
	case 0xE8: case 0xE9:
		immediate = address_size;
		break;
	// XX cp
	case 0x9A: case 0xEA:
		immediate = address_size + 2;
		break;
	// XX /r
	case 0x00: case 0x01: case 0x02: case 0x03:
	case 0x08: case 0x09: case 0x0A: case 0x0B:
	case 0x10: case 0x11: case 0x12: case 0x13:
	case 0x18: case 0x19: case 0x1A: case 0x1B:
	case 0x20: case 0x21: case 0x22: case 0x23:
	case 0x28: case 0x29: case 0x2A: case 0x2B:
	case 0x30: case 0x31: case 0x32: case 0x33:
	case 0x38: case 0x39: case 0x3A: case 0x3B:
	                      case 0x62: case 0x63: case 0x84: case 0x85: case 0x86: case 0x87:
	case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F:
	                                            case 0xC4: case 0xC5: 
	case 0xD0: case 0xD1: case 0xD2: case 0xD3:
	case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF:
	                                                                  case 0xFE: case 0xFF:
		presentModRM = true;
		break;
	// F6 /r
	case 0xF6:
		presentModRM = true;
		immediate = 0 == (*codes & 0x38) ? 1 : 0;
		break;
	// F7 /r
	case 0xF7:
		presentModRM = true;
		immediate = 0 == (*codes & 0x38) ? address_size : 0;
		break;
	// XX /r ib
	case 0x6B: case 0xC6: case 0x80: case 0x83: case 0xC0: case 0xC1:
		presentModRM = true;
		immediate = 1;
		break;
	// XX /r id/iw
	case 0x69: case 0xC7: case 0x81:
		presentModRM = true;
		immediate = address_size;
		break;
	// XX iw ib
	case 0xC8: 
		displacement = 2;
		immediate = 1;
		break;

	// 0F XX XX
	case 0x0F:
		opecode = *codes++;
		switch( opecode )
		{
		// 0F XX
		case 0x06: case 0x08: case 0x09: case 0x0B: case 0x30: case 0x31: case 0x32: case 0x33:
		case 0x34: case 0x35: case 0x77: case 0xA0: case 0xA1: case 0xA2: case 0xA8: case 0xA9:
		case 0xAA: case 0xC8: case 0xC9: case 0xCA: case 0xCB: case 0xCC: case 0xCD: case 0xCE:
		case 0xCF:
			break;
		// 0F XX /r
		case 0x00: case 0x01: case 0x02: case 0x03:
		case 0x10: case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17:
		case 0x18:
		case 0x20: case 0x21: case 0x22: case 0x23:
		case 0x28: case 0x29: case 0x2A: case 0x2B: case 0x2C: case 0x2D: case 0x2E: case 0x2F:
		case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
		case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
		case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
		case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F:
		case 0x60: case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: case 0x67:
		case 0x68: case 0x69: case 0x6A: case 0x6B: case 0x6C: case 0x6D: case 0x6E: case 0x6F:
		                                            case 0x74: case 0x75: case 0x76:
		                                                                  case 0x7E: case 0x7F:
		case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
		case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F:
		                                 case 0xA3:            case 0xA5: case 0xA6: case 0xA7:
		                                 case 0xAB:            case 0xAD: case 0xAE: case 0xAF:
		case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7:
		                                 case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF:
		case 0xC0: case 0xC1:            case 0xC3:                                  case 0xC7:
		           case 0xD1: case 0xD2: case 0xD3: case 0xD4: case 0xD5:            case 0xD7:
		case 0xD8: case 0xD9: case 0xDA: case 0xDB: case 0xDC: case 0xDD: case 0xDE: case 0xDF:
		case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xE4: case 0xE5:            case 0xE7:
		case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEE: case 0xEF:
		           case 0xF1: case 0xF2: case 0xF3: case 0xF4: case 0xF5: case 0xF6: case 0xF7:
		case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD: case 0xFE:
			presentModRM = true;
			break;
		// 0F XX /r imm8/ib
		case 0xA4: case 0xAC: case 0xBA:
		case 0x70: case 0x71: case 0x72: case 0x73: case 0xC2: case 0xC4: case 0xC5: case 0xC6: 
			presentModRM = true;
			immediate = 1;
			break;
		// 0F XX cb/cw
		case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
		case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F:
			immediate = address_size;
			break;
		}
		break;
	}

	// ModR/M
	if( presentModRM )
	{
		bitModRM = *codes++;

		switch( address_size )
		{
		case 2: // Volume.2A 2.1.5 Table 2-1.
			if(  0x80 == (bitModRM & 0xC0) || // Mod == 10
				(0x00 == (bitModRM & 0xC0) && // Mod == 00 &&
				 0x06 == (bitModRM & 0x07)) ) //   R/M == 110
			{
				displacement = 2;
			}
			if(  0x40 == (bitModRM & 0xC0) ) // Mod == 01
			{
				displacement = 1;
			}
			break;
		case 4: // Volume.2A 2.1.5 Table 2-2.
			// SIB present check
			presentSIB = 0x04 == (bitModRM & 0x07) &&
			             0xC0 != (bitModRM & 0xC0);
			// displacement size check
			if(  0x80 == (bitModRM & 0xC0) || // Mod == 10
				(0x00 == (bitModRM & 0xC0) && // Mod == 00 &&
				 0x05 == (bitModRM & 0x07)) ) //   R/M == 101
			{
				displacement = 4;
			}
			if(  0x40 == (bitModRM & 0xC0) ) // Mod == 01
			{
				displacement = 1;
			}
			break;
		}

		// SIB
		if( presentSIB )
		{
			// Volume.2A 2.1.5 Table 2-3.
			bitSIB = *codes++;
			// displacement size check
			if( 0x00 == (bitModRM & 0xC0) && // Mod == 00 &&
				0x05 == (bitSIB & 0x07) )    //   Base == 101
			{
				displacement = 4;
			}
		}
	}

	codes += displacement + immediate;

	return size_t(codes - top);
}
