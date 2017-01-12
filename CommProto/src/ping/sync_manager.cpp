
/*
error_t SyncCallback(const comnet::Header & header, SyncPacket & packet, comnet::Comms & node)
{
		if (packet.IsRequest())
		{
				SyncPacket replyPacket;
				replyPacket.SetRequest(false);
				replyPacket.SetTime(header.source_time);
				node.Send(replyPacket, header.source_id);
		}
		else
		{
				int32_t estSendTime = (Pinger::GetTimeSinceStart() - packet.GetTime()) / 2;
				int32_t estDestTimeStamp = header.source_time + estSendTime;
				int32_t timeOff = Pinger::GetTimeSinceStart() - estDestTimeStamp;
				node.GetPingManager()->SyncTime(header.source_id, timeOff);
		}
}
*/