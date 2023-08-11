/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 NITK Surathkal
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mohit P. Tahiliani <tahiliani@nitk.edu.in>
 *
 */

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

static void Tracing (Ptr<FlowMonitor> monitor){
  FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();
  std::ofstream file_out_thoroughput (exp_name+"/net_throughput.dat", std::ios::out | std::ios::app);
  std::ofstream file_out_delivery (exp_name+"/delivery_ratio.dat", std::ios::out | std::ios::app);
  Time curTime = Now ();
  
  // threshold
  double tot_thr = 0;
  // delay
  double tot_rx_packets = 0;
  // drop and delivery
  double tot_drop = 0;
  double tot_delivery = 0;
  int num_flows = 0;
  for(auto itr:stats)
  {
    // threshold
    tot_thr += (8 * itr.second.rxBytes ) / (1.0 * curTime.GetSeconds () );

    tot_rx_packets += itr.second.rxPackets;
    // drop and delivery

    tot_drop += itr.second.lostPackets;
    tot_delivery += itr.second.rxPackets;
    num_flows++;
  }
  file_out_thoroughput <<  curTime << " " << tot_thr << std::endl; // throughput (bit/s)
  file_out_delivery <<  curTime << " " << (100.0 * tot_delivery)/(tot_rx_packets+tot_drop) << std::endl; // delivery ratio (%)
  Simulator::Schedule (Seconds (0.1), &Tracing, monitor);
}


NS_LOG_COMPONENT_DEFINE ("MyRedAredWifiExample");

int main (int argc, char *argv[])
{
  
  //User Parameters
  uint32_t nWifi = 10; // 20,40,60,80,100
  int32_t packet_rate = 100; //100,200,300,400,500
  int32_t flow = 100; // 10 , 20 , 40 , 70 , 100  -----> x 2
  int32_t speed = 10; // 10 , 20 , 30 , 40 , 50

  CommandLine cmd (__FILE__);
  cmd.AddValue ("nWifi", "Set number nodes", nWifi);
  cmd.AddValue ("packet_rate", "Set Packet Rate",packet_rate);
  cmd.AddValue ("flow", "Set number of flow", flow);
  cmd.AddValue ("speed", "Set Speed", speed);
  cmd.Parse (argc,argv);

  //fixed-packet and speed information
  uint32_t  pktSize = 10000000/packet_rate;
  std::string appDataRate = "5Mbps";

  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (pktSize));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue (appDataRate));

  LogComponentEnable("PacketSink", LOG_LEVEL_INFO);

  /**
   * @brief 2 point to point nodes, which later will work as AP nodes for wifi
   * 
   */

  NodeContainer pointToPoint;
  pointToPoint.Create (2);

  // Create the point-to-point link helpers
  PointToPointHelper bottleNeckLink;
  bottleNeckLink.SetDeviceAttribute  ("DataRate", StringValue ("5Mbps"));
  bottleNeckLink.SetChannelAttribute ("Delay", StringValue ("20ms"));

  NetDeviceContainer p2pBottleNeckDevices;
  p2pBottleNeckDevices = bottleNeckLink.Install (pointToPoint);

  /**
   * @brief wifi left + right
   * 
   */

  NodeContainer wifiStaNodesLeft,wifiStaNodesRight;
  wifiStaNodesLeft.Create (nWifi);
  wifiStaNodesRight.Create (nWifi);
  NodeContainer wifiApNodeLeft = pointToPoint.Get (0);
  NodeContainer wifiApNodeRight = pointToPoint.Get (1);

  // PHY helper
  // constructs the wifi devices and the interconnection channel between these wifi nodes.
  YansWifiChannelHelper channelLeft = YansWifiChannelHelper::Default ();
  YansWifiChannelHelper channelRight = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phyLeft,phyRight;
  phyLeft.SetChannel (channelLeft.Create ()); //  all the PHY layer objects created by the YansWifiPhyHelper share the same underlying channel
  phyRight.SetChannel (channelRight.Create ()); 

  // MAC layer
  WifiHelper wifi;
  wifi.SetStandard(WIFI_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager"); // tells the helper the type of rate control algorithm to use, here AARF algorithm

  WifiMacHelper macLeft,macRight;
  Ssid ssidLeft = Ssid ("ns-3-ssid-left"); //  creates an 802.11 service set identifier (SSID) object
  Ssid ssidRight = Ssid ("ns-3-ssid-right"); 
  
  // configure Wi-Fi for all of our STA nodes
  macLeft.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssidLeft),
               "ActiveProbing", BooleanValue (false)); 
               
  macRight.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssidRight),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevicesLeft, staDevicesRight;
  staDevicesLeft = wifi.Install (phyLeft, macLeft, wifiStaNodesLeft);
  staDevicesRight = wifi.Install (phyRight, macRight, wifiStaNodesRight);

  //  configure the AP (access point) node
  macLeft.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidLeft));
  macRight.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssidRight));

  NetDeviceContainer apDevicesLeft, apDevicesRight;
  apDevicesLeft = wifi.Install (phyLeft, macLeft, wifiApNodeLeft); // single AP which shares the same set of PHY-level Attributes (and channel) as the station
  apDevicesRight = wifi.Install (phyRight, macRight, wifiApNodeRight); 


  /**
   * 
   * Mobility Model
   * 
   * We want the STA nodes to be mobile, wandering around inside a bounding box, 
   * and we want to make the AP node stationary
   * 
   **/

  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (0.5),
                                 "DeltaY", DoubleValue (1.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  

  // tell AP node to stay still
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)),
                             "Speed", StringValue ("ns3::ConstantRandomVariable[Constant="+std::to_string(speed)+"]"));
  
  // install on STA nodes
  mobility.Install (wifiStaNodesLeft);
  mobility.Install (wifiStaNodesRight);

  // tell AP node to stay still
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  // install on AP node
  mobility.Install (wifiApNodeLeft);
  mobility.Install (wifiApNodeRight);

  // Install Stack
  InternetStackHelper stack;
  stack.Install (wifiApNodeLeft);
  stack.Install (wifiApNodeRight);
  stack.Install (wifiStaNodesLeft);
  stack.Install (wifiStaNodesRight);

  // Assign IP Addresses
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


  // Install on/off app on all right side nodes
  OnOffHelper clientHelper ("ns3::TcpSocketFactory", Address ());
  // clientHelper.SetAttribute ("OnTime", StringValue ("ns3::UniformRandomVariable[Min=0.|Max=1.]"));
  // clientHelper.SetAttribute ("OffTime", StringValue ("ns3::UniformRandomVariable[Min=0.|Max=1.]"));
  clientHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  clientHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
 
  Address sinkLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), 9));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", sinkLocalAddress);

  ApplicationContainer sinkApps;
  for (uint32_t i = 0; i < nWifi; ++i)
    {
      // create sink app on left side node
      sinkApps.Add (packetSinkHelper.Install (wifiStaNodesLeft.Get(i)));
    }
  sinkApps.Start (Seconds (1.0));
  sinkApps.Stop (Seconds (6.5));

  // sinkApps.Start (Seconds (0.0));
  // sinkApps.Stop (Seconds (30.0));

  ApplicationContainer clientApps;
  int cur_flow_count = 0;
  for (uint32_t i = 0; i < nWifi; ++i)
    {
      // Create an on/off app on right side node which sends packets to the left side
      AddressValue remoteAddress (InetSocketAddress (staNodeInterfacesLeft.GetAddress(i), 9));
      
      for(uint32_t j = 0; j < nWifi; ++j)
      {
        clientHelper.SetAttribute ("Remote", remoteAddress);
        clientApps.Add (clientHelper.Install (wifiStaNodesRight.Get(j)));

        cur_flow_count++;
        if(cur_flow_count >= flow)
          break;
      }

      if(cur_flow_count >= flow)
          break;

      // clientHelper.SetAttribute ("Remote", remoteAddress);
      // clientApps.Add (clientHelper.Install (wifiStaNodesRight.Get(i)));
    }
  clientApps.Start (Seconds (2.0)); // Start 1 second after sink
  clientApps.Stop (Seconds (6.0)); // Stop before the sink  
  // clientApps.Start (Seconds (1.0)); // Start 1 second after sink
  // clientApps.Stop (Seconds (17.0)); // Stop before the sink

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (6.5)); // force stop,

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

  // AsciiTraceHelper ascii;
  // bottleNeckLink.EnableAsciiAll (ascii.CreateFileStream (exp_name+"/red-wifi.tr"));
  // bottleNeckLink.EnablePcapAll (exp_name+"/red-wifi");

  //&Flow calculation---------------------------------------------------
  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();
  Simulator::Schedule (Seconds (2.2), &Tracing, flowMonitor);
  Simulator::Run ();
  flowMonitor->SerializeToXmlFile(exp_name+"/flow.xml", true, true);
  Simulator::Destroy ();
  return 0;
}