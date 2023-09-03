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
#include "ns3/csma-module.h"
#include "ns3/ssid.h"
#include "ns3/quic-module.h"
#include "ns3/applications-module.h"
#include "ns3/config-store-module.h"
#include "ns3/on-off-helper.h"
#include "ns3/random-variable-stream.h"
#include "ns3/error-model.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Haithem");

int main (int argc, char *argv[])
{

  double duration = 10.0; //seconds
  std::string tr_name ("Haithem2");

  int SegmentSize = 1000;
  int tcpFlow = 1;


  CommandLine cmd (__FILE__);

cmd.AddValue ("tcpFlow", "Select TCP Flows as 1 or 2", tcpFlow);
cmd.AddValue ("duration", "Simulation Time", duration);

// Configure On/Off Application Parameters
    double onTimeMin = 5.0;  // Minimum on time in seconds
    double onTimeMax = 10.0; // Maximum on time in seconds
    double offTimeMin = 0.0; // Minimum off time in seconds
    double offTimeMax = 10.0;// Maximum off time in seconds

//1 Random Variables for On/Off times

    Ptr<UniformRandomVariable> onTimeRandomSource = CreateObject<UniformRandomVariable>();
    onTimeRandomSource->SetAttribute("Min", DoubleValue(onTimeMin));
    onTimeRandomSource->SetAttribute("Max", DoubleValue(onTimeMax));

    Ptr<UniformRandomVariable> offTimeRandomSource = CreateObject<UniformRandomVariable>();
    offTimeRandomSource->SetAttribute("Min", DoubleValue(offTimeMin));
    offTimeRandomSource->SetAttribute("Max", DoubleValue(offTimeMax));
//2
    Ptr<UniformRandomVariable> onTimeRandomSource1 = CreateObject<UniformRandomVariable>();
    onTimeRandomSource1->SetAttribute("Min", DoubleValue(onTimeMin));
    onTimeRandomSource1->SetAttribute("Max", DoubleValue(onTimeMax));

    Ptr<UniformRandomVariable> offTimeRandomSource1 = CreateObject<UniformRandomVariable>();
    offTimeRandomSource1->SetAttribute("Min", DoubleValue(offTimeMin));
    offTimeRandomSource1->SetAttribute("Max", DoubleValue(offTimeMax));
//3
    Ptr<UniformRandomVariable> onTimeRandomSource2 = CreateObject<UniformRandomVariable>();
    onTimeRandomSource2->SetAttribute("Min", DoubleValue(onTimeMin));
    onTimeRandomSource2->SetAttribute("Max", DoubleValue(onTimeMax));

    Ptr<UniformRandomVariable> offTimeRandomSource2 = CreateObject<UniformRandomVariable>();
    offTimeRandomSource2->SetAttribute("Min", DoubleValue(offTimeMin));
    offTimeRandomSource2->SetAttribute("Max", DoubleValue(offTimeMax));

  cmd.Parse (argc, argv);

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


  QuicHelper stack;
  stack.InstallQuic (c);
  // Configure the data rate and packet size globally
    Config::SetDefault("ns3::OnOffApplication::PacketSize", UintegerValue(SegmentSize)); // Set packet size
    Config::SetDefault("ns3::OnOffApplication::DataRate", StringValue("2Mbps"));            // Set data rate


  PointToPointHelper p2p;
  p2p.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));   //Link capacity
  p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));  //Delay settig for the link
  p2p.SetQueue ("ns3::DropTailQueue", "MaxSize", QueueSizeValue(QueueSize("1000p")));
  NetDeviceContainer d0 = p2p.Install (c0r0);
  NetDeviceContainer d1 = p2p.Install (c1r0);
  NetDeviceContainer d2 = p2p.Install (c2r0); 
  NetDeviceContainer d3 = p2p.Install (r0r1);
  NetDeviceContainer d4 = p2p.Install (r1c3); //ifaced4    0th address 
  NetDeviceContainer d5 = p2p.Install (r1c4); 
  NetDeviceContainer d6 = p2p.Install (r1c5); 

// Add the error model to the desired device (d1 in this case)
  //Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
  //em->SetAttribute("ErrorRate", DoubleValue(0.01));//0.01 = 1%
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
 
   OnOffHelper onoff("ns3::QuicSocketFactory",InetSocketAddress(ifaced4.GetAddress(1), port)); //Reciever Nodes  (First Node)



  ApplicationContainer sourceApps;   //application for sender
  ApplicationContainer sinkApps;    //application for receiver

    OnOffHelper source  ("ns3::QuicSocketFactory",InetSocketAddress (ifaced4.GetAddress (1), port)); //Here is the IP address of receiver 
    OnOffHelper source1  ("ns3::QuicSocketFactory",InetSocketAddress (ifaced5.GetAddress (1), port1));
    OnOffHelper source2  ("ns3::QuicSocketFactory",InetSocketAddress (ifaced6.GetAddress (1), port2));  

    source.SetAttribute ("PacketSize", UintegerValue (SegmentSize));

    sourceApps.Add (source.Install (c.Get (0))); //sender node number
    sourceApps.Add (source.Install (c.Get (1))); 
    sourceApps.Add (source.Install (c.Get (2)));

    sourceApps.Add (source1.Install (c.Get (0)));
    sourceApps.Add (source1.Install (c.Get (1)));
    sourceApps.Add (source1.Install (c.Get (2)));

    sourceApps.Add (source2.Install (c.Get (0)));
    sourceApps.Add (source2.Install (c.Get (1)));
    sourceApps.Add (source2.Install (c.Get (2)));

    PacketSinkHelper sink ("ns3::QuicSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port));
    PacketSinkHelper sink1 ("ns3::QuicSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port1));
    PacketSinkHelper sink2 ("ns3::QuicSocketFactory",InetSocketAddress (Ipv4Address::GetAny (), port2));
  sinkApps.Add (sink.Install (c .Get (5)));  //receiver node number 
  sinkApps.Add (sink1.Install (c .Get (6)));   
  sinkApps.Add (sink2.Install (c .Get (7)));   
  
    source.SetAttribute("OnTime", PointerValue(onTimeRandomSource));
    source.SetAttribute("OffTime", PointerValue(offTimeRandomSource));
    source1.SetAttribute("OnTime", PointerValue(onTimeRandomSource1));
    source1.SetAttribute("OffTime", PointerValue(offTimeRandomSource1));
    source2.SetAttribute("OnTime", PointerValue(onTimeRandomSource2));
    source2.SetAttribute("OffTime", PointerValue(offTimeRandomSource2));

  sourceApps.Start (Seconds (onTimeRandomSource->GetValue()));

   FlowMonitorHelper flowmon;
   Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  Simulator::Stop (Seconds (duration));

  AnimationInterface anim ("quicv30.xml");
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
                NS_LOG_UNCOND("****************QUIC Simulation Results****************");
		NS_LOG_UNCOND("Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << " Dst Addr " << t.destinationAddress);
                 txPacket = iter->second.txPackets;
                  rxPacket = iter->second.rxPackets;
                  PacketLoss = txPacket - rxPacket;
                  delay = iter->second.delaySum.GetMilliSeconds();
                  //jitter = iter->second.jitterSum.GetMilliSeconds();
          //std::cout << "  Tx Packets: " << iter->second.txPackets << "\n";
          std::cout << "  Rx Packets: " << iter->second.rxPackets << "\n";
          std::cout << "  Packet Loss: " << PacketLoss << "\n";
  	  //std::cout << "  Tx Bytes:   " << iter->second.txBytes << "\n";
          //std::cout << "  Rx Bytes:   " << iter->second.rxBytes << "\n";
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
             NS_LOG_UNCOND("**********QUIC Simulation Results*************");
	throughput = rxBytessum * 8 / (duration * 1000000.0); //Mbit/s  
	//NS_LOG_UNCOND("Sent Packets = " << txPacketsum);
	//NS_LOG_UNCOND("Received Packets = " << rxPacketsum);
        NS_LOG_UNCOND("Total Packet Loss = " << (txPacketsum-rxPacketsum));
        //NS_LOG_UNCOND("Total Byte Sent = "<<txBytessum);
        //NS_LOG_UNCOND("Total Byte Received = "<<rxBytessum);
      	NS_LOG_UNCOND("Mean Delay: " << Delaysum / txPacketsum << " ms");
        //NS_LOG_UNCOND("Jitter: " << jitterSum / txPacketsum << " ms");
	std::cout << "Average Throughput = "<<throughput << " Mbit/s" << std::endl;
        std::cout << "Packets Delivery Ratio: " << ((rxPacketsum *100) / txPacketsum) << "%" << "\n";
        monitor->SerializeToXmlFile("Haithem.flowmon", true, true);

  Simulator::Destroy ();
}
