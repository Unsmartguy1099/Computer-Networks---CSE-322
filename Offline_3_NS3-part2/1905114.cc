
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/applications-module.h"
#include "ns3/stats-module.h"
#include "ns3/callback.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/csma-module.h"
#include "ns3/ssid.h"
#include "ns3/yans-wifi-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Task 1");

std::ofstream writer1;
std::ofstream writer2;
std::ofstream writer3;
std::ofstream writer4;
std::ofstream writer5;
std::ofstream writer6;

static void
CongestionWindow1 ( uint32_t oldCwnd, uint32_t CongestionWindow)
{
  //Algo1
  writer5<<Simulator::Now().GetSeconds()<<" "<<CongestionWindow<<std::endl;
}

static void
CongestionWindow2 ( uint32_t oldCwnd, uint32_t CongestionWindow)
{
  //Algo2
  writer6<<Simulator::Now().GetSeconds()<<" "<<CongestionWindow<<std::endl;
}

class TutorialApp : public Application
{
  public:
    TutorialApp();
    ~TutorialApp() override;

    /**
     * Register this type.
     * \return The TypeId.
     */
    static TypeId GetTypeId();

    /**
     * Setup the socket.
     * \param socket The socket.
     * \param address The destination address.
     * \param packetSize The packet size to transmit.
     * \param nPackets The number of packets to transmit.
     * \param dataRate the data rate to use.
     */
    void Setup(Ptr<Socket> socket,
               Address address,
               uint32_t packetSize,
               uint32_t nPackets,
               DataRate dataRate);

  private:
    void StartApplication() override;
    void StopApplication() override;

    /// Schedule a new transmission.
    void ScheduleTx();
    /// Send a packet.
    void SendPacket();

    Ptr<Socket> m_socket;   //!< The transmission socket.
    Address m_peer;         //!< The destination address.
    uint32_t m_packetSize;  //!< The packet size.
    uint32_t m_nPackets;    //!< The number of packets to send.
    DataRate m_dataRate;    //!< The data rate to use.
    EventId m_sendEvent;    //!< Send event.
    bool m_running;         //!< True if the application is running.
    uint32_t m_packetsSent; //!< The number of packets sent.
};
TutorialApp::TutorialApp()
    : m_socket(nullptr),
      m_peer(),
      m_packetSize(0),
      m_nPackets(0),
      m_dataRate(0),
      m_sendEvent(),
      m_running(false),
      m_packetsSent(0)
{
}

TutorialApp::~TutorialApp()
{
    m_socket = nullptr;
}

/* static */
TypeId
TutorialApp::GetTypeId()
{
    static TypeId tid = TypeId("TutorialApp")
                            .SetParent<Application>()
                            .SetGroupName("Tutorial")
                            .AddConstructor<TutorialApp>();
    return tid;
}

void
TutorialApp::Setup(Ptr<Socket> socket,
                   Address address,
                   uint32_t packetSize,
                   uint32_t nPackets,
                   DataRate dataRate)
{
    m_socket = socket;
    m_peer = address;
    m_packetSize = packetSize;
    m_nPackets = nPackets;
    m_dataRate = dataRate;
}

void
TutorialApp::StartApplication()
{
    m_running = true;
    m_packetsSent = 0;
    m_socket->Bind();
    m_socket->Connect(m_peer);
    SendPacket();
}

void
TutorialApp::StopApplication()
{
    m_running = false;

    if (m_sendEvent.IsRunning())
    {
        Simulator::Cancel(m_sendEvent);
    }

    if (m_socket)
    {
        m_socket->Close();
    }
}

void
TutorialApp::SendPacket()
{
    Ptr<Packet> packet = Create<Packet>(m_packetSize);
    m_socket->Send(packet);

    if (++m_packetsSent < m_nPackets)
    {
        ScheduleTx();
    }
}

void
TutorialApp::ScheduleTx()
{
    if (m_running)
    {
        Time tNext(Seconds(m_packetSize * 8 / static_cast<double>(m_dataRate.GetBitRate())));
        m_sendEvent = Simulator::Schedule(tNext, &TutorialApp::SendPacket, this);
    }
}

int main(int argc, char *argv[]){

  writer1.open("scratch/throughput_vs_bottleneck_algo1.dat",std::ios::app | std::ios::out);
  writer2.open("scratch/throughput_vs_bottleneck_algo2.dat",std::ios::app | std::ios::out);
  writer3.open("scratch/throughput_vs_pcktloss_algo1.dat",std::ios::app | std::ios::out);
  writer4.open("scratch/throughput_vs_pcktloss_algo2.dat",std::ios::app | std::ios::out);
  writer5.open("scratch/CongestionWindow_vs_Time_algo1.dat",std::ios::app | std::ios::out);
  writer6.open("scratch/CongestionWindow_vs_Time_algo2.dat",std::ios::app | std::ios::out);

  bool verbose = false;
  bool tracing = false;

  std::string tcpAlgo1 = "ns3::TcpNewReno";
  std::string tcpAlgo2 = "ns3::TcpWestwoodPlus";
  int bottleNeckLinkDataRate =1;
  int bottleNeckLinkDelay =100;
  int leafDelay = 1;
  int leafDataRate = 1;
  int packetSize = 1024;
  int bandwidth_delay_product =10000;
  double errorRate = 0.0001;
  int type =1;

  // input from command line
  CommandLine cmd (__FILE__);
  cmd.AddValue ("verbose","give verbose: ", verbose);
  cmd.AddValue ("tracing","give tracing: ", tracing);
  cmd.AddValue ("tcpVariant1","Give name of TCP variant 1: ", tcpAlgo1);
  cmd.AddValue ("tcpVariant2","Give name of TCP variant 2: ", tcpAlgo2);
  cmd.AddValue ("bottleNechDataRate","Give BottlenechDataRate: ", bottleNeckLinkDataRate);
  cmd.AddValue ("bottleNeckDelay","Give BottleNeckDelay: ", bottleNeckLinkDelay);
  cmd.AddValue ("leafDelay","Give leafDelay: ", leafDelay);
  cmd.AddValue ("leafDataRate","Give leafDataRate: ", leafDataRate);
  cmd.AddValue ("packetSize","Give packetSize: ", packetSize);
  cmd.AddValue ("bandwidth_delay_product","Give bandwidth: ", bandwidth_delay_product);
  cmd.AddValue ("errorRate","Give errorrate: ", errorRate);
  cmd.AddValue ("type","Give type: ", type);
  cmd.Parse (argc,argv);

  errorRate=1/errorRate;

  bottleNeckLinkDataRate = bottleNeckLinkDataRate*10000;
  bandwidth_delay_product=(bottleNeckLinkDataRate*bottleNeckLinkDelay)/packetSize;

if(verbose){
  LogComponentEnable("TutorialApp",LOG_LEVEL_INFO);
  LogComponentEnable("PacketSink",LOG_LEVEL_INFO);
}

PointToPointHelper p2p;
p2p.SetChannelAttribute("Delay",StringValue(std::to_string(bottleNeckLinkDelay)+"ms"));
p2p.SetDeviceAttribute("DataRate",StringValue(std::to_string(bottleNeckLinkDataRate)+"Mbps"));

PointToPointHelper p2pLeaf;
p2pLeaf.SetChannelAttribute("Delay",StringValue(std::to_string(leafDelay)+"ms"));
p2pLeaf.SetDeviceAttribute("DataRate",StringValue(std::to_string(leafDataRate)+"Gbps"));

p2p.SetQueue("ns3::DropTailQueue","MaxSize",StringValue(std::to_string(bandwidth_delay_product)+"p"));

PointToPointDumbbellHelper p2pDumbell(2,p2pLeaf,2,p2pLeaf,p2p);
                              
    Ptr<RateErrorModel> em = CreateObject<RateErrorModel> ();
    em->SetAttribute ("ErrorRate", DoubleValue (errorRate));
    p2pDumbell.m_routerDevices.Get(1)->SetAttribute ("ReceiveErrorModel", PointerValue (em)); 

    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (tcpAlgo1));
    InternetStackHelper stack;
    stack.Install (p2pDumbell.GetLeft(0));
    stack.Install (p2pDumbell.GetRight(0));
    stack.Install (p2pDumbell.GetLeft());
    stack.Install (p2pDumbell.GetRight());

    // tcp variant 2
    Config::SetDefault ("ns3::TcpL4Protocol::SocketType", StringValue (tcpAlgo2));
    InternetStackHelper stack2;
    stack.Install (p2pDumbell.GetLeft(1));
    stack.Install (p2pDumbell.GetRight(1));

    // ASSIGN IP Addresses
    p2pDumbell.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"), // left nodes
                          Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"),  // right nodes
                          Ipv4AddressHelper ("10.3.1.0", "255.255.255.0")); // routers 
    Ipv4GlobalRoutingHelper::PopulateRoutingTables (); // populate routing table
    

    // install flow monitor
    FlowMonitorHelper flowmon;
    //flowmon.SetMonitorAttribute("MaxPerHopDelay", TimeValue(Seconds(cleanupTime)));
    Ptr<FlowMonitor> monitor = flowmon.InstallAll ();
    uint16_t port = 8080;




//^server1
      Address sinkAddress1 (InetSocketAddress (p2pDumbell.GetRightIpv4Address (0), port));
      PacketSinkHelper packetSinkHelper1 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny(), port));
      ApplicationContainer sinkApps1 = packetSinkHelper1.Install (p2pDumbell.GetRight (0));
      sinkApps1.Start (Seconds (0));
      sinkApps1.Stop (Seconds (20));

//^ client1
      Ptr<Socket> ns3TcpSocket = Socket::CreateSocket (p2pDumbell.GetLeft (0), TcpSocketFactory::GetTypeId ());
      ns3TcpSocket->TraceDisconnectWithoutContext("CongestionWindow",MakeCallback(&CongestionWindow1));

      
      Ptr<TutorialApp> app1 = CreateObject<TutorialApp> ();
      app1->Setup (ns3TcpSocket, sinkAddress1,packetSize, 100000000, DataRate("1Mbps"));
      p2pDumbell.GetLeft (0)->AddApplication (app1);
      app1->SetStartTime (Seconds (1));
      app1->SetStopTime (Seconds (20));

//^server2
      Address sinkAddress2 (InetSocketAddress (p2pDumbell.GetRightIpv4Address (1), port));
      PacketSinkHelper packetSinkHelper2 ("ns3::TcpSocketFactory", InetSocketAddress (Ipv4Address::GetAny(), port));
      ApplicationContainer sinkApps2 = packetSinkHelper2.Install (p2pDumbell.GetRight (1));
      sinkApps2.Start (Seconds (0));
      sinkApps2.Stop (Seconds (20));

//^ client2
      Ptr<Socket> ns3TcpSocket2 = Socket::CreateSocket (p2pDumbell.GetLeft (0), TcpSocketFactory::GetTypeId ());
      ns3TcpSocket2->TraceDisconnectWithoutContext("CongestionWindow",MakeCallback(&CongestionWindow2));

      
      Ptr<TutorialApp> app2= CreateObject<TutorialApp> ();
      app2->Setup (ns3TcpSocket2, sinkAddress2, packetSize, 100000000, DataRate("1Mbps"));
      p2pDumbell.GetLeft (1)->AddApplication (app2);
      app2->SetStartTime (Seconds (1));
      app2->SetStopTime (Seconds (20));

      Simulator::Stop(Seconds(20));
      Simulator::Run();

  // flow monitor statistics
  int counter=0;

  float Throughput1 = 0;
  float Throughput2 = 0;

  float pcktlossRate1 = 0;
  float pcktlossRate2 = 0;

    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats ();

    for (auto iter = stats.begin (); iter != stats.end (); ++iter) {
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first); 
            // classifier returns FiveTuple in correspondance to a flowID
            //simulation timw 20 seconds
      NS_LOG_UNCOND("Flow: " <<iter->first<<"----------------------------");
      NS_LOG_UNCOND("Src Addr" <<t.sourceAddress << "  Dst Addr "<< t.destinationAddress);
      NS_LOG_UNCOND("Sent Packets = " <<iter->second.txPackets);
      NS_LOG_UNCOND("Received Packets = " <<iter->second.rxPackets);
      NS_LOG_UNCOND("Lost Packets = " <<iter->second.lostPackets);
      NS_LOG_UNCOND("Packet delivery ratio = " <<iter->second.rxPackets*100.0/iter->second.txPackets << "%");
      NS_LOG_UNCOND("Packet loss ratio = " << (iter->second.lostPackets)*100.0/iter->second.txPackets << "%");
      NS_LOG_UNCOND("Delay = " <<iter->second.delaySum / iter->second.rxPackets);
      double throughputKbps = (iter->second.rxBytes * 8.0) / (20.0 * 1000.0); 
      NS_LOG_UNCOND("Throughput = " << throughputKbps << " Kbps");
      NS_LOG_UNCOND("-----------------------------------");

      if(counter%2 == 0){
          Throughput1 = Throughput1 + throughputKbps ; 
      }
        
      if(counter%2 == 1){
          Throughput2 = Throughput1 + throughputKbps ; 
      }
      
      counter = counter + 1;
    }

    counter=counter/2;
    Throughput1=Throughput1/counter;
    Throughput2=Throughput2/counter;

    NS_LOG_UNCOND("Throughput1 = "<< Throughput1<< "Kbps");
    NS_LOG_UNCOND("Throughput2 = " << Throughput2<< "Kbps");


    if(type==1){
        writer1<< bottleNeckLinkDataRate/10000<<" "<<Throughput1 <<std::endl;
        writer2<< bottleNeckLinkDataRate/10000<<" "<<Throughput2 <<std::endl;
    }else if(type==2){
        writer3<< errorRate<<" "<<Throughput1<<std::endl;
        writer4<< errorRate<<" "<<Throughput1<<std::endl;
    }

    Simulator::Destroy ();

    return 0;
}