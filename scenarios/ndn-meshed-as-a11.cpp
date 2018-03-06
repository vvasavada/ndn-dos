// ndn-meshed-as.cppp
// A-11: Attackers localized in local network

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
        //ndnHelper.setCsSize(200);
        ndnHelper.InstallAll();

        // Installing global routing interface on all nodes
        ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
        ndnGlobalRoutingHelper.InstallAll();

        // Getting consumers

        // localized attackers
        Ptr<Node> ucla_cs_attacker = Names::Find<Node>("ucla-cs");

        // global attackers
        Ptr<Node> ucla_math_attacker = Names::Find<Node>("ucla-math");
        Ptr<Node> az_cs_attacker = Names::Find<Node>("az-cs");
        Ptr<Node> az_math_attacker = Names::Find<Node>("az-math"); 
        Ptr<Node> wc_westwood_attacker = Names::Find<Node>("wc-westwood");
        Ptr<Node> wc_santa_monica_attacker = Names::Find<Node>("wc-santa-monica");
        Ptr<Node> mmphs_ee_attacker = Names::Find<Node>("mmphs-ee");


        ndn::AppHelper consumerHelper("ConsApp");
        //consumerHelper.SetAttribute("Name", StringValue("/edu/ucla/cs/alicebla"));
        //consumerHelper.SetAttribute("Frequency", StringValue("1"));
        //consumerHelper.SetAttribute("Randomize", StringValue("uniform"));
        //consumerHelper.Install(ucla_cs_attacker); // should be able to reach
        //consumerHelper.Install(ucla_math_attacker); // should not be able to reach
        //consumerHelper.Install(az_cs_attacker); // should not be able to reach

        //consumerHelper.SetAttribute("Name", StringValue("/edu/ucla/cs/webserver0/index.html"));
        //consumerHelper.SetAttribute("Frequency", StringValue("1"));
        //consumerHelper.Install(az_cs_attacker); // should be able to reach
        //consumerHelper.Install(ucla_cs_attacker); // should be able to reach

        // Getting producers
        Ptr<Node> mmphs_computer_bob00 = Names::Find<Node>("mmphs-computer-bob00");
        Ptr<Node> ucla_cs_webserver0_index = Names::Find<Node>("ucla-cs-webserver0-index");
        Ptr<Node> ucla_cs_alicelovecpp = Names::Find<Node>("ucla-cs-alicelovecpp"); // target

        ndn::AppHelper producerHelper("ProdApp");
        
        ndnGlobalRoutingHelper.AddOrigins("/edu/ucla/cs/webserver0/index.html", ucla_cs_webserver0_index);
        producerHelper.SetPrefix("/edu/ucla/cs/webserver0/index.html");
        producerHelper.Install(ucla_cs_webserver0_index);

        producerHelper.SetPrefix("/edu/ucla/cs/alicelovecpp");
        producerHelper.Install(ucla_cs_alicelovecpp);

        Ptr<Node> rtr_ucla_cs = Names::Find<Node>("rtr-ucla-cs");
        ndnGlobalRoutingHelper.AddOrigins("/edu/ucla/cs", rtr_ucla_cs);

        ndnGlobalRoutingHelper.CalculateRoutes();


        ndn::FibHelper::AddRoute("ucla-cs", "/edu/ucla/cs/alicelovecpp", "rtr-ucla-cs", 1);
        ndn::FibHelper::AddRoute("rtr-ucla-cs", "/edu/ucla/cs/alicelovecpp", "ucla-cs-alicelovecpp", 1);



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
