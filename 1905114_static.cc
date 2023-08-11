
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/traffic-control-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-module.h"
#include <iostream>
#include <iomanip>
#include <map>

using namespace ns3;


std::string exp_name = "1905114-offline-2-Static";
int32_t plot2;
uint32_t nWifi = 10; // 3 , 6, 10 , 13 , 18 
int32_t packet_rate = 100; //100,200,300,400,500
int32_t flow = 100; // 10 , 20 , 40 , 70 , 100  -----> x 2
int32_t range =1; //1,2,3,4,5


static void Tracing (Ptr<FlowMonitor> monitor){
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
  std::ofstream file_out_thoroughput (exp_name+"/net_throughput.dat", std::ios::app);
  std::ofstream file_out_delivery  (exp_name+"/delivery_ratio.dat",std::ios::app);
  Time curTime = Now ();
  

  double tot_thr = 0;
  double tot_rx_packets = 0;
  double tot_drop = 0;
  double tot_delivery = 0;
  int flows = 0;
  for(auto itr:stats)
  {
   
    tot_thr += (8 * itr.second.rxBytes ) / (1.0 * curTime.GetSeconds () );

    tot_rx_packets += itr.second.rxPackets;


    tot_drop += itr.second.lostPackets;
    tot_delivery += itr.second.rxPackets;
    flows++;
  }
  if(plot2==1){
    file_out_thoroughput <<  nWifi << " " << tot_thr << std::endl; 
    file_out_delivery  <<  nWifi << " " << (100.0 * tot_delivery)/(tot_rx_packets+tot_drop) << std::endl; 
  }else if(plot2==2){
    file_out_thoroughput <<  flow << " " << tot_thr << std::endl; 
    file_out_delivery  <<  flow << " " << (100.0 * tot_delivery)/(tot_rx_packets+tot_drop) << std::endl; 
  }else if(plot2==3){
    file_out_thoroughput <<  packet_rate << " " << tot_thr << std::endl; 
    file_out_delivery  <<  packet_rate << " " << (100.0 * tot_delivery)/(tot_rx_packets+tot_drop) << std::endl; 
  }else if(plot2==4){
    file_out_thoroughput <<  range << " " << tot_thr << std::endl; 
    file_out_delivery  <<  range << " " << (100.0 * tot_delivery)/(tot_rx_packets+tot_drop) << std::endl; 
  }


  Simulator::Schedule (Seconds (0.1), &Tracing, monitor);
}


NS_LOG_COMPONENT_DEFINE ("MyRedAredWifiExample");

int main (int argc, char *argv[])
{
  //User Parameters
  // uint32_t nWifi = 10; // 3 , 6, 10 , 13 , 18 
  // int32_t packet_rate = 100; //100,200,300,400,500
  // int32_t flow = 100; // 10 , 20 , 40 , 70 , 100  -----> x 2
  // int32_t range =1; //1,2,3,4,5
   int32_t number = 0; 
  int32_t plot = 1;

  CommandLine cmd (__FILE__);
  cmd.AddValue ("nWifi", "Set number nodes", nWifi);
  cmd.AddValue ("packet_rate", "Set Packet Rate",packet_rate);
  cmd.AddValue ("flow", "Set number of flow", flow);
  cmd.AddValue ("range", "Set Range", range);
  cmd.AddValue ("number", "Set Speed", number);
  cmd.AddValue ("plot", "Plot type", plot);
  cmd.Parse (argc,argv);

   exp_name +=std::to_string(number);

   plot2=plot;

  //fixed-packet and speed information
  uint32_t  pktSize = 10000000/packet_rate;
  std::string appDataRate = "5Mbps";

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (pktSize));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue (appDataRate));

  LogComponentEnable("PacketSink", LOG_LEVEL_INFO);


  NodeContainer pointToPoint;
  pointToPoint.Create (2);

  // Create the point-to-point link helpers
  PointToPointHelper bottleNeckLink;
  bottleNeckLink.SetDeviceAttribute  ("DataRate", StringValue ("5Mbps"));
  bottleNeckLink.SetChannelAttribute ("Delay", StringValue ("20ms"));

  NetDeviceContainer p2pBottleNeckDevices;
  p2pBottleNeckDevices = bottleNeckLink.Install (pointToPoint);

  NodeContainer wifiStaNodesLeft,wifiStaNodesRight;
  wifiStaNodesLeft.Create (nWifi);
  wifiStaNodesRight.Create (nWifi);
  NodeContainer wifiApNodeLeft = pointToPoint.Get (0);
  NodeContainer wifiApNodeRight = pointToPoint.Get (1);

  YansWifiChannelHelper channelLeft = YansWifiChannelHelper::Default ();
  YansWifiChannelHelper channelRight = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phyLeft,phyRight;
  phyLeft.SetChannel (channelLeft.Create ());   phyRight.SetChannel (channelRight.Create ()); 

  // MAC layer
  WifiHelper wifi;
  wifi.SetStandard(WIFI_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager"); 
  WifiMacHelper macLeft,macRight;
  Ssid ssidLeft = Ssid ("ns-3-ssid-left"); 
  Ssid ssidRight = Ssid ("ns-3-ssid-right"); 
  

  macLeft.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssidLeft),
               "ActiveProbing", BooleanValue (false)); 
               
  macRight.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssidRight),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevicesLeft, staDevicesRight;
  staDevicesLeft = wifi.Install (phyLeft, macLeft, wifiStaNodesLeft);
  staDevicesRight = wifi.Install (phyRight, macRight, wifiStaNodesRight);

  //  configure the AP node
  macLeft.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidLeft));
  macRight.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidRight));

  NetDeviceContainer apDevicesLeft, apDevicesRight;
  apDevicesLeft = wifi.Install (phyLeft, macLeft, wifiApNodeLeft); 
  apDevicesRight = wifi.Install (phyRight, macRight, wifiApNodeRight); 


  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (range),
                                 "DeltaY", DoubleValue (range),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  


  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (wifiApNodeLeft);
  mobility.Install (wifiApNodeRight);
  mobility.Install (wifiStaNodesLeft);
  mobility.Install (wifiStaNodesRight);

  InternetStackHelper stack;
  stack.Install (wifiApNodeLeft);
  stack.Install (wifiApNodeRight);
  stack.Install (wifiStaNodesLeft);
  stack.Install (wifiStaNodesRight);

  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pBottleNeckDevices);

  Ipv4InterfaceContainer staNodeInterfacesLeft, staNodeInterfacesRight;
  Ipv4InterfaceContainer apNodeInterfaceLeft, apNodeInterfaceRight;

  address.SetBase ("10.1.2.0", "255.255.255.0");
  staNodeInterfacesLeft = address.Assign (staDevicesLeft);
  apNodeInterfaceLeft = address.Assign (apDevicesLeft);

  address.SetBase ("10.1.3.0", "255.255.255.0");
  staNodeInterfacesRight = address.Assign (staDevicesRight);
  apNodeInterfaceRight = address.Assign (apDevicesRight);



  OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address ());

  clientHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  clientHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
 
  Address sinkLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), 9));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);

  ApplicationContainer sinkApps;
  for (uint32_t i = 0; i < nWifi; ++i)
    {
  
      sinkApps.Add (packetSinkHelper.Install (wifiStaNodesRight.Get(i)));
    }
  sinkApps.Start (Seconds (1.0));
  sinkApps.Stop (Seconds (6.5));



  ApplicationContainer clientApps;
  int cur_flow_count = 0;
  for (uint32_t i = 0; i < nWifi; ++i)
    {
   
      AddressValue remoteAddress (InetSocketAddress (staNodeInterfacesRight.GetAddress(i), 9));
      
      for(uint32_t j = 0; j < nWifi; ++j)
      {
        clientHelper.SetAttribute ("Remote", remoteAddress);
        clientApps.Add (clientHelper.Install (wifiStaNodesLeft.Get(j)));

        cur_flow_count++;
        if(cur_flow_count >= flow)
          break;
      }

      if(cur_flow_count >= flow)
          break;

    }
  clientApps.Start (Seconds (2.0)); 
  clientApps.Stop (Seconds (6.0)); 


  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (6.5)); 

  std::string dirToSave = "mkdir -p " + exp_name;
  std::string dirToDel = "rm -rf " + exp_name;
  if (system (dirToDel.c_str ()) == -1)
  {
      exit (1);
  }
  if (system (dirToSave.c_str ()) == -1)
  {
      exit (1);
  }

  //&Flow calculation---------------------------------------------------
  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();
  Simulator::Schedule (Seconds (2.2), &Tracing, flowMonitor);
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}