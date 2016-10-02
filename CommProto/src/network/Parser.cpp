#include <CommProto/network/parser.h>

#include <string.h>


namespace comnet{
namespace network{


	Parser::Parser() {
		strcpy(terminal_sequence, "*&*");
		parserPosition = 0;
		lastReceivedLength = 0;

	}

	void Parser::parseSend(uint8_t* txData, uint32_t &txLength, uint8_t *parsedData) {
		memset(parsedData, 0, sizeof(parsedData));//zero out buffer
		memcpy(parsedData, terminal_sequence, TERMINAL_SEQUENCE_SIZE);//add terminal to front
		memcpy((parsedData + TERMINAL_SEQUENCE_SIZE), txData, txLength);//add tx data
		memcpy((parsedData + TERMINAL_SEQUENCE_SIZE + txLength), terminal_sequence, TERMINAL_SEQUENCE_SIZE);//add temrinal to end
		txLength += (2 * TERMINAL_SEQUENCE_SIZE);//adjust length
	}


	bool Parser::parseReceive(uint8_t* rxData, uint32_t &rxLength, uint8_t *parsedData) {
		
		//check if parse has completed if so rest position and get new last length
		if (parserPosition == 0 || parserPosition >= lastReceivedLength - 1) {
			parserPosition = 0;
			lastReceivedLength = rxLength;
		}


		bool parsed = false;
		uint32_t messageLength = 0;
		//parse message and calcualte new length of parsed data
		while (!parsed && lastReceivedLength > 0) {

			//check for start sequence			
			if ((char)parsedData[parserPosition] == terminal_sequence[0] && 
				(char)parsedData[parserPosition + 1] == terminal_sequence[1] &&
				(char)parsedData[parserPosition + 2] == terminal_sequence[2]) {
				parserPosition += TERMINAL_SEQUENCE_SIZE;//adjust parse position by teminal size
				
				bool done = false;//loop until terminal sequence is found
				while (!done && parserPosition <= lastReceivedLength) {
					rxData[messageLength++] = parsedData[parserPosition++];

					//check if terminal sequence is next
					if ((char)parsedData[parserPosition] == terminal_sequence[0] &&
						(char)parsedData[parserPosition + 1] == terminal_sequence[1] &&
						(char)parsedData[parserPosition + 2] == terminal_sequence[2])done = true;

				}
				rxLength = messageLength;//set new length
				parserPosition += TERMINAL_SEQUENCE_SIZE;//adjust parser position for end terminal length				
				parsed = true;
			}
			else//no terminal found increment through buffer
			{
				parserPosition++;
				if (parserPosition > lastReceivedLength)parsed = true;
			}
		}
		return messageLength > 0;//return false if parser found no message
	}
}//end NETWORK
}//end COMNET
