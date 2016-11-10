//CommProtocol
#include <CommProto/comms.h>
#include <CommProto/callback.h>

//boost
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/foreach.hpp>

//std
#include <list>
#include <iostream>
#include <string>


//this is  simple round robin test starts a node 1 and ends at node x
//will add more tests
//used xml to parse data for xbee tests
//xml is a pain for udp, but will work better for xbees
//need to add variable packets sizes
//callback is currently constant will need a string to callback function
//call back will have to be pre defined in this file
//used iterators just for Mario.
//I did change to tab = space '4' so will see 


// Ping testing.
class Ping : INHERITS_ABSPACKET {
public:
    Ping() : CHAIN_ABSPACKET(Ping) { }
    Ping(std::string cc)
        : CHAIN_ABSPACKET(Ping)
        , cat(cc) { }

    void Pack(REF_OBJECTSTREAM obj) {
        obj << cat;  // add cat
    }

    void Unpack(REF_OBJECTSTREAM obj) {
        obj >> cat;
    }

    ABSPACKET* Create() {
        return new Ping();
    }

    const std::string& GetCat() const { return cat; }
private:

    std::string cat;
};


int maxNode = 1;
int minNode = 1;

// Callback function that we will be using to link to Ping packet.
error_t PingCallback(const comnet::Header& header, const Ping& packet, comnet::Comms& node) {

    Ping bingBong("bing bong");

    int destID = header.source_id + 1;
    if (destID > maxNode) destID = minNode;

    if (header.source_id != maxNode)
    {       
        node.Send(bingBong, destID);
    }
    else
    {
        printf("************************** round robin complete ***************************\n");
    }

    return comnet::CALLBACK_SUCCESS | comnet::CALLBACK_DESTROY_PACKET;
}




struct connection
{
    //connection
    int connectionNumber;
    int outPort;
    std::string outIP;
};

struct nodeData
{
    //node
    int nodeNumber;

    //init
    transport_protocol_t linkType;
    std::string inPort;
    std::string inIP;

    //list of connections
    std::list<connection> addressList;

    //call back name link
    std::string callBack;
};


transport_protocol_t stringToEnum(std::string value)
{
    transport_protocol_t enum1;

    if (value == "UDP_LINK")
    {
        return UDP_LINK;
    }
    else if (value == "ZIGBEE_LINK")
    {
        return ZIGBEE_LINK;
    }
    else if (value == "SERIAL_LINK")
    {
        return SERIAL_LINK;
    }
    return UDP_LINK;
}


typedef std::list<nodeData> nodeDataList;


nodeDataList readXML(std::istream & is)
{
    using boost::property_tree::ptree;

    ptree pt;
    read_xml(is, pt);

    nodeDataList dataList;

    BOOST_FOREACH(ptree::value_type const& v, pt.get_child("CommProtocol"))
    {
        nodeData tempNodeData;

        //this loop need a list of test
        if (v.first == "node")
        {
            //get this nodes data
            tempNodeData.nodeNumber = v.second.get<int>("id");
            tempNodeData.linkType = stringToEnum(v.second.get<std::string>("link_type"));
            tempNodeData.inPort = v.second.get<std::string>("in_port");
            tempNodeData.inIP = v.second.get <std::string>("in_ip");            

            //fix to class
            tempNodeData.callBack = v.second.get<std::string>("call_back");

            //get all node Sconnectinos
            BOOST_FOREACH(ptree::value_type const& v2, v.second.get_child("connections"))
            {
                if (v2.first == "out_node")
                {
                    connection tempConnection;
                    tempConnection.connectionNumber = v2.second.get<int>("id");
                    tempConnection.outPort = v2.second.get<int>("out_port");
                    tempConnection.outIP = v2.second.get<std::string>("out_ip");

                    tempNodeData.addressList.push_back(tempConnection);                    
                }
            }
            dataList.push_back(tempNodeData);            
        }
    }
    return dataList;
}



int main()
{
    std::ifstream ifs("config.xml");
    nodeDataList simulationDataList =  readXML(ifs);
    ifs.close();

    std::list<comnet::Comms*> simulationNodeList;

    //for mario
    for (std::list<nodeData>::iterator nodeIt = simulationDataList.begin(); nodeIt != simulationDataList.end(); nodeIt++)
    {
        //cerate new node
        comnet::Comms *tempComms = new comnet::Comms((*nodeIt).nodeNumber);
        tempComms->LinkCallback(new Ping(), new comnet::Callback((comnet::callback_t)PingCallback));

        tempComms->InitConnection((*nodeIt).linkType, (*nodeIt).inPort.c_str(), (*nodeIt).inIP.c_str());
        for (std::list<connection>::iterator connectionIt = (*nodeIt).addressList.begin(); connectionIt != (*nodeIt).addressList.end(); connectionIt++)
        {
            tempComms->AddAddress((*connectionIt).connectionNumber, (*connectionIt).outIP.c_str(), (*connectionIt).outPort);
        }

        tempComms->LoadKey("NGCP project 2016");
        simulationNodeList.push_back(tempComms);
    }


    

    for (std::list<comnet::Comms*>::iterator commsIt = simulationNodeList.begin(); commsIt != simulationNodeList.end(); commsIt++)
    {
        (*commsIt)->Run();
    }
    

    comnet::Comms *tempComms = simulationNodeList.front();


    Ping bing("test ping");
    tempComms->Send(bing, 2);


    //for internal testing won't work if using other computer
    minNode = simulationNodeList.front()->GetNodeId();
    maxNode = simulationNodeList.back()->GetNodeId();

    while (true) {
        std::cout << "Sleeping..." << std::endl;
        //comm1 will be sending the packet.

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}