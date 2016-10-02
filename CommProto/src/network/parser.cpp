#include <CommProto/network/parser.h>

#include <string.h>


namespace comnet{
namespace network{


	Parser::Parser() {
		strcpy(terminal_sequence, "*&*");
		parser_position = 0;
		last_received_length = 0;

	}

	void Parser::ParseSend(uint8_t* txData, uint32_t &txLength, uint8_t *parsedData) {
		memset(parsedData, 0, sizeof(parsedData));//zero out buffer
		memcpy(parsedData, terminal_sequence, TERMINAL_SEQUENCE_SIZE);//add terminal to Front
		memcpy((parsedData + TERMINAL_SEQUENCE_SIZE), txData, txLength);//add tx data
		memcpy((parsedData + TERMINAL_SEQUENCE_SIZE + txLength), terminal_sequence, TERMINAL_SEQUENCE_SIZE);//add temrinal to End
		txLength += (2 * TERMINAL_SEQUENCE_SIZE);//adjust length
	}


	bool Parser::ParseReceive(uint8_t* rxData, uint32_t &rxLength, uint8_t *parsedData) {
		
		//check if parse has completed if so rest position and get new last length
		if (parser_position == 0 || parser_position >= last_received_length - 1) {
			parser_position = 0;
			last_received_length = rxLength;
		}


		bool parsed = false;
		uint32_t msg_len = 0;
		//parse message and calcualte new length of parsed data
		while (!parsed && last_received_length > 0) {

			//check for start sequence			
			if ((char)parsedData[parser_position] == terminal_sequence[0] && 
				(char)parsedData[parser_position + 1] == terminal_sequence[1] &&
				(char)parsedData[parser_position + 2] == terminal_sequence[2]) {
				parser_position += TERMINAL_SEQUENCE_SIZE;//adjust parse position by teminal size
				
				bool done = false;//loop until terminal sequence is found
				while (!done && parser_position <= last_received_length) {
					rxData[msg_len++] = parsedData[parser_position++];

					//check if terminal sequence is next
					if ((char)parsedData[parser_position] == terminal_sequence[0] &&
						(char)parsedData[parser_position + 1] == terminal_sequence[1] &&
						(char)parsedData[parser_position + 2] == terminal_sequence[2])done = true;

				}
				rxLength = msg_len;//set new length
				parser_position += TERMINAL_SEQUENCE_SIZE;//adjust parser position for End terminal length				
				parsed = true;
			}
			else//no terminal found increment through buffer
			{
				parser_position++;
				if (parser_position > last_received_length)parsed = true;
			}
		}
		return msg_len > 0;//return false if parser found no message
	}
}//End NETWORK
}//End COMNET
