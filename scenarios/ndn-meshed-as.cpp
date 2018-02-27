// ndn-meshed-as.cppp

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
