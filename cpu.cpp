namespace gbemu{
CPU::execute(){
	//TODO FETCH_OP should automatically increased ticks by 4,
	//adjustmets made in opcodes that are a multiple of 4
	unsigned char opcode = FETCH_OP(pc);
	switch(opcode){
		//NOP
		case 0x0:
			break;
		case 0x03:
			inc_16(_b, _c);
			ticks+=4;
			break;
		case 0x04:
			inc_8(_b);
			break;
		case 0x05:
			dec_8(_b);
			break;
		case 0x0B:
			dec_16(_b, _c);
			ticks += 4;
			break;
		case 0x0C:
			inc_8(_c);
			break;
		case 0x0D:
			dec_8(_c);
			break;
		case 0x13:
			inc_16(_d, _e);
			ticks+=4;
			break;
		case 0x14:
			inc_8(_d);
			break;
		case 0x15:
			dec_8(_d);	
			break;
		case 0x1B:
			dec_16(_d, _e);
			ticks += 4;
			break;
		case 0x1C:
			inc_8(_e);
			break;
		case 0x1D:
			dec_8(_e);
			break;
		case 0x23:
			inc_16(_h, _l);
			ticks+=4;
			break;
		case 0x24:
			inc_8(_h);
			break;
		case 0x25:
			dec_8(_h);	
			break;
		case 0x2B:
			dec_16(_h, _l);
			ticks+=4;
			break;
		case 0x2C:
			inc_8(_l);
			break;
		case 0x2D:
			dec_8(_l);
			break;
		case 0x33:
			sp = sp+1 & 0xFFFF;
			ticks+=4;
			break;
		case 0x34:
		case 0x35:
			//inc/dec by (HL)
			break;
		case 0x3B:
			sp = sp-1 & 0xFFFF;
			ticks+= 4;
			break;
		case 0x3C:
			inc_8(_a);
			break;
		case 0x3D:
			dec_8(_a);
			break;
		case 0xCB:
			{
			unsigned char opcode_2 = FETCH_OP(pc);
			switch(opcode_2){
				case 0x30:
					swap(_b);
					break;
				case 0x31:
					swap(_c);
					break;
				case 0x32:
					swap(_d);
					break;
				case 0x33:
					swap(_e);
					break;
				case 0x34:
					swap(_h);
					break;
				case 0x35:
					swap(_l);
					break;
			}
			}
			
	}
}

}
