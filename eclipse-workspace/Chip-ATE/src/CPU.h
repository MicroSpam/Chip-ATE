#ifndef _CPU_H
#define _CPU_H

#define MEMORY 4096
#define REGISTER 16
#define STACK 16
#define KEYSTATE 16
#define DISPLAY 64*32

#include <stdint.h>

struct CPU
{
	uint8_t* memory;
	uint8_t* v;
	uint16_t* stack;
	uint8_t* keystate;
	uint16_t i;
	uint16_t pc;

	uint8_t* screenBuffer;

	int stackpointer;
	int delay_timer;
	int sound_timer;

	char running;
	char redrawFrame;
};

struct CPU* initCPU()
{
	struct CPU* m_CPU = 	(struct CPU*)malloc(sizeof(struct CPU));
	m_CPU->memory = 		(uint8_t*)malloc(sizeof(uint8_t)*MEMORY);
	m_CPU->v = 		(uint8_t*)malloc(sizeof(uint8_t)*REGISTER);
	m_CPU->stack = 			(uint16_t*)malloc(sizeof(uint16_t)*STACK);
	m_CPU->keystate = 		(uint8_t*)malloc(sizeof(uint8_t)*KEYSTATE);
	m_CPU->screenBuffer = 	(uint8_t*)calloc((DISPLAY), sizeof(uint8_t));
	//m_CPU->screenBuffer = (uint8_t*)malloc(sizeof(uint8_t)*DISPLAY);
	//memset(m_CPU->screenBuffer, 0x01, sizeof(m_CPU->screenBuffer)*DISPLAY);

	m_CPU->i = 				0x0;
	m_CPU->pc = 			0x200;
	m_CPU->stackpointer = 	0x0;
	m_CPU->delay_timer = 	0x0;
	m_CPU->sound_timer = 	0x0;
	m_CPU->running = 		1;
	m_CPU->redrawFrame = 	0x0;
	return m_CPU;
}

void destroyCPU(struct CPU* arg_CPU)
{
	free(arg_CPU);
}

void runCPU(struct CPU* arg_CPU)
{
	if (arg_CPU-> running == 1)
	{
		//fetch opcode
		uint8_t opcode = (arg_CPU->memory[arg_CPU->pc] << 8) | arg_CPU->memory[arg_CPU->pc+1];
		//decode opcode
		switch(opcode & 0xF000)
		{
		case 0x00E0 : //CLS
		{
			memset(arg_CPU->screenBuffer, 0, DISPLAY);
			arg_CPU->pc += 2;
			break;
		}
		case 0x00EE : //RET
		{
			arg_CPU->stack[0x10] = arg_CPU->pc;
			arg_CPU->stackpointer -= 1;
			break;
		}
		case 0x1000 : //JP addr
		{
			arg_CPU->pc = opcode & 0x0FFF;
			break;
		}
		case 0x2000 : //CALL addr
		{
			arg_CPU->pc += 2;
			arg_CPU->stack[0x10] = arg_CPU->pc;
			arg_CPU->pc = opcode & 0x0FFF;
			break;
		}
		case 0x3000 : //SE Vx, byte
		{
			if (arg_CPU->v[opcode & 0x0F00] == (opcode & 0x00FF))
			{
				arg_CPU->pc += 2;
			}
			arg_CPU->pc += 2;
			break;
		}
		case 0x4000 : //SNE Vx, byte
		{
			if (arg_CPU->v[opcode & 0x0F00] != (opcode & 0x00FF))
			{
				arg_CPU->pc += 2;
			}
			arg_CPU->pc += 2;
			break;
		}
		case 0x5000 : //SE Vx, Vy
		{
			if (arg_CPU->v[opcode & 0x0F00] == arg_CPU->v[opcode & 0x00F0])
			{
				arg_CPU->pc += 2;
			}
			arg_CPU->pc += 2;
			break;
		}
		case 0x6000 : //LD Vx, byte
		{
			arg_CPU->v[opcode & 0x0F00] = (opcode & 0x00FF);
			arg_CPU->pc += 2;
			break;
		}
		case 0x7000 : //ADD Vx, byte
		{
			uint8_t byte = (uint8_t)(opcode & 0x00FF);
			arg_CPU->v[opcode & 0x0F00] += byte;
			arg_CPU->pc += 2;
			break;
		}
		case 0x8000 :
		{
			switch(opcode & 0x000F)
			{
			case 0x0000 : //LD Vx, Vy
			{
				arg_CPU->v[opcode & 0x0F00] = arg_CPU->v[opcode & 0x00F0];
				arg_CPU->pc += 2;
				break;
			}
			case 0x0001 : //OR Vx, Vy
			{
				arg_CPU->v[opcode & 0x0F00] |= arg_CPU->v[opcode & 0x00F0];
				arg_CPU->pc += 2;
				break;
			}
			case 0x0002 : //AND Vx, Vy
			{
				arg_CPU->v[opcode & 0x0F00] &= arg_CPU->v[opcode & 0x00F0];
				arg_CPU->pc += 2;
				break;
			}
			}
			break;
		}
		default:
			printf("%x is not a valid opcode!\n", opcode);
			arg_CPU->running = 0;
			break;
		}
		//execute opcode

	}
}

void getOpcode(uint8_t opcode)
{

}

//nibbles are read left to right

uint16_t opcodeTable[35] =
{
		0x0000, //call RCA 1802
		0x00E0, //clear screen buffer
		0x00EE, //return from subroutine
		0x1000, //jump to 12-bit address
		0x2000, //call subroutine at the address of last 3 nibbles
		0x3000, //if Vx == NN
		0x4000, //if != NN
		0x5000, //if Vx == Vy
		0x6000, //Vx = NN
		0x7000, //Vx += NN (without carry)
		0x8000, //Vx = Vy
		0x8001, //Vx = Vx|Vy
		0x8002, //Vx = Vx&Vy
		0x8003, //Vx = Vx^Vy
		0x8004, //Vx += VY (with carry)
		0x8005, //Vx -= VY (with borrow)
		0x8006, //Vx = Vy = (Vy >> 1)
		0x8007, //Vx = Vy -Vx
		0x800E, //Vx = Vy = (Vy << 1)
		0x9000, //if(Vx!=Vy)
		0xA000, //I = NNN
		0xB000, //PC = V0 + NNN
		0xC000, //random number generator
		0xD000, //draw sprite at coordinate VX,VY

};

#endif
