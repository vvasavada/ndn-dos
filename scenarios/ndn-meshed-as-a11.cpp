// ndn-meshed-as.cppp
// A-11: Attackers localized in local AS

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {
    int main(int argc, char* argv[]){
        CommandLine cmd;
        cmd.Parse(argc, argv);

        AnnotatedTopologyReader topologyReader("", 1);
        topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-meshed-as.txt");
        topologyReader.Read();

        // Install NDN stack on all nodes
        ndn::StackHelper ndnHelper;
        ndnHelper.InstallAll();

        // Installing global routing interface on all nodes
        ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
        ndnGlobalRoutingHelper.InstallAll();

        // Getting consumers
        Ptr<Node> ucla_cs_attacker = Names::Find<Node>("ucla-cs"); //localized attacker
        Ptr<Node> ucla_math_attacker = Names::Find<Node>("ucla-math"); // localized attacker

        ndn::AppHelper consumerHelper("ConsApp");
        consumerHelper.SetAttribute("Name", StringValue("/edu/ucla/cs/alicelovecpp/xyz"));
        consumerHelper.SetAttribute("Frequency", StringValue("10"));
        //consumerHelper.SetAttribute("AutoAppend", StringValue("false"));
        consumerHelper.Install(ucla_cs_attacker);

        Ptr<Node> az_math = Names::Find<Node>("az-math");
        Ptr<Node> az_cs = Names::Find<Node>("az-cs");
        Ptr<Node> wc_westwood = Names::Find<Node>("wc-westwood");
        Ptr<Node> wc_santa_monica = Names::Find<Node>("wc-santa-monica");
        Ptr<Node> mmphs_ee = Names::Find<Node>("mmphs-ee");

        // Getting producers
        Ptr<Node> mmphs_computer_bob00 = Names::Find<Node>("mmphs-computer-bob00");
        Ptr<Node> ucla_cs_webserver0_index = Names::Find<Node>("ucla-cs-webserver0-index");
        Ptr<Node> ucla_cs_alicelovecpp = Names::Find<Node>("ucla-cs-alicelovecpp"); // target


        //ndn::AppHelper consumerHelper("ns3::ndn::Consumer");
        //consumerHelper("Prefix", StringValue("/edu/ucla/cs/alicelovecpp"));
        //consumerHelper.Install(ucla_cs_attacker);
        //consumerHelper.Install(ucla_math_attacker);

        ndn::AppHelper producerHelper("ProdApp");
        //producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

        ndnGlobalRoutingHelper.AddOrigins("/edu/ucla/cs/alicelovecpp", ucla_cs_alicelovecpp);
        producerHelper.SetPrefix("/edu/ucla/cs/alicelovecpp");
        producerHelper.Install(ucla_cs_alicelovecpp);

        ndnGlobalRoutingHelper.CalculateRoutes();


        Simulator::Stop(Seconds(20.0));

        L2RateTracer::InstallAll("a11.txt", Seconds(0.5));

        Simulator::Run();
        Simulator::Destroy();

        return 0;
    }
} // namespace ns3

int main(int argc, char* argv[]){
    return ns3::main(argc, argv);
}

