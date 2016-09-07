#include <CommProto\network\Parser.h>


namespace Comnet{
namespace Network{
	Parser::Parser(){
		strcpy(terminal_sequence, "*&*");
		parserPosition = 0;
		lastReceivedLength = 0;

	}

	void Parser::parseSend(uint8_t* txData, uint32_t &txLength, uint8_t *parsedData){
		memset(parsedData, 0, sizeof(parsedData));
		memcpy(parsedData, terminal_sequence, TERMINAL_SEQUENCE_SIZE);
		memcpy((parsedData + TERMINAL_SEQUENCE_SIZE), txData, txLength);
		memcpy((parsedData + TERMINAL_SEQUENCE_SIZE + txLength), terminal_sequence, TERMINAL_SEQUENCE_SIZE);
		txLength += (2 * TERMINAL_SEQUENCE_SIZE);
	}


	bool Parser::parseReceive(uint8_t* rxData, uint32_t &rxLength, uint8_t *parsedData){
		//this could be better
		if (parserPosition == 0 || parserPosition >= lastReceivedLength - 1){
			parserPosition = 0;
			lastReceivedLength = rxLength;
		}


		bool parsed = false;
		uint32_t messageLength = 0;
		while (!parsed && lastReceivedLength > 0){

			//check for sequence
			//printf("ParserPosition: %d\n", parserPosition);
			if ((char)parsedData[parserPosition] == '*' && (char)parsedData[parserPosition + 1] == '&' && (char)parsedData[parserPosition + 2] == '*'){
				parserPosition += TERMINAL_SEQUENCE_SIZE;
				//printf("ParserPosition: %d\n", parserPosition);
				bool done = false;
				while (!done && messageLength < 516){
					rxData[messageLength++] = parsedData[parserPosition++];
					char a = parsedData[parserPosition];
					char b = parsedData[parserPosition + 1];
					char c = parsedData[parserPosition + 2];
					if (a == '*' && b == '&' && c == '*')done = true;

				}
				//printf("%c\n", (char)serialBufferRecv[parserPosition + 1]);
				//printf("%c\n", (char)serialBufferRecv[parserPosition + 2]);
				//printf("%c\n", (char)serialBufferRecv[parserPosition + 3]);
				//printf("Message Length %d\n", messageLength);
				rxLength = messageLength;
				parserPosition += TERMINAL_SEQUENCE_SIZE;
				//printf("ParserPosition: %d\n", parserPosition);
				parsed = true;
			}
			else
			{
				parserPosition++;
				if (parserPosition > lastReceivedLength)parsed = true;
			}
		}

		return messageLength > 0;
	}
}
}