// ndn-meshed-as.cppp
// A-11: Attacker localized in local network

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
        ndnHelper.SetDefaultRoutes(true);
        ndnHelper.InstallAll();

        // Installing global routing interface on all nodes
        ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
        ndnGlobalRoutingHelper.InstallAll();

        // Getting consumers
        Ptr<Node> ucla_cs_attacker = Names::Find<Node>("ucla-cs"); //localized attacker

        Ptr<Node> ucla_math = Names::Find<Node>("ucla-math");
        Ptr<Node> az_math = Names::Find<Node>("az-math");
        Ptr<Node> az_cs = Names::Find<Node>("az-cs");
        Ptr<Node> wc_westwood = Names::Find<Node>("wc-westwood");
        Ptr<Node> wc_santa_monica = Names::Find<Node>("wc-santa-monica");
        Ptr<Node> mmphs_ee = Names::Find<Node>("mmphs-ee");

        // Getting producers
        Ptr<Node> mmphs_computer_bob00 = Names::Find<Node>("mmphs-computer-bob00");
        Ptr<Node> ucla_cs_webserver0_index = Names::Find<Node>("ucla-cs-webserver0-index");
        Ptr<Node> ucla_cs_alicelovecpp = Names::Find<Node>("ucla-cs-alicelovecpp"); // target


        // Manually configure UCLA AS routes
        ndn::FibHelper::AddRoute("rtr-az-1", "/edu/ucla/cs/webserver0/index.html", "rtr-ucla-1", 1);
        ndn::FibHelper::AddRoute("rtr-wc-1", "/edu/ucla/cs/webserver0/index.html", "rtr-ucla-1", 1);
        ndn::FibHelper::AddRoute("rtr-mmphs-1", "/edu/ucla/cs/webserver0/index.html", "rtr-ucla-2", 1);

        ndn::FibHelper::AddRoute("rtr-ucla-1", "/edu/ucla/cs/webserver0/index.html", "rtr-ucla-cs", 1);
        ndn::FibHelper::AddRoute("rtr-ucla-2", "/edu/ucla/cs/webserver0/index.html", "rtr-ucla-math", 1);

        ndn::FibHelper::AddRoute("rtr-ucla-math", "/edu/ucla/cs", "rtr-ucla-cs", 1);
        ndn::FibHelper::AddRoute("ucla-math", "/edu/ucla/cs", "rtr-ucla-math", 1);
        ndn::FibHelper::AddRoute("rtr-ucla-math", "/edu/ucla/cs", "rtr-ucla-cs", 1);
        ndn::FibHelper::AddRoute("ucla-cs", "/edu/ucla/cs", "rtr-ucla-cs", 1);
        ndn::FibHelper::AddRoute("rtr-ucla-cs", "/edu/ucla/cs/alicelovecpp", "ucla-cs-alicelovecpp", 1);
        ndn::FibHelper::AddRoute("rtr-ucla-cs", "/edu/ucla/cs/webserver0/index.html", "ucla-cs-webserver0-index", 1);

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

