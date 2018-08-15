#include "cpu.hpp"
#include <iostream>

//TODO better undefined opcode handling so it isnt the same as STOP
//throw exception?
namespace gbemu{
void GBCPU::execute(){
	unsigned char opcode = mem->read(_pc);
	_pc++; ticks+=4;
	switch(opcode){
		case 0x0:
			break;
		case 0x01:
			reg_load_u16(_b, _c, mem->read(_pc), mem->read(_pc+1));
			_pc+=2;
			ticks+=8;
			break;
		case 0x02:
			write_a_to_mem(_b, _c);
			ticks+=4;
			break;
		case 0x03:
			inc_u16(_b, _c);
			ticks+=4;
			break;
		case 0x04:
			inc_u8(_b);
			break;
		case 0x05:
			dec_u8(_b);
			break;
		case 0x06:
			reg_load(_b, mem->read(_pc));
			_pc+=1;
			ticks+=4;
			break;
		case 0x07:
			rlca();
			break;
		case 0x08:
			write_sp(mem->read(_pc) << 8 | mem->read(_pc+1));
			ticks+=16;
			break;
		case 0x09:
			reg_add_u16(_h, _l, _bc);
			break;
		case 0x0A:
			reg_load(_a, mem->read(_bc));
			ticks+=4;
			break;
		case 0x0B:
			dec_u16(_b, _c);
			ticks+=4;
			break;
		case 0x0C:
			inc_u8(_c);
			break;
		case 0x0D:
			dec_u8(_c);
			break;
		case 0x0E:
			reg_load(_c, mem->read(_pc));
			_pc+=1;
			ticks+=4;
		case 0x0F:
			rrca();
			break;
		case 0x10:
			runnable = false;
			break;
		case 0x11:
			reg_load_u16(_d, _e, mem->read(_pc), mem->read(_pc+1));
			_pc+=2;
			ticks+=8;
			break;
		case 0x12:
			write_a_to_mem(_d, _e);
			ticks +=4;
			break;	
		case 0x13:
			inc_u16(_d, _e);
			ticks+=4;
			break;
		case 0x14:
			inc_u8(_d);
			break;
		case 0x15:
			dec_u8(_d);	
			break;
		case 0x16:
			reg_load(_d, mem->read(_pc));
			_pc+=1;
			ticks+=4;
		case 0x17:
			rla();
			break;
		case 0x18:
			_pc += static_cast<signed char>(mem->read(_pc));
			ticks+=8;
			break;
		case 0x19:
			reg_add_u16(_h, _l, _de);
			ticks+=4;
			break;
		case 0x1A:
			reg_load(_a, mem->read(_de));
			ticks+=4;
			break;
		case 0x1B:
			dec_u16(_d, _e);
			ticks+=4;
			break;
		case 0x1C:
			inc_u8(_e);
			break;
		case 0x1D:
			dec_u8(_e);
			break;
		case 0x1E:
			reg_load(_e, mem->read(_pc));
			_pc+=1;
			ticks+=4;
			break;
		case 0x1F:
			rra();
			break;
		case 0x20:
			cond_jmp_rel(zf, static_cast<signed char>(mem->read(_pc)));
			_pc+=1;
			break;
		case 0x21:
			reg_load_u16(_h, _l, mem->read(_pc), mem->read(_pc+1));
			_pc+=2;
			ticks+=8;
			break;
		case 0x22:
			write_a_to_mem(_h, _l);
			inc_u16(_h, _l);
			ticks+=4;
			break;
		case 0x23:
			inc_u16(_h, _l);
			ticks+=4;
			break;
		case 0x24:
			inc_u8(_h);
			break;
		case 0x25:
			dec_u8(_h);	
			break;
		case 0x26:
			reg_load(_h, mem->read(_pc));
			_pc+=1;
			ticks+=4;
			break;
		case 0x27:
			daa();
			break;
		case 0x28:
			cond_jmp_rel(!zf, static_cast<signed char>(mem->read(_pc)));
			break;
		case 0x29:
			reg_add_u16(_h, _l, _hl);
			ticks+=4;
			break;
		case 0x2A:
			reg_load(_a, mem->read(_hl));
			inc_u16(_h, _l);
			ticks+=4;
			break;
		case 0x2B:
			dec_u16(_h, _l);
			ticks+=4;
			break;
		case 0x2C:
			inc_u8(_l);
			break;
		case 0x2D:
			dec_u8(_l);
			break;
		case 0x2E:
			reg_load(_l, mem->read(_pc));
			_pc+=1;
			ticks+=4;
			break;
		case 0x2F:
			_a = ~_a;
			break;
		case 0x30:
			cond_jmp_rel(!cf, static_cast<signed char>(mem->read(_pc)));
			break;
		case 0x31:
			write_sp(mem->read(_pc) + (mem->read(_pc+1) << 8));
			break;
		case 0x32:
			mem->write(_hl, _a);
			inc_u16(_h, _l);
			break;
		case 0x33:
			_sp = _sp+1 & 0xFFFF;
			ticks+=4;
			break;
		case 0x34:
			inc_u8_mem(_hl);
			ticks+=8;
			break;
		case 0x35:
			dec_u8_mem(_hl);
			ticks+=8;
			break;
		case 0x36:
			mem->write(_hl, mem->read(_pc));
			_pc+=1;
			ticks+=8;
			break;
		case 0x37:
			cf = 1;
			break;
		case 0x38:
			cond_jmp_rel(cf, static_cast<signed char>(mem->read(_pc)));
			break;
		case 0x39:
			reg_add_u16(_h, _l, _sp);
			ticks+=4;
			break;
		case 0x3A:
			reg_load(_a, mem->read(_hl));
			dec_u16(_h, _l);
			ticks+=4;
			break;
		case 0x3B:
			_sp = _sp-1 & 0xFFFF;
			ticks+= 4;
			break;
		case 0x3C:
			inc_u8(_a);
			break;
		case 0x3D:
			dec_u8(_a);
			break;
		case 0x3F:
			cf = !cf;
			break;
		case 0x40:
			reg_load(_b, _b);
			break;
		case 0x41:
			reg_load(_b, _c);
			break;
		case 0x42:
			reg_load(_b, _d);
			break;
		case 0x43:
			reg_load(_b, _e);
			break;
		case 0x44:
			reg_load(_b, _h);
			break;
		case 0x45:
			reg_load(_b, _l);
			break;
		case 0x46:
			reg_load(_b, mem->read(_hl));
			ticks+=4;
			break;
		case 0x47:
			reg_load(_b, _a);
			break;
		case 0x48:
			reg_load(_c, _b);
			break;
		case 0x49:
			reg_load(_c, _c);
			break;
		case 0x4A:
			reg_load(_c, _d);
			break;
		case 0x4B:
			reg_load(_c, _e);
			break;
		case 0x4C:
			reg_load(_c, _h);
			break;
		case 0x4D:
			reg_load(_c, _l);
			break;
		case 0x4E:
			reg_load(_b, mem->read(_hl));
			ticks+=4;
			break;
		case 0x4F:
			reg_load(_c, _a);
			break;
		case 0x50:
			reg_load(_d, _b);
			break;
		case 0x51:
			reg_load(_d, _c);
			break;
		case 0x52:
			reg_load(_d, _d);
			break;
		case 0x53:
			reg_load(_d, _e);
			break;
		case 0x54:
			reg_load(_d, _h);
			break;
		case 0x55:
			reg_load(_d, _l);
			break;
		case 0x56:
			reg_load(_d, mem->read(_hl));
			ticks+=4;
			break;
		case 0x57:
			reg_load(_d, _a);
			break;
		case 0x58:
			reg_load(_e, _b);
			break;
		case 0x59:
			reg_load(_e, _c);
			break;
		case 0x5A:
			reg_load(_e, _d);
			break;
		case 0x5B:
			reg_load(_e, _e);
			break;
		case 0x5C:
			reg_load(_e, _h);
			break;
		case 0x5D:
			reg_load(_e, _l);
			break;
		case 0x5E:
			reg_load(_e, mem->read(_hl));
			ticks+=4;
			break;
		case 0x5F:
			reg_load(_e, _a);
			break;
		case 0x60:
			reg_load(_h, _b);
			break;
		case 0x61:
			reg_load(_h, _c);
			break;
		case 0x62:
			reg_load(_h, _d);
			break;
		case 0x63:
			reg_load(_h, _e);
			break;
		case 0x64:
			reg_load(_h, _h);
			break;
		case 0x65:
			reg_load(_h, _l);
			break;
		case 0x66:
			reg_load(_h, mem->read(_hl));
			ticks+=4;
			break;
		case 0x67:
			reg_load(_h, _a);
			break;
		case 0x68:
			reg_load(_l, _b);
			break;
		case 0x69:
			reg_load(_l, _c);
			break;
		case 0x6A:
			reg_load(_l, _d);
			break;
		case 0x6B:
			reg_load(_l, _e);
			break;
		case 0x6C:
			reg_load(_l, _h);
			break;
		case 0x6D:
			reg_load(_l, _l);
			break;
		case 0x6E:
			reg_load(_l, mem->read(_hl));
			ticks+=4;
			break;
		case 0x6F:
			reg_load(_l, _a);
			break;
		case 0x70:
			mem->write(_hl, _b);
			ticks+=4;
			break;
		case 0x71:
			mem->write(_hl, _c);
			ticks+=4;
			break;
		case 0x72:
			mem->write(_hl, _d);
			ticks+=4;
			break;
		case 0x73:
			mem->write(_hl, _e);
			ticks+=4;
			break;
		case 0x74:
			mem->write(_hl, _h);
			ticks+=4;
			break;
		case 0x75:
			mem->write(_hl, _l);
			ticks+=4;
			break;
		//TODO HALT
		case 0x76:
			break;
		case 0x77:
			mem->write(_hl, _a);
			break;
		case 0x78:
			reg_load(_a, _b);
			break;
		case 0x79:
			reg_load(_a, _c);
			break;
		case 0x7A:
			reg_load(_a, _d);
			break;
		case 0x7B:
			reg_load(_a, _e);
			break;
		case 0x7C:
			reg_load(_a, _h);
			break;
		case 0x7D:
			reg_load(_a, _l);
			break;
		case 0x7E:
			reg_load(_a, mem->read(_hl));
			ticks+=4;
			break;
		case 0x7F:
			reg_load(_a, _a);
			break;
		case 0x80:
			reg_a_add_u8(_b, false);
			break;
		case 0x81:
			reg_a_add_u8(_c, false);
			break;
		case 0x82:
			reg_a_add_u8(_d, false);
			break;
		case 0x83:
			reg_a_add_u8(_e, false);
			break;
		case 0x84:
			reg_a_add_u8(_h, false);
			break;
		case 0x85:
			reg_a_add_u8(_l, false);
			break;
		case 0x86:
			reg_a_add_u8(mem->read(_hl), false);
			ticks+=8;
			break;
		case 0x87:
			reg_a_add_u8(_a, false);
			break;
		case 0x88:
			reg_a_add_u8(_b, true);
			break;
		case 0x89:
			reg_a_add_u8(_c, true);
			break;
		case 0x8A:
			reg_a_add_u8(_d, true);
			break;
		case 0x8B:
			reg_a_add_u8(_e, true);
			break;
		case 0x8C:
			reg_a_add_u8(_h, true);
			break;
		case 0x8D:
			reg_a_add_u8(_l, true);
			break;
		case 0x8E:
			reg_a_add_u8(mem->read(_hl), true);
			break;
		case 0x8F:
			reg_a_add_u8(_a, true);
		case 0x90:
			reg_a_sub_u8(_b, false);
			break;
		case 0x91:
			reg_a_sub_u8(_c, false);
			break;
		case 0x92:
			reg_a_sub_u8(_d, false);
			break;
		case 0x93:
			reg_a_sub_u8(_e, false);
			break;
		case 0x94:
			reg_a_sub_u8(_h, false);
			break;
		case 0x95:
			reg_a_sub_u8(_l, false);
			break;
		case 0x96:
			reg_a_sub_u8(mem->read(_hl), false);
			break;
		case 0x97:
			reg_a_sub_u8(_a, false);
			break;
		case 0x98:
			reg_a_sub_u8(_b, true);
			break;
		case 0x99:
			reg_a_sub_u8(_c, true);
			break;
		case 0x9A:
			reg_a_sub_u8(_d, true);
			break;
		case 0x9B:
			reg_a_sub_u8(_e, true);
			break;
		case 0x9C:
			reg_a_sub_u8(_h, true);
			break;
		case 0x9D:
			reg_a_sub_u8(_l, true);
			break;
		case 0x9E:
			reg_a_sub_u8(mem->read(_hl), true);
			break;
		case 0x9F:
			reg_a_sub_u8(_a, true);
			break;
		case 0xA0:
			reg_a_and_u8(_b);
			break;
		case 0xA1:
			reg_a_and_u8(_c);
			break;
		case 0xA2:
			reg_a_and_u8(_d);
			break;
		case 0xA3:
			reg_a_and_u8(_e);
			break;
		case 0xA4:
			reg_a_and_u8(_h);
			break;
		case 0xA5:
			reg_a_and_u8(_l);
			break;
		case 0xA6:
			reg_a_and_u8(mem->read(_hl));
			break;
		case 0xA7:
			//does nothing but set flags
			reg_a_and_u8(_a);
			break;
		case 0xA8:
			reg_a_xor_u8(_b);
			break;
		case 0xA9:
			reg_a_xor_u8(_c);
			break;
		case 0xAA:
			reg_a_xor_u8(_d);
			break;
		case 0xAB:
			reg_a_xor_u8(_e);
			break;
		case 0xAC:
			reg_a_xor_u8(_h);
			break;
		case 0xAD:
			reg_a_xor_u8(_l);
			break;
		case 0xAE:
			reg_a_xor_u8(mem->read(_hl));
			break;
		case 0xAF:
			reg_a_xor_u8(_a);
			break;
		case 0xB0:
			reg_a_or_u8(_b);
			break;
		case 0xB1:
			reg_a_or_u8(_c);
			break;
		case 0xB2:
			reg_a_or_u8(_d);
			break;
		case 0xB3:
			reg_a_or_u8(_e);
			break;
		case 0xB4:
			reg_a_or_u8(_h);
			break;
		case 0xB5:
			reg_a_or_u8(_l);
			break;
		case 0xB6:
			reg_a_or_u8(mem->read(_hl));
			break;
		case 0xB7:
			reg_a_or_u8(_a);
			break;
		case 0xB8:
			reg_a_cmp_u8(_b);
			break;
		case 0xB9:
			reg_a_cmp_u8(_c);
			break;
		case 0xBA:
			reg_a_cmp_u8(_d);
			break;
		case 0xBB:
			reg_a_cmp_u8(_e);
			break;
		case 0xBC:
			reg_a_cmp_u8(_h);
			break;
		case 0xBD:
			reg_a_cmp_u8(_l);
			break;
		case 0xBE:
			reg_a_cmp_u8(mem->read(_hl));
			break;
		case 0xBF:
			reg_a_cmp_u8(_a);
			break;
		//ret if flags is not zero...but we store complement
		case 0xC0:
			ret(zf);
			break;
		case 0xC1:
			pop(_b, _c);	
			break;
		case 0xC2:
			cond_jmp_abs(zf);
			break;
		case 0xC3:
			write_pc(mem->read(_pc+1), mem->read(_pc));
			ticks += 8;
			break;
		case 0xC4:
			call(zf, IMM_16_PC); 
			break;
		case 0xC5:
			push(_b, _c);
			break;
		case 0xC6:
			add_a_imm_mem(_pc, false);
			break;
		case 0xC7:
			rst(0x0);
			break;
		case 0xC8:
			ret(!zf);
			break;
		case 0xC9:
			ret(true);
			break;
		case 0xCA:
			cond_jmp_abs(!zf);
			break;
		case 0xCB:
		{
			unsigned char opcode_2 = mem->read(_pc);
			_pc++; ticks+=4;
			switch(opcode_2){
				case 0x00:
					rlc(_b);
					break;
				case 0x01:
					rlc(_c);
					break;
				case 0x02:
					rlc(_d);
					break;
				case 0x03:
					rlc(_e);
					break;
				case 0x04:
					rlc(_h);
					break;
				case 0x05:
					rlc(_l);
					break;
				case 0x06:
					rlc_mem(_hl);
					break;
				case 0x07:
					rlc(_a);
					break;
				case 0x08:
					rrc(_b);
					break;
				case 0x09:
					rrc(_c);
					break;
				case 0x0A:
					rrc(_d);
					break;
				case 0x0B:
 					rrc(_e);
					break;
				case 0x0C:
					rrc(_h);
					break;
				case 0x0D:
					rrc(_l);
					break;
				case 0x0E:
					rrc_mem(_hl);
					break;
				case 0x0F:
					rrc(_a);
					break;
				case 0x10:
					rl(_b);
					break;
				case 0x11:
					rl(_c);
					break;
				case 0x12:
					rl(_d);
					break;
				case 0x13:
					rl(_e);
					break;
				case 0x14:
					rl(_h);
					break;
				case 0x15:
					rl(_l);
					break;
				case 0x16:
					rl_mem(_hl);
					break;
				case 0x17:
					rl(_a);
					break;
				case 0x18:
					rr(_b);
					break;
				case 0x19:
					rr(_c);
					break;
				case 0x1A:
					rr(_d);
					break;
				case 0x1B:
 					rr(_e);
					break;
				case 0x1C:
					rr(_h);
					break;
				case 0x1D:
					rr(_l);
					break;
				case 0x1E:
					rr_mem(_hl);
					break;
				case 0x1F:
					rr(_a);
					break;
				case 0x20:
					sla(_b);
					break;
				case 0x21:
					sla(_c);
					break;
				case 0x22:
					sla(_d);
					break;
				case 0x23:
					sla(_e);
					break;
				case 0x24:
					sla(_h);
					break;
				case 0x25:
					sla(_l);
					break;
				case 0x26:
					sla(_hl);
					break;
				case 0x27:
					sra(_a);
					break;
				case 0x28:
					sra(_b);	
					break;
				case 0x29:
					sra(_c);
					break;
				case 0x2A:
					sra(_d);	
					break;
				case 0x2B:
					sra(_e);
					break;
				case 0x2C:
					sra(_h);
					break;
				case 0x2D:
					sra(_l);
					break;
				case 0x2E:
					sra(_hl);
					break;
				case 0x2F:
					sra(_a);
					break;
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
				case 0x36:
					swap(_hl);
					ticks+=8;
					break;
				case 0x37:
					swap(_a);
					break;
				case 0x38:
					srl(_b);
					break;
				case 0x39:
					srl(_c);
					break;
				case 0x3A:
					srl(_d);
					break;
				case 0x3B:
					srl(_e);
					break;
				case 0x3C:
					srl(_h);
					break;
				case 0x3D:
					srl(_l);
					break;
				case 0x3E:
					srl(_hl);
					break;
				case 0x3F:
					srl(_a);
					break;
				case 0x80:
					reset(0xFE, _b);
					break;
				case 0x81:
					reset(0xFE, _c);
					break;
				case 0x82:
					reset(0xFE, _d);
					break;
				case 0x83:
					reset(0xFE, _e);
					break;
				case 0x84:
					reset(0xFE, _h);
					break;
				case 0x85:
					reset(0xFE, _l);
					break;
				case 0x86:
					//TODO FIX
					reset(0xFE, mem->read(_hl));
					ticks+=4;
					break;
				case 0x87:
					reset(0xFE, _a);
					break;
				case 0x88:
					reset(0xFD, _b);
					break;
				case 0x89:
					reset(0xFD, _c);
					break;
				case 0x8A:
					reset(0xFD, _d);
					break;
				case 0x8B:
					reset(0xFD, _e);
					break;
				case 0x8C:
					reset(0xFD, _h);
					break;
				case 0x8D:
					reset(0xFD, _l);
					break;
				case 0x8E:
					//TODO FIX
					reset(0xFD, mem->read(_hl));
					ticks+=4;
					break;
				case 0x8F:
					reset(0xFD, _a);
					break;
				case 0x90:
					reset(0xFB, _b);
					break;
				case 0x91:
					reset(0xFB, _c);
					break;
				case 0x92:
					reset(0xFB, _d);
					break;
				case 0x93:
					reset(0xFB, _e);
					break;
				case 0x94:
					reset(0xFB, _h);
					break;
				case 0x95:
					reset(0xFB, _l);
					break;
				case 0x96:
					//TODO FIX
					reset(0xFB, mem->read(_hl));
					ticks+=4;
					break;
				case 0x97:
					reset(0xFB, _a);
					break;
				case 0x98:
					reset(0xF7, _b);
					break;
				case 0x99:
					reset(0xF7, _c);
					break;
				case 0x9A:
					reset(0xF7, _d);
					break;
				case 0x9B:
					reset(0xF7, _e);
					break;
				case 0x9C:
					reset(0xF7, _h);
					break;
				case 0x9D:
					reset(0xF7, _l);
					break;
				case 0x9E:
					//TODO FIX
					reset(0xF7, mem->read(_hl));
					ticks+=4;
					break;
				case 0x9F:
					reset(0xF7, _a);
					break;
				case 0xA0:
					reset(0xEF, _b);
					break;
				case 0xA1:
					reset(0xEF, _c);
					break;
				case 0xA2:
					reset(0xEF, _d);
					break;
				case 0xA3:
					reset(0xEF, _e);
					break;
				case 0xA4:
					reset(0xEF, _h);
					break;
				case 0xA5:
					reset(0xEF, _l);
					break;
				case 0xA6:
					reset(0xEF, mem->read(_hl));
					ticks+=4;
					break;
				case 0xA7:
					reset(0xEF, _a);
					break;
				case 0xA8:
					reset(0xDF, _b);
					break;
				case 0xA9:
					reset(0xDF, _c);
					break;
				case 0xAA:
					reset(0xDF, _d);
					break;
				case 0xAB:
					reset(0xDF, _e);
					break;
				case 0xAC:
					reset(0xDF, _h);
					break;
				case 0xAD:
					reset(0xDF, _l);
					break;
				case 0xAE:
					reset(0xDF, mem->read(_hl));
					ticks+=4;
					break;
				case 0xAF:
					reset(0xDF, _a);
					break;
				case 0xB0:
					reset(0xBF, _b);
					break;
				case 0xB1:
					reset(0xBF, _c);
					break;
				case 0xB2:
					reset(0xBF, _d);
					break;
				case 0xB3:
					reset(0xBF, _e);
					break;
				case 0xB4:
					reset(0xBF, _h);
					break;
				case 0xB5:
					reset(0xBF, _l);
					break;
				case 0xB6:
					reset(0xBF, mem->read(_hl));
					ticks+=4;
					break;
				case 0xB7:
					reset(0xBF, _a);
					break;
				case 0xB8:
					reset(0x7F, _b);
					break;
				case 0xB9:
					reset(0x7F, _c);
					break;
				case 0xBA:
					reset(0x7F, _d);
					break;
				case 0xBB:
					reset(0x7F, _e);
					break;
				case 0xBC:
					reset(0x7F, _h);
					break;
				case 0xBD:
					reset(0x7F, _l);
					break;
				case 0xBE:
					reset(0x7F, mem->read(_hl));
					ticks+=4;
					break;
				case 0xBF:
					reset(0x7F, _a);
					break;
				case 0xC0:
					set(0x1, _b);
					break;
				case 0xC1:
					set(0x01, _c);
					break;
				case 0xC2:
					set(0x01, _d);
					break;
				case 0xC3:
					set(0x01, _e);
					break;
				case 0xC4:
					set(0x01, _h);
					break;
				case 0xC5:
					set(0x01, _l);
					break;
				case 0xC6:
					set(0x01, mem->read(_hl));
					ticks+=4;
					break;
				case 0xC7:
					set(0x01, _a);
					break;
				case 0xC8:
					set(0x02, _b);
					break;
				case 0xC9:
					set(0x02, _c);
					break;
				case 0xCA:
					set(0x02, _d);
					break;
				case 0xCB:
					set(0x02, _e);
					break;
				case 0xCC:
					set(0x02, _h);
					break;
				case 0xCD:
					set(0x02, _l);
					break;
				case 0xCE:
					set(0x02, mem->read(_hl));
					ticks+=4;
					break;
				case 0xCF:
					set(0x02, _a);
					break;
				case 0xD0:
					set(0x04, _b);
					break;
				case 0xD1:
					set(0x04, _c);
					break;
				case 0xD2:
					set(0x04, _d);
					break;
				case 0xD3:
					set(0x04, _e);
					break;
				case 0xD4:
					set(0x04, _h);
					break;
				case 0xD5:
					set(0x04, _l);
					break;
				case 0xD6:
					set(0x04, mem->read(_hl));
					ticks+=4;
					break;
				case 0xD7:
					set(0x04, _a);
					break;
				case 0xD8:
					set(0x08, _b);
					break;
				case 0xD9:
					set(0x08, _c);
					break;
				case 0xDA:
					set(0x08, _d);
					break;
				case 0xDB:
					set(0x08, _e);
					break;
				case 0xDC:
					set(0x08, _h);
					break;
				case 0xDD:
					set(0x08, _l);
					break;
				case 0xDE:
					set(0x08, mem->read(_hl));
					ticks+=4;
					break;
				case 0xDF:
					set(0x08, _a);
					break;
				case 0xE0:
					set(0x20, _b);
					break;
				case 0xE1:
					set(0x20, _c);
					break;
				case 0xE2:
					set(0x10, _d);
					break;
				case 0xE3:
					set(0x10, _e);
					break;
				case 0xE4:
					set(0x10, _h);
					break;
				case 0xE5:
					set(0x10, _l);
					break;
				case 0xE6:
					set(0x10, mem->read(_hl));
					ticks+=4;
					break;
				case 0xE7:
					set(0x10, _a);
					break;
				case 0xE8:
					set(0x20, _b);
					break;
				case 0xE9:
					set(0x20, _c);
					break;
				case 0xEA:
					set(0x20, _d);
					break;
				case 0xEB:
					set(0x20, _e);
					break;
				case 0xEC:
					set(0x20, _h);
					break;
				case 0xED:
					set(0x20, _l);
					break;
				case 0xEE:
					set(0x20, mem->read(_hl));
					ticks+=4;
					break;
				case 0xEF:
					set(0x20, _a);
					break;
				case 0xF0:
					set(0x40, _b);
					break;
				case 0xF1:
					set(0x40, _c);
					break;
				case 0xF2:
					set(0x40, _d);
					break;
				case 0xF3:
					set(0x40, _e);
					break;
				case 0xF4:
					set(0x40, _h);
					break;
				case 0xF5:
					set(0x40, _l);
					break;
				case 0xF6:
					set(0x40, mem->read(_hl));
					ticks+=4;
					break;
				case 0xF7:
					set(0x40, _a);
					break;
				case 0xF8:
					set(0x80, _b);
					break;
				case 0xF9:
					set(0x80, _c);
					break;
				case 0xFA:
					set(0x80, _d);
					break;
				case 0xFB:
					set(0x80, _e);
					break;
				case 0xFC:
					set(0x80, _h);
					break;
				case 0xFD:
					set(0x80, _l);
					break;
				case 0xFE:
					set(0x80, mem->read(_hl));
					ticks+=4;
					break;
				case 0xFF:
					set(0x80, _a);
					break;
			}
			break;
		}
		case 0xCC:
			call(!zf, IMM_16_PC);
			break;
		case 0xCD:
			call(true, IMM_16_PC);
			break;
		case 0xCE:
			add_a_imm_mem(_pc, true);
			break;
		case 0xCF:
			rst(0x08);
			break;
		case 0xD0:
			ret(!cf);
			break;
		case 0xD1:
			pop(_d, _e);
			break;
		case 0xD2:
			cond_jmp_abs(!cf);
			break;
		case 0xD3:
			runnable = false;
			break;
		case 0xD4:
			call(!cf, IMM_16_PC);
			break;
		case 0xD5:
			push(_d, _e);
			break;
		case 0xD6:
			sub_a_imm_mem(_pc, false); 
			break;
		case 0xD7:
			rst(0x10);
			break;
		case 0xD8:
			ret(cf);
			break;
		case 0xD9:
			ret(true);
			interrupts = true;
			break;
		case 0xDA:
			cond_jmp_abs(cf);
			break;
		case 0xDB:
			runnable = false;
			break;
		case 0xDC:
			call(cf, IMM_16_PC);
			break;
		case 0xDD:
			runnable = false;
			break;
		case 0xDE:
			sub_a_imm_mem(_pc, true);
			break;
		case 0xDF:
			rst(0x18);
			break;
		case 0xE0:
			ldh_into_mem();
			break;
		case 0xE1:
			pop(_h, _l);
			break;
		//TODO LD (C), A
		case 0xE2:
			break;
		case 0xE3:
		case 0xE4:
			runnable = false;
			break;
		case 0xE5:
			push(_h, _l);
			break;
		case 0xE6:
			_a = _a & mem->read(_pc);
			zf = _a;
			hf = cf = 0;
			nf = 1;
			_pc++;
			ticks+=4;
			break;
		case 0xE7:
			rst(0x20);
			break;
		case 0xE8:
			/*reg_add_u16(_sp, mem->read(_pc));
			_pc++;
			ticks+=8;
			break;*/
		case 0xE9:
			_pc = _hl;
			break;
		case 0xEA:
			reg_store_mem(_pc);
			ticks += 12;
			break;
		case 0xEB:
		case 0xEC:
		case 0xED:
			runnable = false;
			break;
		case 0xEE:
			_a = _a ^ mem->read(_pc);
			zf = _a;
			hf = cf = nf = 0;
			_pc++;
			ticks+=4;
			break;
		case 0xEF:
			rst(0x28);
			break;
		case 0xF0:
			ldh_into_a();
			break;
		case 0xF1:
			pop_af();
			break;
		case 0xF2:
			break;
		case 0xF3:
			interrupts = false;
			break;
		case 0xF4:
			runnable = false;
			break;
		case 0xF5:
			push_af();
			break;
		case 0xF6:
			_a = _a | mem->read(_pc);
			zf = _a;
			hf = nf = cf = 0;
			_pc++;
			ticks+=4;
			break;
		case 0xF7:
			rst(0x30);
			break;
		//TODO LD hl, sp+imm_8
		case 0xF8:
			
			break;
		case 0xF9:
			_sp = _hl;
			ticks+=4;
			break;
		case 0xFA:
			reg_load_a(_pc);
			ticks+=12;
			break;
		case 0xFB:
			interrupts = true;
		case 0xFC:
		case 0xFD:
			runnable = false;
			break;
		case 0xFE:
			cmp_imm(mem->read(_pc));
			_pc++;
			ticks+=4;
			break;
		case 0xFF:
			rst(0x38);
			break;
	}	
}
void GBCPU::dbg_dump(){
	std::cerr << std::hex
	<< "a " << _a << "\n"
	<< "b " << _b << "\n"
	<< "c " << _c << "\n"
	<< "d " << _d << "\n"
	<< "e " << _e << "\n"
	<< "h " << _h << "\n"
	<< "l " << _l << "\n"
	<< "pc " << _pc << "\n"
	<< "sp " << _sp << std::endl;
}

}


