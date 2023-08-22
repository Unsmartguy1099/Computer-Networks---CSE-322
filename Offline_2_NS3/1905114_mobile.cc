
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


std::string exp_name = "1905114-offline-2-Mobile";
int32_t plot2;
uint32_t nWifi2 = 10; // 3 , 6, 10 , 13 , 18 
int32_t packet_rate2 = 100; //100,200,300,400,500
int32_t flow2 = 100; // 10 , 20 , 40 , 70 , 100  -----> x 2
int32_t speed2= 10; //1,2,3,4,5
int32_t number2 = 1;

static void Tracing (Ptr<FlowMonitor> monitor){
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
    std::ofstream file_out_thoroughput;
    std::ofstream file_out_delivery;
    // Construct the file paths
    std::string throughput_file_path = exp_name + "/net_throughput"+std::to_string(number2)+".dat";
    std::string delivery_file_path = exp_name + "/delivery_ratio"+std::to_string(number2)+".dat";
    // Open files in append mode
    file_out_thoroughput.open(throughput_file_path, std::ios::app);
    file_out_delivery.open(delivery_file_path, std::ios::app);
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
    file_out_thoroughput <<  nWifi2 << " " << tot_thr << std::endl; 
    file_out_delivery  <<  nWifi2 << " " << (100.0 * tot_delivery)/(tot_rx_packets+tot_drop) << std::endl; 
  }else if(plot2==2){
    file_out_thoroughput <<  flow2 << " " << tot_thr << std::endl; 
    file_out_delivery  <<  flow2 << " " << (100.0 * tot_delivery)/(tot_rx_packets+tot_drop) << std::endl; 
  }else if(plot2==3){
    file_out_thoroughput <<  packet_rate2 << " " << tot_thr << std::endl; 
    file_out_delivery  <<  packet_rate2 << " " << (100.0 * tot_delivery)/(tot_rx_packets+tot_drop) << std::endl; 
  }else if(plot2==4){
    file_out_thoroughput<<speed2 << " " << tot_thr << std::endl; 
    file_out_delivery<<speed2<< " " << (100.0 * tot_delivery)/(tot_rx_packets+tot_drop) << std::endl; 
  }
  Simulator::Schedule (Seconds (0.1), &Tracing, monitor);
}


NS_LOG_COMPONENT_DEFINE ("MyRedAredWifiExample");

int main (int argc, char *argv[])
{
  
  //User Parameters
  int32_t nWifi = 30; // 20,40,60,80,100
  int32_t packet_rate = 100; //100,200,300,400,500
  int32_t flow = 100; // 10 , 20 , 40 , 70 , 100  -----> x 2
  int32_t speed = 10; // 10 , 20 , 30 , 40 , 50
  int32_t number = 0; 
  int32_t plot = 1;

  
  CommandLine cmd (__FILE__);
  cmd.AddValue ("nWifi", "Set number nodes", nWifi);
  cmd.AddValue ("packet_rate", "Set Packet Rate",packet_rate);
  cmd.AddValue ("flow", "Set number of flow", flow);
  cmd.AddValue ("speed", "Set Speed", speed);
  cmd.AddValue ("number", "Set Speed", number);
  cmd.AddValue ("plot", "Plot type", plot);
  cmd.Parse (argc,argv);

  exp_name +=std::to_string(plot);
   plot2=plot;
   nWifi2 = nWifi;
   packet_rate2 = packet_rate;
   flow2 = flow;
   speed2 =speed;
   number2=number;

  std::cout<<plot2<<" "<<nWifi2<<" "<<packet_rate2<<" "<<speed2<<std::endl;

  uint32_t  pktSize = 10000000/packet_rate;
  std::string appDataRate = "5Mbps";

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (pktSize));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue (appDataRate));

  LogComponentEnable("PacketSink", LOG_LEVEL_INFO);


  NodeContainer pointToPoint;
  pointToPoint.Create (2);

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

  macLeft.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidLeft));
  macRight.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidRight));

  NetDeviceContainer apDevicesLeft, apDevicesRight;
  apDevicesLeft = wifi.Install (phyLeft, macLeft, wifiApNodeLeft);   apDevicesRight = wifi.Install (phyRight, macRight, wifiApNodeRight); 


  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (0.5),
                                 "DeltaY", DoubleValue (1.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  


  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)),
                             "Speed", StringValue ("ns3::ConstantRandomVariable[Constant="+std::to_string(speed)+"]"));
  

  mobility.Install (wifiStaNodesLeft);
  mobility.Install (wifiStaNodesRight);


  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (wifiApNodeLeft);
  mobility.Install (wifiApNodeRight);


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

  //&Flow calculation---------------------------------------------------
  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();
  Simulator::Schedule (Seconds (2.2), &Tracing, flowMonitor);
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}