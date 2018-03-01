// ndn-meshed-as.cppp
// A-11: Attacker localized in local network
// TODO: Set appropriate attacker and consumers interests/sec to create congestion
// TODO: Collect statistics

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

        // Choosing forwarding strategy
        ndn::StrategyChoiceHelper::InstallAll("/prefix", "/localhost/nfd/strategy/best-route");

        // Installing global routing interface on all nodes
        ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
        ndnGlobalRoutingHelper.InstallAll();

        // Getting attackers
        Ptr<Node> ucla_cs_attacker = Names::Find<Node>("ucla-cs"); //localized

        // Getting consumers
        Ptr<Node> ucla_math = Names::Find<Node>("ucla-math");
        Ptr<Node> az_math = Names::Find<Node>("az-math");
        Ptr<Node> az_cs = Names::Find<Node>("az-cs");
        Ptr<Node> wc_westwood = Names::Find<Node>("wc-westwood");
        Ptr<Node> wc_santa_monica = Names::Find<Node>("wc-santa-monica");
        Ptr<Node> mmphs_ee = Names::Find<Node>("mmphs-ee");

        ndn::AppHelper consumerHelper("ns3::ndn::ConsumerCbr");
        consumerHelper.SetAttribute("Frequency", StringValue("100")); // interests per Second
        consumerHelper.SetAttribute("Randomize", StringValue("uniform"));

        // install consumer application on attackers expressing interests in appropriate namespace
        consumerHelper.SetPrefix("/edu/ucla/cs/alicelovecpp");
        consumerHelper.Install(ucla_cs_attacker);

        consumerHelper.SetAttribute("Frequency", StringValue("10")); // interests per Second
        consumerHelper.SetAttribute("Randomize", StringValue("uniform"));

        consumerHelper.SetPrefix("/edu/ucla/cs");
        consumerHelper.Install(ucla_math);

        consumerHelper.SetPrefix("/edu/ucla");
        consumerHelper.Install(az_math);
        consumerHelper.Install(az_cs);
        consumerHelper.Install(wc_westwood);
        consumerHelper.Install(wc_santa_monica);
        consumerHelper.Install(mmphs_ee);

        // Getting producers
        Ptr<Node> mmphs_computer_bob00 = Names::Find<Node>("mmphs-computer-bob00");
        Ptr<Node> ucla_cs_webserver0_index = Names::Find<Node>("ucla-cs-webserver0-index");
        Ptr<Node> ucla_cs_alicelovecpp = Names::Find<Node>("ucla-cs-alicelovecpp"); // target
       

        ndn::AppHelper producerHelper("ns3::ndn::Producer");
        producerHelper.SetAttribute("PayloadSize", StringValue("1024"));

        // Register prefixes in global routing controller
        // install producers that will satisfy those prefixes
        ndnGlobalRoutingHelper.AddOrigins("/edu/ucla", ucla_cs_webserver0_index);
        producerHelper.SetPrefix("/edu/ucla/cs/webserver0/index.html");
        producerHelper.Install(ucla_cs_webserver0_index);

        ndnGlobalRoutingHelper.AddOrigins("/edu/ucla/cs", ucla_cs_alicelovecpp);
        producerHelper.SetPrefix("/edu/ucla/cs/ucla-cs-alicelovecpp");
        producerHelper.Install(ucla_cs_alicelovecpp);

        ndnGlobalRoutingHelper.AddOrigins("/edu/memphis/computer", mmphs_computer_bob00);
        producerHelper.SetPrefix("/edu/memphis/computer/bob00");
        producerHelper.Install(mmphs_computer_bob00);

        // Calculate and install FIBs
        ndn::GlobalRoutingHelper::CalculateRoutes();

        Simulator::Stop(Seconds(20.0));

        //ndn::L3RateTracer::InstallAll("rate-trace.txt", Seconds(0.5));

        Simulator::Run();
        Simulator::Destroy();

        return 0;
    }
} // namespace ns3

int main(int argc, char* argv[]){
    return ns3::main(argc, argv);
}

