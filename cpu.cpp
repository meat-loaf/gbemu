namespace gbemu{
CPU::execute(){
	unsigned char opcode = FETCH_OP(pc);
	switch(opcode){
		//NOP
		case 0x0:
			break;
		case 0x03:
			inc_16(_b, _c);
			ticks+=4;
			break;
		case 0x13:
			inc_16(_d, _e);
			ticks+=4;
			break;
		case 0x23:
			inc_16(_h, _l);
			ticks+=4;
			break;
		case 0x33:
			sp+1 & 0xFFFF;
			ticks += 4;
			break;
		case 0x3E:
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
