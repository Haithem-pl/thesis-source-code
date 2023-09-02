#include <fstream>
#include <iostream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/animation-interface.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/olsr-module.h"
#include "ns3/csma-module.h"
#include "ns3/ssid.h"
#include "ns3/on-off-helper.h"
#include "ns3/random-variable-stream.h"
#include "ns3/error-model.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Haithem");

int main (int argc, char *argv[])
{

  double duration = 10.0; //seconds
  std::string tr_name ("Haithem");
  int tcpSegmentSize = 1000;
  std::string phyMode("DsssRate1Mbps");
 std::string recovery = "ns3::TcpClassicRecovery";
 std::string transport_prot;
 
 double onTimeMin = 5.0;
 double onTimeMax = 10.0;
 double offTimeMin = 0.0;
 double offTimeMax = 10.0;
 
 Ptr<UniformRandomVariable> onTimeRandom = CreateObject<UniformRandomVariable>();
 onTimeRandom->SetAttribute("Min",DoubleValue(onTimeMin));
 onTimeRandom->SetAttribute("Max",DoubleValue(onTimeMax));
 
 Ptr<UniformRandomVariable> offTimeRandom = CreateObject<UniformRandomVariable>();
 offTimeRandom->SetAttribute("Min",DoubleValue(offTimeMin));
 offTimeRandom->SetAttribute("Max",DoubleValue(offTimeMax));
 
 
  Ptr<UniformRandomVariable> onTimeRandom1 = CreateObject<UniformRandomVariable>();
 onTimeRandom1->SetAttribute("Min",DoubleValue(onTimeMin));
 onTimeRandom1->SetAttribute("Max",DoubleValue(onTimeMax));
 
 Ptr<UniformRandomVariable> offTimeRandom1 = CreateObject<UniformRandomVariable>();
 offTimeRandom1->SetAttribute("Min",DoubleValue(offTimeMin));
 offTimeRandom1->SetAttribute("Max",DoubleValue(offTimeMax));
 
 
  Ptr<UniformRandomVariable> onTimeRandom2 = CreateObject<UniformRandomVariable>();
 onTimeRandom2->SetAttribute("Min",DoubleValue(onTimeMin));
 onTimeRandom2->SetAttribute("Max",DoubleValue(onTimeMax));
 
 Ptr<UniformRandomVariable> offTimeRandom2 = CreateObject<UniformRandomVariable>();
 offTimeRandom2->SetAttribute("Min",DoubleValue(offTimeMin));
 offTimeRandom2->SetAttribute("Max",DoubleValue(offTimeMax));
int tp;

std::cout<<"Enter 1 for TcpNewReno, 2 for TcpCubic and 3 for TcpBbr"<<std::endl;
std::cin>>tp;

if(tp==1) 
{
transport_prot = "TcpNewReno";
} 
else if(tp==2)
{
transport_prot = "TcpCubic";
}
else if(tp==3)
{
transport_prot = "TcpBbr";
}
else
{
std::cout<<"Please select correct TCP variants by adding tp=1, tp=2 or tp=3"<<std::endl;
}

  transport_prot = std::string ("ns3::") + transport_prot;
  Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (transport_prot));


Config::SetDefault("ns3::TcpL4Protocol::RecoveryType",
                       TypeIdValue(TypeId::LookupByName(recovery)));


  NS_LOG_INFO ("Create nodes."); 
  
  std::cout<<"Select duration of the simulation in seconds"<<std::endl;
  std::cin>>duration;

  
  NodeContainer c;   
  c.Create (8);   //Create Total Nodes
  
  
  NodeContainer c0r0 = NodeContainer (c.Get (0), c.Get (3));
  NodeContainer c1r0 = NodeContainer (c.Get (1), c.Get (3));
  NodeContainer c2r0 = NodeContainer (c.Get (2), c.Get (3));
  NodeContainer r0r1 = NodeContainer (c.Get (3), c.Get (4));
  NodeContainer r1c3 = NodeContainer (c.Get (4), c.Get (5));
  NodeContainer r1c4 = NodeContainer (c.Get (4), c.Get (6));
  NodeContainer r1c5 = NodeContainer (c.Get (4), c.Get (7));


  InternetStackHelper internet; 
  internet.Install (c);

  Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(tcpSegmentSize));
  Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue("2Mbps"));
  
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  p2p.SetChannelAttribute ("Delay", StringValue ("1ms")); 
  p2p.SetQueue ("ns3::DropTailQueue", "MaxSize", QueueSizeValue(QueueSize("1000p")));
  NetDeviceContainer d0 = p2p.Install (c0r0);
  NetDeviceContainer d1 = p2p.Install (c1r0); 
  NetDeviceContainer d2 = p2p.Install (c2r0);
  NetDeviceContainer d3 = p2p.Install (r0r1);
  NetDeviceContainer d4 = p2p.Install (r1c3); 
  NetDeviceContainer d5 = p2p.Install (r1c4); 
  NetDeviceContainer d6 = p2p.Install (r1c5); 
  
// Add the error model to the desired device (d1 in this case)
  //Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
  //em->SetAttribute("ErrorRate", DoubleValue(0.0001));//0.01 = 1%
  //d3.Get(0)->SetAttribute("ReceiveErrorModel", PointerValue(em));
  //d3.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));

  
   // Mobilty and location of the devices setting here
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();

      	positionAlloc->Add (Vector ( 0.0, 0, 0));
    	positionAlloc->Add (Vector (0.0, 10, 0));
    	positionAlloc->Add (Vector (0.0, 20, 0));
    	positionAlloc->Add (Vector (10.0, 10, 0));
    	positionAlloc->Add (Vector (30.0, 10, 0));
    	positionAlloc->Add (Vector (40.0, 0, 0));
    	positionAlloc->Add (Vector (40.0, 10, 0));
    	positionAlloc->Add (Vector (40.0, 20, 0));
    	
    	
  mobility.SetPositionAllocator (positionAlloc);     

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (c);


   NS_LOG_INFO ("assigning ip address");


//   IPv4 addresses.
  //NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4;
  
   ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaced0 = ipv4.Assign (d0);

   ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaced1 = ipv4.Assign (d1);

   ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaced2 = ipv4.Assign (d2);

   ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaced3 = ipv4.Assign (d3);
 
   ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaced4 = ipv4.Assign (d4);
  
   ipv4.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaced5 = ipv4.Assign (d5);
  
   ipv4.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer ifaced6 = ipv4.Assign (d6);


 Ipv4GlobalRoutingHelper::PopulateRoutingTables ();  //Enable Default Routing from source to Destination

 
   uint16_t port = 50000;    //port number
   uint16_t port1 = 50001; 
   uint16_t port2 = 50002; 
   
   OnOffHelper onoff("ns3::TcpSocketFactory",InetSocketAddress(ifaced4.GetAddress(1), port)); //Reciever Nodes  (First Node)

  ApplicationContainer sourceApps;   //application for sender
  ApplicationContainer sinkApps;    //application for receiver
  
    OnOffHelper source  ("ns3::TcpSocketFactory",InetSocketAddress (ifaced4.GetAddress (1), port)); //Here is the IP address of receiver 
    OnOffHelper source1  ("ns3::TcpSocketFactory",InetSocketAddress (ifaced5.GetAddress (1), port1));  
    OnOffHelper source2  ("ns3::TcpSocketFactory",InetSocketAddress (ifaced6.GetAddress (1), port2));  
    
    source.SetAttribute ("PacketSize", UintegerValue (tcpSegmentSize));
    
    sourceApps.Add (source.Install (c.Get (0))); //sender node number
    sourceApps.Add (source.Install (c.Get (1))); 
    sourceApps.Add (source.Install (c.Get (2))); 

    sourceApps.Add (source1.Install (c.Get (0)));
    sourceApps.Add (source1.Install (c.Get (1))); 
    sourceApps.Add (source1.Install (c.Get (2))); 

    sourceApps.Add (source2.Install (c.Get (0)));
    sourceApps.Add (source2.Install (c.Get (1))); 
    sourceApps.Add (source2.Install (c.Get (2))); 

    PacketSinkHelper sink ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
    PacketSinkHelper sink1 ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
    PacketSinkHelper sink2 ("ns3::TcpSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port2));
  sinkApps.Add (sink.Install (c .Get (5)));  //receiver node number 
  sinkApps.Add (sink1.Install (c .Get (6))); 
  sinkApps.Add (sink2.Install (c .Get (7)));
   
    source.SetAttribute ("OnTime", PointerValue (onTimeRandom));
    source.SetAttribute ("OffTime", PointerValue (offTimeRandom));      
    source1.SetAttribute ("OnTime", PointerValue (onTimeRandom1));
    source1.SetAttribute ("OffTime", PointerValue (offTimeRandom1));
    source2.SetAttribute ("OnTime", PointerValue (onTimeRandom2));
    source2.SetAttribute ("OffTime", PointerValue (offTimeRandom2));
      
  sourceApps.Start (Seconds (onTimeRandom->GetValue()));  // Start at a random on time
  
   FlowMonitorHelper flowmon;
   Ptr<FlowMonitor> monitor = flowmon.InstallAll();

   Simulator::Stop (Seconds (duration));
 
  AnimationInterface anim ("tcpNewRenoCubicBbr.xml");
  anim.SetMaxPktsPerTraceFile(99999999999999);
  Simulator::Run ();
  Ptr < Ipv4FlowClassifier > classifier = DynamicCast < Ipv4FlowClassifier >(flowmon.GetClassifier());
	std::map < FlowId, FlowMonitor::FlowStats > stats = monitor->GetFlowStats();

	double Delaysum = 0;
        //double jitterSum = 0;
	uint64_t txPacketsum = 0;
	uint64_t rxPacketsum = 0;
	uint32_t txPacket = 0;
	uint32_t rxPacket = 0;
        uint32_t PacketLoss = 0;
        uint64_t txBytessum = 0; 
	uint64_t rxBytessum = 0;
       double delay;
       //double jitter;
       	double throughput = 0;
	for (std::map < FlowId, FlowMonitor::FlowStats > ::const_iterator iter = stats.begin(); iter != stats.end(); ++iter) {
		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);
                NS_LOG_UNCOND("*****************************************");
		NS_LOG_UNCOND("Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << " Dst Addr " << t.destinationAddress);
                 txPacket = iter->second.txPackets;
                  rxPacket = iter->second.rxPackets;
                  PacketLoss = txPacket - rxPacket;
                  delay = iter->second.delaySum.GetMilliSeconds();
                  //jitter = iter->second.jitterSum.GetMilliSeconds();
           std::cout << "  Tx Packets: " << iter->second.txPackets << "\n";
          std::cout << "  Rx Packets: " << iter->second.rxPackets << "\n";
          std::cout << "  Packet Loss: " << PacketLoss << "\n";
  	  std::cout << "  Tx Bytes:   " << iter->second.txBytes << "\n";
          std::cout << "  Rx Bytes:   " << iter->second.rxBytes << "\n";
          std::cout << "  Throughput: " << iter->second.rxBytes * 8.0 / 9.0 / 1000 / 1000  << " Mbps\n";
         NS_LOG_UNCOND("  Mean Delay: " << delay / txPacket << " ms");
         //NS_LOG_UNCOND("  Per Node Jitter: " << jitter / txPacket << " ms");
         std::cout << "   PDR for current flow ID : " << ((rxPacket *100) / txPacket) << "%" << "\n";
                                      
		txPacketsum += iter->second.txPackets;
		rxPacketsum += iter->second.rxPackets;
		txBytessum += iter->second.txBytes;
		rxBytessum += iter->second.rxBytes;
		Delaysum += iter->second.delaySum.GetMilliSeconds();
                //jitterSum += iter->second.jitterSum.GetMilliSeconds();
        }          
            if(tp==1){
             NS_LOG_UNCOND("***********TcpNewReno  Results*************");
            }
           else if(tp==2){
             NS_LOG_UNCOND("***********TcpCubic  Results*************");
            }
          else{
             NS_LOG_UNCOND("***********TcpBbr  Results*************");
            }

	throughput = rxBytessum * 8 / (duration * 1000000.0); //Mbit/s  
	NS_LOG_UNCOND("Sent Packets = " << txPacketsum);
	NS_LOG_UNCOND("Received Packets = " << rxPacketsum);
        NS_LOG_UNCOND("Total Packet Loss = " << (txPacketsum-rxPacketsum));
        NS_LOG_UNCOND("Total Byte Sent = "<<txBytessum);
        NS_LOG_UNCOND("Total Byte Received = "<<rxBytessum);
      	NS_LOG_UNCOND("Mean Delay: " << Delaysum / txPacketsum << " ms");
        //NS_LOG_UNCOND("Jitter: " << jitterSum / txPacketsum << " ms");
	std::cout << "Average Throughput = "<<throughput << " Mbit/s" << std::endl;
        std::cout << "Packets Delivery Ratio: " << ((rxPacketsum *100) / txPacketsum) << "%" << "\n";
        monitor->SerializeToXmlFile("Haithem.flowmon", true, true);
 
  Simulator::Destroy ();
}