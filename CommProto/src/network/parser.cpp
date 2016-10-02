#include <CommProto/network/parser.h>

#include <string.h>


namespace comnet{
namespace network{


	Parser::Parser() {
		strcpy(terminal_sequence, "*&*");
		parser_position = 0;
		last_received_length = 0;

	}

	void Parser::ParseSend(uint8_t* tx_data, uint32_t &tx_length, uint8_t *parsed_data) {
		memset(parsed_data, 0, sizeof(parsed_data));//zero out buffer
		memcpy(parsed_data, terminal_sequence, TERMINAL_SEQUENCE_SIZE);//add terminal to Front
		memcpy((parsed_data + TERMINAL_SEQUENCE_SIZE), tx_data, tx_length);//add tx data
		memcpy((parsed_data + TERMINAL_SEQUENCE_SIZE + tx_length), terminal_sequence, TERMINAL_SEQUENCE_SIZE);//add temrinal to End
		tx_length += (2 * TERMINAL_SEQUENCE_SIZE);//adjust length
	}


	bool Parser::ParseReceive(uint8_t* rx_data, uint32_t &rx_length, uint8_t *parsed_data) {
		
		//check if parse has completed if so rest position and get new last length
		if (parser_position == 0 || parser_position >= last_received_length - 1) {
			parser_position = 0;
			last_received_length = rx_length;
		}


		bool parsed = false;
		uint32_t msg_len = 0;
		//parse message and calcualte new length of parsed data
		while (!parsed && last_received_length > 0) {

			//check for start sequence			
			if ((char)parsed_data[parser_position] == terminal_sequence[0] && 
				(char)parsed_data[parser_position + 1] == terminal_sequence[1] &&
				(char)parsed_data[parser_position + 2] == terminal_sequence[2]) {
				parser_position += TERMINAL_SEQUENCE_SIZE;//adjust parse position by teminal size
				
				bool done = false;//loop until terminal sequence is found
				while (!done && parser_position <= last_received_length) {
					rx_data[msg_len++] = parsed_data[parser_position++];

					//check if terminal sequence is next
					if ((char)parsed_data[parser_position] == terminal_sequence[0] &&
						(char)parsed_data[parser_position + 1] == terminal_sequence[1] &&
						(char)parsed_data[parser_position + 2] == terminal_sequence[2])done = true;

				}
				rx_length = msg_len;//set new length
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
